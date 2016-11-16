/*
 * MotionControlTask.h
 *
 *  Created on: 2016/11/14
 *      Author: idt12312
 */

#ifndef TASK_MOTIONCONTROLTASK_H_
#define TASK_MOTIONCONTROLTASK_H_

#include "TaskBase.h"
#include "WallDetectTask.h"
#include "MotorControlTask.h"
#include "Odometry.h"
#include "TrackingController.h"
#include "Motion.h"

class MotionControlTask: public TaskBase {
public:
	MotionControlTask();
	virtual ~MotionControlTask();

	void set_wall_detec_task(WallDetectTask *p) { wall_detect_task = p; }
	void set_motor_control_task(MotorControlTask *p) { motor_control_task = p; }
	Position get_pos();

	void push_motion(Motion &motion);
	void wait_finish_motion();


private:
	xSemaphoreHandle motion_end_semaphore;
	xQueueHandle motion_queue;
	xQueueHandle pos_queue;

	WallDetectTask *wall_detect_task = nullptr;
	MotorControlTask *motor_control_task = nullptr;

	TrackingController tracking_controller;
	Odometry odometry;

	Motion *motion = nullptr;
	Position last_goal_pos;

	virtual void task();
	void update_odometry();
	void update_motion();
	void wall_trace_control();
};

#endif /* TASK_MOTIONCONTROLTASK_H_ */