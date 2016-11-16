/*
 * BatteryMonitorTask.h
 *
 *  Created on: 2016/11/14
 *      Author: idt12312
 */

#ifndef TASK_BATTERYMONITORTASK_H_
#define TASK_BATTERYMONITORTASK_H_

#include "TaskBase.h"

// 定期的にバッテリー電圧をサンプルし、
// 数サンプルの平均値が基準値以下の場合はエラーを出す
class BatteryMonitorTask: public TaskBase {
public:
	BatteryMonitorTask();
	virtual ~BatteryMonitorTask();

private:
	float voltage_sum = 0.0f;
	uint8_t voltage_cnt = 0;

	virtual void task();
};

#endif /* TASK_BATTERYMONITORTASK_H_ */
