/*
 * MotorControlTask.h
 *
 *  Created on: 2016/11/14
 *      Author: idt12312
 */

#ifndef TASK_MOTORCONTROLTASK_H_
#define TASK_MOTORCONTROLTASK_H_

#include "TaskBase.h"
#include "Geometry.h"
#include "MotorController.h"

class MotorControlTask: public TaskBase {
public:
	MotorControlTask();
	virtual ~MotorControlTask();

	void set_reference(const Velocity &ref);
	bool get_measured_velocity(Velocity *ref);

	void enable();
	void disable();


private:
	MotorController motor_controller;
	xQueueHandle reference_velocity_queue;
	xQueueHandle measured_velocity_queue;
	bool control_enable = false;
	bool control_reset_req = false;

	virtual void task();
};

#endif /* TASK_MOTORCONTROLTASK_H_ */
