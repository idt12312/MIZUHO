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
	BlinkLedTask(uint8_t _led_num);
	virtual ~BlinkLedTask(){}

	void set_time(uint32_t _on_time, uint32_t _off_time);

private:
	uint8_t led_num;
	uint32_t on_time = 0;
	uint32_t off_time = 1000;

	virtual void task();
};

#endif /* TASK_BLINKLEDTASK_H_ */
