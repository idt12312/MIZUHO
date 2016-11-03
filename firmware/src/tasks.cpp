/*
 * tasks.cpp
 *
 *  Created on: 2016/10/31
 *      Author: idt12312
 */
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

#include "config.h"

#include "QuadratureDemodulator.h"
#include "WallDetect.h"

/************************************************
 * タスク間通信のためのハンドル
 ***********************************************/
static xQueueHandle wall_info_queue;

static void abort(const char* error_msg);
static void battery_monitor_task(void *);
static void wall_detect_task(void *);
static void test_task(void *);


void Tasks_init()
{
	wall_info_queue = xQueueCreate(1, sizeof(WallDetect::WallInfo));

	xTaskCreate(battery_monitor_task, "batt monitor",
			BATTERY_MONITOR_TASK_STACK_SIZE, NULL,
			BATTERY_MONITOR_TASK_PRIORITY, NULL);

	xTaskCreate(wall_detect_task, "wall detect",
			WALL_DETECT_TASK_STACK_SIZE, NULL,
			WALL_DETECT_TASK_PRIORITY, NULL);

	xTaskCreate(test_task, "test",
			256, NULL,
			1, NULL);
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
	WallDetect::WallInfo wall_info;
	TickType_t last_wake_tick = xTaskGetTickCount();

	while (1) {
		vTaskDelayUntil(&last_wake_tick, 300);
		last_wake_tick = xTaskGetTickCount();

		xQueuePeek(wall_info_queue, &wall_info, 300);
		printf("wall info : %d %d %d\n", wall_info.front, wall_info.left, wall_info.right);
	}
}
