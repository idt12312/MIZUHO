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

	size_t offset_cnt = 0;
	uint32_t right_offset_sum = 0;
	uint32_t left_offset_sum = 0;
	uint32_t right_offset = 0;
	uint32_t left_offset = 0;

public:
	WallDetect(){}
	virtual ~WallDetect(){}

	struct WallInfo {
		bool right;
		bool left;
		bool front;
		int32_t right_value;
		int32_t left_value;
	};

	void calib_offset()
	{
		offset_cnt = WALL_OFFSET_CNT;
		left_offset_sum = 0;
		right_offset_sum = 0;
	}

	WallInfo update(const QuadratureDemodulator::Result &sensor_value)
	{
		if (offset_cnt>0) {
			left_offset_sum += sensor_value.sensor2;
			right_offset_sum += sensor_value.sensor3;

			offset_cnt--;
			if (offset_cnt == 0) {
				left_offset = left_offset_sum / WALL_OFFSET_CNT;
				right_offset = right_offset_sum / WALL_OFFSET_CNT;
			}
		}


		// センサが閾値以下 -> カウンタをクリア
		// センサが閾値以上 -> カウンタをWALL_DETECT_CNTで飽和させながらカウントアップ
		if (sensor_value.sensor2 < WALL_DETECT_RL_THREHOLD) left_cnt = 0;
		else left_cnt = std::min(left_cnt+1, WALL_DETECT_CNT);

		if (sensor_value.sensor3 < WALL_DETECT_RL_THREHOLD) right_cnt = 0;
		else right_cnt = std::min(right_cnt+1, WALL_DETECT_CNT);

		// 前壁の判定は左右の前壁センサの値の平均値を用いる
		// 前壁は横に壁がある時とない時で閾値を変える
		if (sensor_value.sensor2 > WALL_DETECT_RL_THREHOLD && sensor_value.sensor3 > WALL_DETECT_RL_THREHOLD) {
			if ((sensor_value.sensor1 + sensor_value.sensor4)/2 < WALL_DETECT_FRONT_THRETHOLD_WITH_SIDE) front_cnt = 0;
			else front_cnt = std::min(front_cnt+1, WALL_DETECT_CNT);
		}
		else {
			if ((sensor_value.sensor1 + sensor_value.sensor4)/2 < WALL_DETECT_FRONT_THRETHOLD_NOSIDE) front_cnt = 0;
			else front_cnt = std::min(front_cnt+1, WALL_DETECT_CNT);
		}

		WallInfo wall_info;
		wall_info.front = front_cnt == WALL_DETECT_CNT;
		wall_info.right = right_cnt == WALL_DETECT_CNT;
		wall_info.left = left_cnt == WALL_DETECT_CNT;
		wall_info.right_value = sensor_value.sensor3 - right_offset;
		wall_info.left_value =  sensor_value.sensor2 - left_offset;

		return wall_info;
	}
};


#endif /* WALLDETECT_H_ */
