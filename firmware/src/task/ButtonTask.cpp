/*
 * ButtonTask.cpp
 *
 *  Created on: 2016/11/16
 *      Author: idt12312
 */

#include "ButtonTask.h"
#include "led_button.h"
#include <cstdio>

ButtonTask::ButtonTask()
:TaskBase("button", 5, 256)
{

}

ButtonTask::~ButtonTask()
{

}


void ButtonTask::task()
{
	TickType_t last_wake_tick = xTaskGetTickCount();
	while (1) {
		vTaskDelayUntil(&last_wake_tick, 50);
		last_wake_tick = xTaskGetTickCount();

		if (ButtonR_get()) {
			printf("button\n");
			motor_control_task->disable();
		}
	}
}
