/*
 * TopTask.cpp
 *
 *  Created on: 2016/11/15
 *      Author: idt12312
 */

#include "TopTask.h"
#include "config.h"
#include "led_button.h"
#include "mpu6500.h"
#include <cstdio>
#include "arm_math.h"



TopTask::TopTask()
:TaskBase("top", TOP_TASK_PRIORITY, TOP_TASK_STACK_SIZE)
{

}


TopTask::~TopTask()
{

}


void TopTask::task()
{
	Straight straight1(BLOCK_SIZE*0.5, 0, 0);
	PivotTurn turn(PI);
	Straight straight2(-BLOCK_SIZE*0.5, 0, 0);
	Straight straight3(BLOCK_SIZE*0.5, 0, 0);


	TickType_t last_wake_tick = xTaskGetTickCount();
	while (1) {
		vTaskDelayUntil(&last_wake_tick, 100);
		last_wake_tick = xTaskGetTickCount();

		if (ButtonL_get()) {
			vTaskDelay(2000);
			MPU6500_calib_offset();
			vTaskDelay(1000);
			motor_control_task->enable();

			motion_control_task->push_motion(straight1);
			motion_control_task->push_motion(turn);
			motion_control_task->push_motion(straight2);

			straight3.set_adjust_odometry_flag();
			motion_control_task->push_motion(straight3);
		}
		if (ButtonR_get()) {
			motor_control_task->disable();
		}


		Position pos = motion_control_task->get_pos();
		std::printf("%d %d %d\n", (int)(pos.x*1000), (int)(pos.y*1000), (int)(pos.theta/PI*180));
	}
}
