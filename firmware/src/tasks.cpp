/*
 * tasks.cpp
 *
 *  Created on: 2016/10/31
 *      Author: idt12312
 */
#include "MotorController.h"
#include "Geometry.h"
#include "PIDController.h"
#include "Odometry.h"
#include "TrackingController.h"
#include "Trajectory.h"
#include <stdio.h>

#include "stm32f4xx.h"
#include "tasks.h"
#include "arm_math.h"

#include "FreeRTOS.h"
#include "task.h"
#include "semphr.h"
#include "queue.h"

#include "battery_monitor.h"
#include "led_button.h"
#include "uart.h"
#include "motor.h"
#include "enc.h"
#include "mpu6500.h"

#include "config.h"

#include "QuadratureDemodulator.h"
#include "WallDetect.h"

/************************************************
 * タスク間通信のためのハンドル
 ***********************************************/
static xQueueHandle wall_info_queue;
static xQueueHandle motor_ref_queue;
static xQueueHandle machine_velocity_queue;
static xQueueHandle trajectory_queue;
static xQueueHandle pos_queue;
static xSemaphoreHandle trajectory_end_semaphore;

static void abort(const char* error_msg);
static void battery_monitor_task(void *);
static void wall_detect_task(void *);
static void motor_control_task(void *);
static void tracking_control_task(void *);
static void test_task(void *);

static bool control_enable = false;
static bool is_first = true;
void Tasks_init()
{
	wall_info_queue = xQueueCreate(1, sizeof(WallDetect::WallInfo));
	motor_ref_queue = xQueueCreate(1, sizeof(Velocity));
	machine_velocity_queue = xQueueCreate(10, sizeof(Velocity));
	trajectory_queue = xQueueCreate(10, sizeof(Trajectory*));
	pos_queue = xQueueCreate(1, sizeof(Position));
	trajectory_end_semaphore = xSemaphoreCreateBinary();

	xTaskCreate(battery_monitor_task, "batt monitor",
			BATTERY_MONITOR_TASK_STACK_SIZE, NULL,
			BATTERY_MONITOR_TASK_PRIORITY, NULL);

	xTaskCreate(wall_detect_task, "wall detect",
			WALL_DETECT_TASK_STACK_SIZE, NULL,
			WALL_DETECT_TASK_PRIORITY, NULL);

	static PIDParam pid_param;
	pid_param.T = MOTOR_CONTROL_TASK_PERIOD_SEC;
	pid_param.Kp = 1.0f;
	pid_param.Ki = 0.01f;
	pid_param.Kd = 0.0001f;
	xTaskCreate(motor_control_task, "motor ctrl",
			MOTOR_CONTROL_TASK_STACK_SIZE, &pid_param,
			MOTOR_CONTROL_TASK_PRIORITY, NULL);


	static PIDParam track_param[3];
	// pos x
	track_param[0].T = TRACKING_CONTROL_TASK_PERIOD_SEC;
	track_param[0].Kp = 80.0f;
	track_param[0].Ki = 0.00f;
	track_param[0].Kd = 1.0f;

	// pox y
	track_param[1].T = TRACKING_CONTROL_TASK_PERIOD_SEC;
	track_param[1].Kp = 60.0f;
	track_param[1].Ki = 0.0f;
	track_param[1].Kd = 0.05f;

	// angle
	track_param[2].T = TRACKING_CONTROL_TASK_PERIOD_SEC;
	track_param[2].Kp = 3.0f;
	track_param[2].Ki = 0.05f;
	track_param[2].Kd = 0.0f;
	xTaskCreate(tracking_control_task, "track ctrl",
			TRACKING_CONTROL_TASK_STACK_SIZE, track_param,
			TRACKING_CONTROL_TASK_PRIORITY, NULL);


	xTaskCreate(test_task, "test",
			1024, NULL,
			1, NULL);

	control_enable = false;
}


// 定期的にバッテリー電圧をサンプルし、
// 数サンプルの平均値が基準値以下の場合はエラーを出す
void battery_monitor_task(void *)
{
	#define ADC_TO_BATTERY_VOLTAGE(x)	((float)x / 4096.0f * 3.3f * 3.0f)
	float voltage_sum = 0.0;
	uint8_t voltage_cnt = 0;

	TickType_t last_wake_tick = xTaskGetTickCount();
	while (1) {
		vTaskDelayUntil(&last_wake_tick, BATTERY_MONITOR_TASK_PERIOD);
		last_wake_tick = xTaskGetTickCount();

		voltage_sum += ADC_TO_BATTERY_VOLTAGE(BatteryMonitor_read());
		voltage_cnt++;

		if (voltage_cnt > BATTERY_MONITOR_AVE_CNT) {
			const float battery_voltage = voltage_sum / voltage_cnt;
			voltage_sum = 0.0;
			voltage_cnt = 0;
			printf("battery voltage: %d\n", (int16_t)(battery_voltage*1000));

			if (battery_voltage < BATTERY_MONITOR_ALERT_THREHOLD) {
				abort("Low Battery");
			}
		}
	}
}



void wall_detect_task(void *)
{
	SensorRawData raw;
	QuadratureDemodulator demodulator(64,4);
	WallDetect wall_detector;

	TickType_t last_wake_tick = xTaskGetTickCount();
	while (1) {
		vTaskDelayUntil(&last_wake_tick, WALL_DETECT_TASK_PERIOD);
		last_wake_tick = xTaskGetTickCount();

		// 赤外線センサの読み取りを開始し、終了をタスクをブロックして待機
		IrSensor_start();
		IrSensor_block(WALL_DETECT_TASK_PERIOD);
		IrSensor_get(&raw);

		// データ処理
		QuadratureDemodulator::Result result = demodulator.calc(&raw);
		//printf("raw:%5u %5u %5u %5u\n", result.sensor1,result.sensor2,result.sensor3,result.sensor4);
		WallDetect::WallInfo wall_info = wall_detector.update(result);

		xQueueOverwrite(wall_info_queue, &wall_info);

		// 壁の状態をLEDで表示
		if (wall_info.front) Led_on(LED_2 | LED_3);
		else Led_off(LED_2 | LED_3);
		if (wall_info.left) Led_on(LED_1);
		else Led_off(LED_1);
		if (wall_info.right) Led_on(LED_4);
		else Led_off(LED_4);
	}
}



void motor_control_task(void *arg)
{
	PIDParam *control_param = (PIDParam*)arg;
	MotorController motor_controller(*control_param);

	Velocity dummy;
	xQueueSend(machine_velocity_queue, &dummy, 0);

	TickType_t last_wake_tick = xTaskGetTickCount();
	while (1) {
		vTaskDelayUntil(&last_wake_tick, MOTOR_CONTROL_TASK_PERIOD);
		last_wake_tick = xTaskGetTickCount();

		const float gyro = MPU6500_read_gyro_z();

		EncValue enc_value;
		Enc_read(&enc_value);

		Velocity ref;
		if (xQueuePeek(motor_ref_queue, &ref, MOTOR_CONTROL_TASK_PERIOD) == pdFALSE) continue;

		MotorVoltage motor_voltage = motor_controller.update(ref, enc_value, gyro);
		if (!control_enable) {
			motor_voltage.left = motor_voltage.right = 0.0f;
			motor_controller.reset();
		}
		Motor_set_voltage(&motor_voltage);

		Velocity measured_velocity = motor_controller.get_measured_velocity();
#if USE_GYRO==1
		measured_velocity.omega = gyro;
#endif
		xQueueSend(machine_velocity_queue, &measured_velocity, MOTOR_CONTROL_TASK_PERIOD);
	}
}



static void tracking_control_task(void *arg)
{
	PIDParam* controller_param = (PIDParam*)arg;
	TrackingController tracking_controller(controller_param[0], controller_param[1], controller_param[2]);
	Odometry odometry(MOTOR_CONTROL_TASK_PERIOD_SEC);
	Trajectory *traj = nullptr;
	TrajectoryTarget target(TrajectoryTarget::Type::PIVOT, Position(), Velocity());
	xQueueSend(pos_queue, &odometry.get_pos(), TRACKING_CONTROL_TASK_PERIOD);

	Position last_target_pos;

	TickType_t last_wake_tick = xTaskGetTickCount();
	while (1) {
		vTaskDelayUntil(&last_wake_tick, TRACKING_CONTROL_TASK_PERIOD);
		last_wake_tick = xTaskGetTickCount();

		Velocity measured_velocity;
		while (xQueueReceive(machine_velocity_queue, &measured_velocity, 0) == pdTRUE) {
			odometry.update(measured_velocity);
		}
		xQueueOverwrite(pos_queue, &odometry.get_pos());


		if (traj == nullptr || traj->is_end()) {
			// 新しい軌道シーケンスをセット
			if (xQueueReceive(trajectory_queue, &traj, 0) == pdTRUE) {
				Position current_pos = odometry.get_pos();
				odometry.reset();
				if (is_first) {
					is_first = false;
				}
				else {
					odometry.set_pos(current_pos-last_target_pos);
				}
				traj->adjust_start_position(odometry.get_pos());
				tracking_controller.reset();
			}
			else {
				// 終了を通知
			}
		}

		if (traj == nullptr) continue;

		target = traj->next();
		last_target_pos = target.pos;


		WallDetect::WallInfo wall_info;
		xQueuePeek(wall_info_queue, &wall_info, TRACKING_CONTROL_TASK_PERIOD);
		// 直進時は壁補正をかける
		if (target.type == TrajectoryTarget::Type::STRAIGHT) {
			if (wall_info.left && wall_info.right) {
				// 右によっている　右のセンサの値が大きくなる rl_diffが大きくなる
				// pos.xを大きくする 原点が左に動いたことになる 機体は左に行こうとする
				// rl_diffは+-3500のオーダ　中心付近では多分+-1000くらいで変動
				Position current_pos = odometry.get_pos();
				current_pos.x += WALL_COMPENSATOR_P_GAIN*wall_info.rl_diff;
				odometry.set_pos(current_pos);
			}
		}

		Velocity motor_ref;
		motor_ref = tracking_controller.update(odometry.get_pos(), target);
		xQueueOverwrite(motor_ref_queue, &motor_ref);

		if (traj->is_end()) xSemaphoreGive(trajectory_end_semaphore);
	}
}


static void abort(const char* error_msg)
{
	vTaskSuspendAll();

	float duty[2] = {0.0, 0.0};
	Motor_set_duty(duty);

	Led_on(LED_ALL);

	printf("[ERROR] %s\n", error_msg);
	while(1);
}


static void test_task(void *)
{
	TickType_t last_wake_tick = xTaskGetTickCount();

	Velocity motor_ref(0.0f, 0.0f);
	xQueueOverwrite(motor_ref_queue, &motor_ref);

	Velocity measured;

	Straight straight1(BLOCK_SIZE*1, 0, STRAIGHT_DEFAULT_VELOCITY);
	//PivotTurn turn(-PI/2);
	SlalomTurn slalom_right(-PI/2);
	SlalomTurn slalom_left(PI/2);
	Straight straight2(BLOCK_SIZE*1, STRAIGHT_DEFAULT_VELOCITY, 0);
	Trajectory *traj;

	while (1) {
		vTaskDelayUntil(&last_wake_tick, 100);
		last_wake_tick = xTaskGetTickCount();

		char ch = 0;
		if (Uart_rcv_size()) {
			ch = Uart_read_byte();
		}
		if (ButtonL_get() || ch == 's') {
			vTaskDelay(2000);
			MPU6500_calib_offset();
			vTaskDelay(1000);
			last_wake_tick = xTaskGetTickCount();
			control_enable = true;
			traj = &straight1;
			xQueueSend(trajectory_queue, &traj, 10);
			xSemaphoreTake(trajectory_end_semaphore, 10000);
			printf("end straight1\n");

			WallDetect::WallInfo wall_info;
			xQueuePeek(wall_info_queue, &wall_info,10);
			if (!wall_info.right) {
				traj = &slalom_right;
				xQueueSend(trajectory_queue, &traj, 10);
				xSemaphoreTake(trajectory_end_semaphore, 10000);
				printf("end right\n");
			}
			else if (!wall_info.left) {
				traj = &slalom_left;
				xQueueSend(trajectory_queue, &traj, 10);
				xSemaphoreTake(trajectory_end_semaphore, 10000);
				printf("end left\n");
			}
			traj = &straight2;
			xQueueSend(trajectory_queue, &traj, 10);
			printf("end straight2\n");

		}
		if (ButtonR_get() || ch == 'q') {
			control_enable = false;
		}

		Position pos;
		xQueuePeek(pos_queue, &pos, 100);
		//printf("%d %d %d\n", (int)(pos.x*1000), (int)(pos.y*1000), (int)(pos.theta/PI*180));

	}
}
