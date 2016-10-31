/*
 * tasks.cpp
 *
 *  Created on: 2016/10/31
 *      Author: idt12312
 */
#include <stdio.h>

#include "stm32f4xx.h"
#include "tasks.h"

#include "FreeRTOS.h"
#include "task.h"

#include "battery_monitor.h"
#include "led_button.h"
#include "uart.h"
#include "motor.h"


#define BATTERY_MONITOR_TASK_PERIOD	2000
#define ADC_TO_BATTERY_VOLTAGE(x)	((float)x / 4096.0f * 3.3f * 3.0f)
#define BATTERY_AVE_CNT	5
#define BATTERY_LOW_THREHOLD	7.4f


static void abort(const char* error_msg)
{
	vTaskSuspendAll();

	float duty[2] = {0.0, 0.0};
	Motor_set(duty);

	Led_on(LED_ALL);

	printf("[ERROR] %s\n", error_msg);
	while(1);
}

// 定期的にバッテリー電圧をサンプルし、
// 数サンプルの平均値が基準値以下の場合はエラーを出す
void battery_monitor_task(void *)
{
	float voltage_sum = 0.0;
	uint8_t voltage_cnt = 0;

	// 初回チェック
	if (ADC_TO_BATTERY_VOLTAGE(BatteryMonitor_read()) < BATTERY_LOW_THREHOLD) {
		abort("Low Battery");
	}

	TickType_t last_wake_tick = xTaskGetTickCount();
	while (1) {
		vTaskDelayUntil(&last_wake_tick, BATTERY_MONITOR_TASK_PERIOD);
		last_wake_tick = xTaskGetTickCount();

		voltage_sum += ADC_TO_BATTERY_VOLTAGE(BatteryMonitor_read());
		voltage_cnt++;

		if (voltage_cnt > BATTERY_AVE_CNT) {
			const float battery_voltage = voltage_sum / voltage_cnt;
			voltage_sum = 0.0;
			voltage_cnt = 0;
			printf("battery voltage: %d\n", (int16_t)(battery_voltage*1000));

			if (battery_voltage < BATTERY_LOW_THREHOLD) {
				abort("Low Battery");
			}
		}
	}
}

