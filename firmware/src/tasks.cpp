/*
 * tasks.cpp
 *
 *  Created on: 2016/10/31
 *      Author: idt12312
 */
#include "MotorController.h"
#include "Geometry.h"
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

static void abort(const char* error_msg);
static void battery_monitor_task(void *);
static void wall_detect_task(void *);
static void motor_control_task(void *);
static void test_task(void *);

static bool control_enable = false;

void Tasks_init()
{
	wall_info_queue = xQueueCreate(1, sizeof(WallDetect::WallInfo));
	motor_ref_queue = xQueueCreate(1, sizeof(Velocity));
	machine_velocity_queue = xQueueCreate(1, sizeof(Velocity));

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

	// 初回チェック
	if (ADC_TO_BATTERY_VOLTAGE(BatteryMonitor_read()) < BATTERY_MONITOR_ALERT_THREHOLD) {
		abort("Low Battery");
	}

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
			float duty[2];
			duty[0] = 0.0;
			duty[1] = 0.0;
			Motor_set_duty(duty);
		}
		Motor_set_voltage(&motor_voltage);

		const Velocity measured_velocity = motor_controller.get_measured_velocity();
		//xQueueSend(machine_velocity_queue, &measured_velocity, MOTOR_CONTROL_TASK_PERIOD);

		xQueueOverwrite(machine_velocity_queue, &measured_velocity);
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

	Velocity motor_ref(0.0f, 3.0f);
	xQueueOverwrite(motor_ref_queue, &motor_ref);

	Velocity measured;

	while (1) {
		vTaskDelayUntil(&last_wake_tick, 200);
		last_wake_tick = xTaskGetTickCount();

		if (Uart_rcv_size() != 0) {
			char ch = Uart_read_byte();
			if (ch == 's') control_enable = true;
			else control_enable = false;

		}
		xQueuePeek(machine_velocity_queue, &measured, 0);
		printf("%d %d\n", (int32_t)(measured.v*1000), (int32_t)(measured.omega*1000));
	}
}