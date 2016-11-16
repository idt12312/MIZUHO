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
	Straight straight_start(BLOCK_SIZE-MACHINE_AXLE_TO_TAIL, 0, STRAIGHT_DEFAULT_VELOCITY);
	SlalomTurn slalom(PI/2);
	Straight straight_end(BLOCK_SIZE/2, STRAIGHT_DEFAULT_VELOCITY, 0);
	PivotTurn turn(PI/2);
	Straight back(-(BLOCK_SIZE/2-MACHINE_AXLE_TO_TAIL+MACHINE_TAIL_COMP_LEN), 0, 0, true);


	TickType_t last_wake_tick = xTaskGetTickCount();
	while (1) {
		vTaskDelayUntil(&last_wake_tick, 100);
		last_wake_tick = xTaskGetTickCount();

		if (ButtonL_get()) {
			printf("start\n");
			vTaskDelay(2000);
			MPU6500_calib_offset();
			vTaskDelay(1000);
			printf("calib end\n");
			motor_control_task->enable();

			for (int i=0;i<3;i++) {
				motion_control_task->push_motion(straight_start);
				motion_control_task->wait_finish_motion();

				motion_control_task->push_motion(slalom);
				motion_control_task->wait_finish_motion();

				motion_control_task->push_motion(straight_end);
				motion_control_task->wait_finish_motion();

				motion_control_task->push_motion(turn);
				motion_control_task->wait_finish_motion();

				motion_control_task->push_motion(back);
				motion_control_task->wait_finish_motion();
			}
			printf("end\n");


			motor_control_task->disable();
		}


		Position pos = motion_control_task->get_pos();
		//std::printf("%d %d %d\n", (int)(pos.x*1000), (int)(pos.y*1000), (int)(pos.theta/PI*180));
	}
}
