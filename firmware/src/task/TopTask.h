/*
 * TopTask.h
 *
 *  Created on: 2016/11/15
 *      Author: idt12312
 */

#ifndef TASK_TOPTASK_H_
#define TASK_TOPTASK_H_

#include "TaskBase.h"
#include "MotionControlTask.h"
#include "MotorControlTask.h"

class TopTask: public TaskBase {
public:
	TopTask();
	virtual ~TopTask();

	void set_motor_control_task(MotorControlTask *p) { motor_control_task = p; }
	void set_motion_control_task(MotionControlTask *p) { motion_control_task = p; }


private:
	MotorControlTask *motor_control_task;
	MotionControlTask *motion_control_task;


	virtual void task();
};

#endif /* TASK_TOPTASK_H_ */
