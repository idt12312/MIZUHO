/*
 * BlinkLedTask.cpp
 *
 *  Created on: 2016/11/18
 *      Author: idt12312
 */


#include "BlinkLedTask.h"
#include "config.h"

BlinkLedTask::BlinkLedTask(uint8_t _led_num)
:TaskBase("blink led", BLINK_LED_TASK_PRIORITY, BLINK_LED_TASK_STACK_SIZE), led_num(_led_num)
{

}


void BlinkLedTask::set_time(uint32_t _on_time, uint32_t _off_time)
{
	on_time = _on_time;
	off_time = _off_time;
}

void BlinkLedTask::task()
{
	while (1)  {
		if (on_time > 0) {
			Led_on(led_num);
			vTaskDelay(on_time);
		}

		if (off_time > 0) {
			Led_off(led_num);
			vTaskDelay(off_time);
		}
	}
}
