/*
 * tasks.cpp
 *
 *  Created on: 2016/10/31
 *      Author: idt12312
 */
#include "Motion.h"
#include "MotorController.h"
#include "Geometry.h"
#include "PIDController.h"
#include "Odometry.h"
#include "TrackingController.h"
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
static xQueueHandle motion_queue;
static xQueueHandle pos_queue;
static xSemaphoreHandle motion_end_semaphore;

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
	motion_queue = xQueueCreate(10, sizeof(Motion*));
	pos_queue = xQueueCreate(1, sizeof(Position));
	motion_end_semaphore = xSemaphoreCreateBinary();

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

#if ENABLE_BATTERY_MONITOR_ABORT==1
			if (battery_voltage < BATTERY_MONITOR_ALERT_THREHOLD) {
				abort("Low Battery");
			}
#endif
		}
	}
}



void wall_detect_task(void *)
{
	QuadratureDemodulator demodulator(64,4);
	WallDetect wall_detector;

	TickType_t last_wake_tick = xTaskGetTickCount();
	while (1) {
		vTaskDelayUntil(&last_wake_tick, WALL_DETECT_TASK_PERIOD);
		last_wake_tick = xTaskGetTickCount();

		// 赤外線センサの読み取りを開始し、終了をタスクをブロックして待機
		SensorRawData raw;
		IrSensor_start();
		IrSensor_block(WALL_DETECT_TASK_PERIOD);
		IrSensor_get(&raw);

		// データ処理
		QuadratureDemodulator::Result result = demodulator.calc(&raw);
		WallDetect::WallInfo wall_info = wall_detector.update(result);

#if PRINT_WALL_SENSOR_RAW_VALUE==1
		printf("raw:%5u %5u %5u %5u\n", result.sensor1,result.sensor2,result.sensor3,result.sensor4);
#endif

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
#if ODOMETRY_USE_GYRO==1
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
	bool is_first = true;

	Motion *motion = nullptr;
	Position last_target_pos;

	TickType_t last_wake_tick = xTaskGetTickCount();
	while (1) {
		vTaskDelayUntil(&last_wake_tick, TRACKING_CONTROL_TASK_PERIOD);
		last_wake_tick = xTaskGetTickCount();

		// motor controllerから送られてきたマシンの速度情報を受け取り、
		// odometryを更新する
		Velocity measured_velocity;
		while (xQueueReceive(machine_velocity_queue, &measured_velocity, 0) == pdTRUE) {
			odometry.update(measured_velocity);
		}
		xQueueOverwrite(pos_queue, &odometry.get_pos());


		// motionが終了していたら新たなmotionをqueueから取り出す
		bool is_idle = false;
		if (motion == nullptr || motion->is_end()) {
			bool adjust_odometry_flag = motion->get_adjust_odometry_flag();

			// 新しいMotionをセット
			if (xQueueReceive(motion_queue, &motion, 0) == pdTRUE) {
				Position current_pos = odometry.get_pos();
				odometry.reset();

				// odometryをリセットする
				// 直前の目標点と現在の座標のずれを計算し、odometryを補正する
				if (is_first) {
					is_first = false;
				}
				else {
					Position pos_err = current_pos-last_target_pos;
					if (adjust_odometry_flag) {
						pos_err.y = 0.0f;
						pos_err.theta = 0.0f;
					}
					odometry.set_pos(pos_err);
				}
				motion->reset(odometry.get_pos());
				tracking_controller.reset();
			}
			else {
				is_idle = true;
			}
		}

		if (is_idle) {
			Velocity motor_ref(0,0);
			xQueueOverwrite(motor_ref_queue, &motor_ref);
			continue;
		}

		// 次の目標点をセット
		TrackingTarget target = motion->next();
		last_target_pos = target.pos;

		// 壁を使ったx方向の補正
		// odometryのx座標をずらす
		// 補正は直進時のみかける
		WallDetect::WallInfo wall_info;
		xQueuePeek(wall_info_queue, &wall_info, TRACKING_CONTROL_TASK_PERIOD);
		if (target.type == TrackingTarget::Type::STRAIGHT) {
			if (wall_info.left && wall_info.right) {
				// 右によっている　右のセンサの値が大きくなる rl_diffが大きくなる
				// pos.xを大きくする 原点が左に動いたことになる 機体は左に行こうとする
				// rl_diffは+-3500のオーダ　中心付近では多分+-1000くらいで変動
				Position current_pos = odometry.get_pos();
				current_pos.x += TRACKING_CONTROL_WALL_P_GAIN * wall_info.rl_diff;
				odometry.set_pos(current_pos);
			}
		}

		// 目標点に追従をする制御
		Velocity motor_ref;
		motor_ref = tracking_controller.update(odometry.get_pos(), target);
		xQueueOverwrite(motor_ref_queue, &motor_ref);

		// 1つのmotionが終わったことを通知する
		if (motion->is_end()) xSemaphoreGive(motion_end_semaphore);
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

	Straight straight1(-BLOCK_SIZE*0.5, 0, 0);
	PivotTurn turn(PI);
	SlalomTurn slalom_right(-PI/2);
	SlalomTurn slalom_left(PI/2);
	Straight straight2(BLOCK_SIZE*0.5, 0, 0);
	Motion *motion;

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

			motion = &straight2;
			xQueueSend(motion_queue, &motion, 10);

			motion = &turn;
			xQueueSend(motion_queue, &motion, 10);

			straight1.set_adjust_odometry_flag();
			motion = &straight1;
			xQueueSend(motion_queue, &motion, 10);

			motion = &straight2;
			xQueueSend(motion_queue, &motion, 10);
		}
		if (ButtonR_get() || ch == 'q') {
			control_enable = false;
		}

		Position pos;
		xQueuePeek(pos_queue, &pos, 100);
		printf("%d %d %d\n", (int)(pos.x*1000), (int)(pos.y*1000), (int)(pos.theta/PI*180));

	}
}
