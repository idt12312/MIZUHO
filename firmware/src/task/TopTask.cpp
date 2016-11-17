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


Straight TopTask::search_straight_start(BLOCK_SIZE-MACHINE_AXLE_TO_TAIL, 0, SEARCH_STRAIGHT_DEFAULT_VELOCITY, SEARCH_STRAIGHT_ACCERALATION, SEARCH_STRAIGHT_MAX_VELOCITY);
Straight TopTask::search_straight_half_start(BLOCK_SIZE/2-MACHINE_AXLE_TO_TAIL, 0, SEARCH_STRAIGHT_DEFAULT_VELOCITY, SEARCH_STRAIGHT_ACCERALATION, SEARCH_STRAIGHT_MAX_VELOCITY);
Straight TopTask::search_straight_half_start_end(BLOCK_SIZE/2-MACHINE_AXLE_TO_TAIL, 0, 0, SEARCH_STRAIGHT_ACCERALATION, SEARCH_STRAIGHT_MAX_VELOCITY);
Straight TopTask::search_straight_end(BLOCK_SIZE/2, SEARCH_STRAIGHT_DEFAULT_VELOCITY, 0, SEARCH_STRAIGHT_ACCERALATION, SEARCH_STRAIGHT_MAX_VELOCITY);
Straight TopTask::search_straight(BLOCK_SIZE, SEARCH_STRAIGHT_DEFAULT_VELOCITY, SEARCH_STRAIGHT_DEFAULT_VELOCITY, SEARCH_STRAIGHT_ACCERALATION, SEARCH_STRAIGHT_MAX_VELOCITY);
Straight TopTask::search_back(-(BLOCK_SIZE/2-MACHINE_AXLE_TO_TAIL+MACHINE_TAIL_COMP_LEN), 0, 0, true, SEARCH_STRAIGHT_ACCERALATION, SEARCH_STRAIGHT_MAX_VELOCITY);
PivotTurn TopTask::search_turn180(PI, SEARCH_PIVOT_ROTATION_ACCERALATION, SEARCH_PIVOT_ROTATION_VELOCITY);
PivotTurn TopTask::search_turn90(PI/2, SEARCH_PIVOT_ROTATION_ACCERALATION, SEARCH_PIVOT_ROTATION_VELOCITY);
SlalomTurn TopTask::search_turn_right(-PI/2, SEARCH_STRAIGHT_DEFAULT_VELOCITY, SEARCH_SLALOM_ROTATION_ACCERALATION, SEARCH_SLALOM_ROTATION_VELOCITY);
SlalomTurn TopTask::search_turn_left(PI/2, SEARCH_STRAIGHT_DEFAULT_VELOCITY, SEARCH_SLALOM_ROTATION_ACCERALATION, SEARCH_SLALOM_ROTATION_VELOCITY);


Straight TopTask::fast_straight_start(BLOCK_SIZE-MACHINE_AXLE_TO_TAIL, 0, FAST_STRAIGHT_DEFAULT_VELOCITY, FAST_STRAIGHT_ACCERALATION, FAST_STRAIGHT_MAX_VELOCITY);
Straight TopTask::fast_straight_end(BLOCK_SIZE/2, FAST_STRAIGHT_DEFAULT_VELOCITY, 0, FAST_STRAIGHT_ACCERALATION, FAST_STRAIGHT_MAX_VELOCITY);
SlalomTurn TopTask::fast_turn_right(-PI/2, FAST_STRAIGHT_DEFAULT_VELOCITY, FAST_SLALOM_ROTATION_ACCERALATION, FAST_SLALOM_ROTATION_VELOCITY);
SlalomTurn TopTask::fast_turn_left(PI/2, FAST_STRAIGHT_DEFAULT_VELOCITY, FAST_SLALOM_ROTATION_ACCERALATION, FAST_SLALOM_ROTATION_VELOCITY);

Straight TopTask::fast_straight[16] = {
		Straight(BLOCK_SIZE, FAST_STRAIGHT_DEFAULT_VELOCITY, FAST_STRAIGHT_DEFAULT_VELOCITY, FAST_STRAIGHT_ACCERALATION, FAST_STRAIGHT_MAX_VELOCITY),
		Straight(BLOCK_SIZE*2, FAST_STRAIGHT_DEFAULT_VELOCITY, FAST_STRAIGHT_DEFAULT_VELOCITY, FAST_STRAIGHT_ACCERALATION, FAST_STRAIGHT_MAX_VELOCITY),
		Straight(BLOCK_SIZE*3, FAST_STRAIGHT_DEFAULT_VELOCITY, FAST_STRAIGHT_DEFAULT_VELOCITY, FAST_STRAIGHT_ACCERALATION, FAST_STRAIGHT_MAX_VELOCITY),
		Straight(BLOCK_SIZE*4, FAST_STRAIGHT_DEFAULT_VELOCITY, FAST_STRAIGHT_DEFAULT_VELOCITY, FAST_STRAIGHT_ACCERALATION, FAST_STRAIGHT_MAX_VELOCITY),
		Straight(BLOCK_SIZE*5, FAST_STRAIGHT_DEFAULT_VELOCITY, FAST_STRAIGHT_DEFAULT_VELOCITY, FAST_STRAIGHT_ACCERALATION, FAST_STRAIGHT_MAX_VELOCITY),
		Straight(BLOCK_SIZE*6, FAST_STRAIGHT_DEFAULT_VELOCITY, FAST_STRAIGHT_DEFAULT_VELOCITY, FAST_STRAIGHT_ACCERALATION, FAST_STRAIGHT_MAX_VELOCITY),
		Straight(BLOCK_SIZE*7, FAST_STRAIGHT_DEFAULT_VELOCITY, FAST_STRAIGHT_DEFAULT_VELOCITY, FAST_STRAIGHT_ACCERALATION, FAST_STRAIGHT_MAX_VELOCITY),
		Straight(BLOCK_SIZE*8, FAST_STRAIGHT_DEFAULT_VELOCITY, FAST_STRAIGHT_DEFAULT_VELOCITY, FAST_STRAIGHT_ACCERALATION, FAST_STRAIGHT_MAX_VELOCITY),
		Straight(BLOCK_SIZE*9, FAST_STRAIGHT_DEFAULT_VELOCITY, FAST_STRAIGHT_DEFAULT_VELOCITY, FAST_STRAIGHT_ACCERALATION, FAST_STRAIGHT_MAX_VELOCITY),
		Straight(BLOCK_SIZE*10, FAST_STRAIGHT_DEFAULT_VELOCITY, FAST_STRAIGHT_DEFAULT_VELOCITY, FAST_STRAIGHT_ACCERALATION, FAST_STRAIGHT_MAX_VELOCITY),
		Straight(BLOCK_SIZE*11, FAST_STRAIGHT_DEFAULT_VELOCITY, FAST_STRAIGHT_DEFAULT_VELOCITY, FAST_STRAIGHT_ACCERALATION, FAST_STRAIGHT_MAX_VELOCITY),
		Straight(BLOCK_SIZE*12, FAST_STRAIGHT_DEFAULT_VELOCITY, FAST_STRAIGHT_DEFAULT_VELOCITY, FAST_STRAIGHT_ACCERALATION, FAST_STRAIGHT_MAX_VELOCITY),
		Straight(BLOCK_SIZE*13, FAST_STRAIGHT_DEFAULT_VELOCITY, FAST_STRAIGHT_DEFAULT_VELOCITY, FAST_STRAIGHT_ACCERALATION, FAST_STRAIGHT_MAX_VELOCITY),
		Straight(BLOCK_SIZE*14, FAST_STRAIGHT_DEFAULT_VELOCITY, FAST_STRAIGHT_DEFAULT_VELOCITY, FAST_STRAIGHT_ACCERALATION, FAST_STRAIGHT_MAX_VELOCITY),
		Straight(BLOCK_SIZE*15, FAST_STRAIGHT_DEFAULT_VELOCITY, FAST_STRAIGHT_DEFAULT_VELOCITY, FAST_STRAIGHT_ACCERALATION, FAST_STRAIGHT_MAX_VELOCITY),
		Straight(BLOCK_SIZE*16, FAST_STRAIGHT_DEFAULT_VELOCITY, FAST_STRAIGHT_DEFAULT_VELOCITY, FAST_STRAIGHT_ACCERALATION, FAST_STRAIGHT_MAX_VELOCITY)
};


TopTask::TopTask()
:TaskBase("top", TOP_TASK_PRIORITY, TOP_TASK_STACK_SIZE),
 agent(maze),
 robot_position(0,0),
 robot_dir(NORTH)
{

}


TopTask::~TopTask()
{

}


Direction TopTask::getWallData()
{
	Direction wall;
	// 壁の絶対方向を返す
	WallDetect::WallInfo wall_info =wall_detect_task->get_wall_info();

	// 壁の状態をLEDで表示
	if (wall_info.front) Led_on(LED_2 | LED_3);
	else Led_off(LED_2 | LED_3);
	if (wall_info.left) Led_on(LED_1);
	else Led_off(LED_1);
	if (wall_info.right) Led_on(LED_4);
	else Led_off(LED_4);

	int8_t robot_dir_index = 0;
	while (1) {
		if (robot_dir.byte == NORTH << robot_dir_index) break;
		robot_dir_index++;
	}

	if (wall_info.front) {
		wall.byte |= robot_dir;
	}

	if (wall_info.right) {
		wall.byte |= NORTH << (robot_dir_index+1)%4;
	}

	if (wall_info.left) {
		if (robot_dir_index == 0) wall.byte |= WEST;
		else wall.byte |= NORTH << (robot_dir_index-1)%4;
	}

	prev_wall_cnt = wall.nWall();

	return wall;
}


IndexVec TopTask::getRobotPosion()
{
	// 絶対座標系で返す
	return robot_position;
}


void TopTask::robotMove(const Direction &dir)
{



	int8_t robot_dir_index = 0;
	while (1) {
		if (robot_dir.byte == NORTH << robot_dir_index) break;
		robot_dir_index++;
	}

	int8_t next_dir_index = 0;
	while (1) {
		if (dir.byte == NORTH << next_dir_index) break;
		next_dir_index++;
	}

	int8_t dir_diff = next_dir_index - robot_dir_index;
	// 直進
	if (dir_diff == 0) {
		if (is_start_block) {
			motion_control_task->push_motion(search_straight_start);
			is_start_block = false;
		}
		else motion_control_task->push_motion(search_straight);
	}
	// 右
	else if (dir_diff == 1 || dir_diff == -3) {
		motion_control_task->push_motion(search_turn_right);
	}
	// 左
	else if (dir_diff == -1 || dir_diff == 3) {
		motion_control_task->push_motion(search_turn_left);
	}
	// 180度ターン
	else  {
		if (prev_wall_cnt == 3) {
			motion_control_task->push_motion(search_straight_end);
			motion_control_task->wait_finish_motion();
			motion_control_task->push_motion(search_turn90);
			motion_control_task->wait_finish_motion();
			motion_control_task->push_motion(search_back);
			motion_control_task->wait_finish_motion();
			motion_control_task->push_motion(search_straight_half_start_end);
			motion_control_task->wait_finish_motion();
			motion_control_task->push_motion(search_turn90);
			motion_control_task->wait_finish_motion();
			motion_control_task->push_motion(search_back);
			motion_control_task->wait_finish_motion();
			motion_control_task->push_motion(search_straight_start);
		}
		else {
			motion_control_task->push_motion(search_straight_end);
			motion_control_task->wait_finish_motion();
			motion_control_task->push_motion(search_turn180);
			motion_control_task->wait_finish_motion();
			motion_control_task->push_motion(search_straight_half_start);
		}
	}

	robot_dir = dir;
	// robot positionをdirの分だけ動かす
	if (NORTH == dir.byte) {
		robot_position += IndexVec::vecNorth;
	}
	else if (SOUTH == dir.byte) {
		robot_position += IndexVec::vecSouth;
	}
	else if (EAST == dir.byte) {
		robot_position += IndexVec::vecEast;
	}
	else if (WEST == dir.byte) {
		robot_position += IndexVec::vecWest;
	}
}


void TopTask::task()
{
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


			while(1) {
				//センサから取得した壁情報を入れる
				Direction wallData = getWallData();
				//ロボットの座標を取得
				IndexVec robotPos = getRobotPosion();

				//壁情報を更新 次に進むべき方向を計算
				agent.update(robotPos, wallData);

				//Agentの状態を確認
				//FINISHEDになったら計測走行にうつる
				if (agent.getState() == Agent::FINISHED) break;


				////ゴールにたどり着いた瞬間に一度だけmazeのバックアップをとる
				////Mazeクラスはoperator=が定義してあるからa = bでコピーできる
				//if (prevState == Agent::SEARCHING_NOT_GOAL && agent.getState() == Agent::SEARCHING_REACHED_GOAL) {
				//	maze_backup = maze;
				//}
				//
				prevState = agent.getState();


				////一度はゴールにたどり着き、少なくともゴールできる状態で追加の探索をしているが、
				////もう時間が無いから探索をうちやめてスタート地点に戻る
				//if (isTimeOut() && agent.getState() == Agent::SEARCHING_REACHED_GOAL){
				//	agent.forceGotoStart();
				//}
				//

				//Agentの状態が探索中の場合は次に進むべき方向を取得する
				Direction nextDir = agent.getNextDirection();

				if (nextDir.byte == 0) {
					break;
				}

				//nextDirの示す方向に進む
				//突然今と180度逆の方向を示してくる場合もあるので注意
				//止まらないと壁にぶつかる
				robotMove(nextDir);  //robotMove関数はDirection型を受け取ってロボットをそっちに動かす関数


				//1区画進んで壁の状態が分かるまで待機
				//ここで待っている時に割り込みでモーターを制御したりセンサの値を処理したりすることになる
				motion_control_task->wait_finish_motion();

			}
			printf("end\n");

			motor_control_task->disable();
		}
	}
}

