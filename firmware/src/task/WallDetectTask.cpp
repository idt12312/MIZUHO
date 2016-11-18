/*
 * WallDetectTask.cpp
 *
 *  Created on: 2016/11/14
 *      Author: idt12312
 */

#include "WallDetectTask.h"
#include "led_button.h"
#include <cstdio>

WallDetectTask::WallDetectTask()
:TaskBase("wall detect", WALL_DETECT_TASK_PRIORITY, WALL_DETECT_TASK_STACK_SIZE),
 demodulator(64,4)
{
	wall_info_queue = xQueueCreate(1, sizeof(WallDetect::WallInfo));
}


WallDetectTask::~WallDetectTask()
{
	vQueueDelete(wall_info_queue);
}


WallDetect::WallInfo WallDetectTask::get_wall_info()
{
	WallDetect::WallInfo wall_info;
	xQueuePeek(wall_info_queue, &wall_info, 0);
	return wall_info;
}


void WallDetectTask::task()
{
	TickType_t last_wake_tick = xTaskGetTickCount();
	while (1) {
		vTaskDelayUntil(&last_wake_tick, WALL_DETECT_TASK_PERIOD);
		last_wake_tick = xTaskGetTickCount();

		// 赤外線センサの読み取りを開始し、終了をタスクをブロックして待機
		SensorRawData raw;
		IrSensor_start();
		IrSensor_block(WALL_DETECT_TASK_PERIOD);
		IrSensor_get(&raw);

		// データ処理
		QuadratureDemodulator::Result result = demodulator.calc(&raw);
		WallDetect::WallInfo wall_info = wall_detector.update(result);

		xQueueOverwrite(wall_info_queue, &wall_info);

#if PRINT_WALL_SENSOR_RAW_VALUE==1
		if (wall_info.front) Led_on(LED_2 | LED_3);
		else Led_off(LED_2 | LED_3);
		if (wall_info.left) Led_on(LED_1);
		else Led_off(LED_1);
		if (wall_info.right) Led_on(LED_4);
		else Led_off(LED_4);

		static int print_cnt = 0;
		print_cnt++;
		if (print_cnt > 100/WALL_DETECT_TASK_PERIOD) {
			print_cnt = 0;
			std::printf("raw:%5u %5u %5u %5u\n", result.sensor1,result.sensor2,result.sensor3,result.sensor4);
		}
#endif

	}
}
