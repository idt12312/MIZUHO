/*
 * WallDetect.h
 *
 *  Created on: 2016/10/31
 *      Author: idt12312
 */

#ifndef WALLDETECT_H_
#define WALLDETECT_H_

#include <algorithm>
#include "config.h"
#include "QuadratureDemodulator.h"

class WallDetect {
private:
	uint8_t right_cnt = 0;
	uint8_t left_cnt = 0;
	uint8_t front_cnt = 0;

public:
	WallDetect(){}
	virtual ~WallDetect(){}

	struct WallInfo {
		bool right;
		bool left;
		bool front;
	};

	WallInfo update(const QuadratureDemodulator::Result &sensor_value)
	{
		// 前壁の判定は左右の前壁センサの値の平均値を用いる
		// センサが閾値以下 -> カウンタをクリア
		// センサが閾値以上 -> カウンタをWALL_DETECT_CNTで飽和させながらカウントアップ
		if ((sensor_value.sensor1 + sensor_value.sensor4)/2 < WALL_DETECT_FRONT_THRETHOLD) front_cnt = 0;
		else front_cnt = std::min(front_cnt+1, WALL_DETECT_CNT);

		if (sensor_value.sensor2 < WALL_DETECT_RL_THREHOLD) left_cnt = 0;
		else left_cnt = std::min(left_cnt+1, WALL_DETECT_CNT);

		if (sensor_value.sensor3 < WALL_DETECT_RL_THREHOLD) right_cnt = 0;
		else right_cnt = std::min(right_cnt+1, WALL_DETECT_CNT);

		WallInfo wall_info;
		wall_info.front = front_cnt == WALL_DETECT_CNT;
		wall_info.right = right_cnt == WALL_DETECT_CNT;
		wall_info.left = left_cnt == WALL_DETECT_CNT;

		return wall_info;
	}
};


#endif /* WALLDETECT_H_ */
