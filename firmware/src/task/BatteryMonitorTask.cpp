/*
 * BatteryMonitorTask.cpp
 *
 *  Created on: 2016/11/14
 *      Author: idt12312
 */

#include "BatteryMonitorTask.h"
#include "tasks.h"
#include "battery_monitor.h"
#include "config.h"
#include <cstdio>

BatteryMonitorTask::BatteryMonitorTask()
:TaskBase("batt monitor", BATTERY_MONITOR_TASK_PRIORITY, BATTERY_MONITOR_TASK_STACK_SIZE)
{
}

BatteryMonitorTask::~BatteryMonitorTask()
{
}

void BatteryMonitorTask::task()
{
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

#if ENABLE_BATTERY_MONITOR_PRINT==1
			std::printf("battery voltage: %d\n", (int16_t)(battery_voltage*1000));
#endif

#if ENABLE_BATTERY_MONITOR_ABORT==1
			if (battery_voltage < BATTERY_MONITOR_ALERT_THREHOLD) {
				Tasks_abort("Low Battery");
			}
#endif
		}
	}
}
