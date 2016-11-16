/*
 * BlinkLed.h
 *
 *  Created on: 2016/11/14
 *      Author: idt12312
 */

#ifndef TASK_BLINKLEDTASK_H_
#define TASK_BLINKLEDTASK_H_

#include "TaskBase.h"
#include "led_button.h"

class BlinkLedTask: public TaskBase {
public:
	BlinkLedTask(uint8_t _led_num, uint32_t _period)
	:TaskBase("blink led", 5, 256), led_num(_led_num), period(_period)
	{}
	virtual ~BlinkLedTask(){}

private:
	uint8_t led_num;
	uint32_t period;

	virtual void task()
	{
		TickType_t last_wake_tick = xTaskGetTickCount();

		while (1)  {
			vTaskDelayUntil(&last_wake_tick, period);
			last_wake_tick = xTaskGetTickCount();

			Led_toggle(led_num);
		}
	}
};

#endif /* TASK_BLINKLEDTASK_H_ */
