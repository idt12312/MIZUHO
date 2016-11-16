/*
 * ButtonTask.h
 *
 *  Created on: 2016/11/16
 *      Author: idt12312
 */

#ifndef TASK_BUTTONTASK_H_
#define TASK_BUTTONTASK_H_

#include "TaskBase.h"
#include "MotorControlTask.h"

class ButtonTask: public TaskBase {
public:
	ButtonTask();
	virtual ~ButtonTask();


	void set_motor_control_task(MotorControlTask *p) { motor_control_task = p; }


private:
	MotorControlTask *motor_control_task;

	virtual void task();
};

#endif /* TASK_BUTTONTASK_H_ */
