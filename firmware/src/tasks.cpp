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
	pid_param.T = 0.001f;
	pid_param.Kp = 0.8f;
	pid_param.Ki = 0.01f;
	pid_param.Kd = 0.0001f;
	xTaskCreate(motor_control_task, "motor ctrl",
			MOTOR_CONTROL_TASK_STACK_SIZE, &pid_param,
			MOTOR_CONTROL_TASK_PRIORITY, NULL);


	static PIDParam track_param[3];
	// pos x
	track_param[0].T = 0.005f;
	track_param[0].Kp = 20.0f;
	track_param[0].Ki = 0.00f;
	track_param[0].Kd = 0.01f;

	// pox y
	track_param[1].T = 0.005f;
	track_param[1].Kp = 50.0f;
	track_param[1].Ki = 0.1f;
	track_param[1].Kd = 0.02f;

	// angle
	track_param[2].T = 0.005f;
	track_param[2].Kp = 3.0f;
	track_param[2].Ki = 0.05f;
	track_param[2].Kd = 0.0f;
	xTaskCreate(tracking_control_task, "track ctrl",
			TRACKING_CONTROL_TASK_STACK_SIZE, track_param,
			TRACKING_CONTROL_TASK_PRIORITY, NULL);


	xTaskCreate(test_task, "test",
			256, NULL,
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

		const Velocity measured_velocity = motor_controller.get_measured_velocity();
		xQueueSend(machine_velocity_queue, &measured_velocity, MOTOR_CONTROL_TASK_PERIOD);
	}
}



static void tracking_control_task(void *arg)
{
	PIDParam* controller_param = (PIDParam*)arg;
	TrackingController tracking_controller(controller_param[0], controller_param[1], controller_param[2]);
	Odometry odometry(0.001);
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
				//TODO:odometryをリセット?
				Position current_pos = odometry.get_pos();
				odometry.reset();
				odometry.set_pos(current_pos-last_target_pos);
				tracking_controller.reset();
			}
			else {
				// 終了を通知
			}
		}

		if (traj == nullptr) continue;

		target = traj->next();
		last_target_pos = target.pos;

		// TODO:target.type==STRAIGHTのときに壁とかで補正をかける

		Velocity motor_ref;
		motor_ref = tracking_controller.update(odometry.get_pos(), target);
		xQueueOverwrite(motor_ref_queue, &motor_ref);
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

	Straight straight;
	//PivotTurnRight90 turn;
	SlalomTurnRight90 slalom;
	Trajectory *traj;

	vTaskDelay(3000);
	MPU6500_calib_offset();
	while (1) {
		vTaskDelayUntil(&last_wake_tick, 100);
		last_wake_tick = xTaskGetTickCount();

		if (ButtonL_get()) {
			vTaskDelay(2000);
			// TODO:odometryをリセットしたい
			last_wake_tick = xTaskGetTickCount();
			control_enable = true;
			traj = &straight;
			xQueueSend(trajectory_queue, &traj, 1);
			traj = &slalom;
			xQueueSend(trajectory_queue, &traj, 1);
		}
		if (ButtonR_get()) {
			control_enable = false;
		}

		Position pos;
		xQueuePeek(pos_queue, &pos, 100);
		printf("%d %d %d\n", (int)(pos.x*1000), (int)(pos.y*1000), (int)(pos.theta/PI*180));
	}
}
