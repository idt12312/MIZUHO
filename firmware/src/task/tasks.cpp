/*
 * tasks.cpp
 *
 *  Created on: 2016/10/31
 *      Author: idt12312
 */


#include <cstdio>

#include "FreeRTOS.h"
#include "task.h"

#include "tasks.h"
#include "led_button.h"
#include "motor.h"
#include "config.h"


extern "C" {
void vApplicationStackOverflowHook( TaskHandle_t xTask, char *pcTaskName );
void vApplicationMallocFailedHook();
void vApplicationIdleHook();
void vApplicationTickHook();
}


void vApplicationStackOverflowHook( TaskHandle_t xTask, char *pcTaskName )
{
	std::printf("\n[ERROR] Stack Overflow %s\n", pcTaskName);
	while(1);
}


void vApplicationMallocFailedHook()
{
	std::printf("\n[ERROR] Malloc Failed\n");
	while(1);
}


void vApplicationIdleHook()
{

}


void vApplicationTickHook()
{

}


void Tasks_abort(const char* error_msg)
{
	vTaskSuspendAll();

	float duty[2] = {0.0, 0.0};
	Motor_set_duty(duty);

	Led_on(LED_ALL);

	printf("[ERROR] %s\n", error_msg);
	while(1);
}

