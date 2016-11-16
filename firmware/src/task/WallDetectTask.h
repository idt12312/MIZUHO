/*
 * WallDetectTask.h
 *
 *  Created on: 2016/11/14
 *      Author: idt12312
 */

#ifndef TASK_WALLDETECTTASK_H_
#define TASK_WALLDETECTTASK_H_


#include "TaskBase.h"
#include "QuadratureDemodulator.h"
#include "WallDetect.h"


class WallDetectTask: public TaskBase {
public:
	WallDetectTask();
	virtual ~WallDetectTask();

	WallDetect::WallInfo get_wall_info();


private:
	QuadratureDemodulator demodulator;
	WallDetect wall_detector;
	xQueueHandle wall_info_queue;

	virtual void task();
};

#endif /* TASK_WALLDETECTTASK_H_ */
