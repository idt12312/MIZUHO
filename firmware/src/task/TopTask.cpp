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
Straight TopTask::fast_straight_end(BLOCK_SIZE/2+MACHINE_AXLE_TO_TAIL*2, FAST_STRAIGHT_DEFAULT_VELOCITY, 0, FAST_STRAIGHT_ACCERALATION, FAST_STRAIGHT_MAX_VELOCITY);
SlalomTurn TopTask::fast_turn_right(-PI/2, FAST_STRAIGHT_DEFAULT_VELOCITY, FAST_SLALOM_ROTATION_ACCERALATION, FAST_SLALOM_ROTATION_VELOCITY);
SlalomTurn TopTask::fast_turn_left(PI/2, FAST_STRAIGHT_DEFAULT_VELOCITY, FAST_SLALOM_ROTATION_ACCERALATION, FAST_SLALOM_ROTATION_VELOCITY);


Straight TopTask::test_straight(BLOCK_SIZE*6, 0, 0, SEARCH_STRAIGHT_ACCERALATION, SEARCH_STRAIGHT_MAX_VELOCITY);

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

		motion_control_task->wait_finish_motion();
	}
	// 右
	else if (dir_diff == 1 || dir_diff == -3) {
		motion_control_task->push_motion(search_turn_right);
		motion_control_task->wait_finish_motion();
	}
	// 左
	else if (dir_diff == -1 || dir_diff == 3) {
		motion_control_task->push_motion(search_turn_left);
		motion_control_task->wait_finish_motion();
	}
	// 180度ターン
	else  {
		if (prev_wall_cnt == 3) {
			motion_control_task->push_motion(search_straight_end);
			motion_control_task->wait_finish_motion();
			motion_control_task->push_motion(search_turn180);
			motion_control_task->wait_finish_motion();
			motion_control_task->push_motion(search_back);
			motion_control_task->wait_finish_motion();
			motion_control_task->push_motion(search_straight_start);
			motion_control_task->wait_finish_motion();
		}
		else {
			motion_control_task->push_motion(search_straight_end);
			motion_control_task->wait_finish_motion();
			motion_control_task->push_motion(search_turn180);
			motion_control_task->wait_finish_motion();
			motion_control_task->push_motion(search_straight_half_start);
			motion_control_task->wait_finish_motion();
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


void TopTask::robotMove(const Operation &op)
{
	switch (op.op) {
	case Operation::FORWARD:
		if (is_start_block) {
			is_start_block = false;
			motion_control_task->push_motion(fast_straight_start);
			motion_control_task->wait_finish_motion();
			if (op.n == 1) break;
			motion_control_task->push_motion(fast_straight[op.n-2]);
			motion_control_task->wait_finish_motion();
		}
		motion_control_task->push_motion(fast_straight[op.n-1]);
		motion_control_task->wait_finish_motion();
		break;
	case Operation::TURN_LEFT90:
		motion_control_task->push_motion(fast_turn_left);
		motion_control_task->wait_finish_motion();
		break;
	case Operation::TURN_RIGHT90:
		motion_control_task->push_motion(fast_turn_right);
		motion_control_task->wait_finish_motion();
		break;
	}
}


void TopTask::task()
{
	bool is_maze_valid = false;
	bool is_maze_complete = false;

	wall_detect_task->calib_offset();

	while (1) {
		blink_led_task->set_time(50,450);
		motor_control_task->disable();

		while (1) {
			vTaskDelay(100);
			if (ButtonL_get()) break;
		}
		blink_led_task->set_time(300,300);
		while (1) {
			vTaskDelay(50);
			WallDetect::WallInfo wall_info =wall_detect_task->get_wall_info();
			if (wall_info.front) break;
		}
		if (!is_maze_valid) {
			blink_led_task->set_time(50,50);

			vTaskDelay(2000);
			MPU6500_calib_offset();
			wall_detect_task->calib_offset();
			vTaskDelay(1000);

			motion_control_task->reset();

			blink_led_task->set_time(100, 1900);
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
				if (agent.getState() == Agent::FINISHED) {
					motion_control_task->push_motion(search_straight_end);
					motion_control_task->wait_finish_motion();
					motion_control_task->push_motion(search_turn180);
					motion_control_task->wait_finish_motion();
					motor_control_task->disable();
					is_start_block = true;
					is_maze_complete = true;
					break;
				}

				//ゴールにたどり着いた瞬間に一度だけmazeのバックアップをとる
				//Mazeクラスはoperator=が定義してあるからa = bでコピーできる
				if (prevState == Agent::SEARCHING_NOT_GOAL && agent.getState() == Agent::SEARCHING_REACHED_GOAL) {
					maze_backup = maze;
					is_maze_valid = true;
					blink_led_task->set_time(100, 900);
				}

				prevState = agent.getState();


				//Agentの状態が探索中の場合は次に進むべき方向を取得する
				Direction nextDir = agent.getNextDirection();
				if (nextDir.byte == 0) {
					break;
				}

				//nextDirの示す方向に進む
				//突然今と180度逆の方向を示してくる場合もあるので注意
				//止まらないと壁にぶつかる
				robotMove(nextDir);  //robotMove関数はDirection型を受け取ってロボットをそっちに動かす関数
			}
		}
		else {
			blink_led_task->set_time(50,50);

			vTaskDelay(2000);
			MPU6500_calib_offset();
			wall_detect_task->calib_offset();
			vTaskDelay(1000);

			// リセット
			motor_control_task->disable();
			motion_control_task->reset();


			// 迷路情報が不完全の場合はバックアップから復元する
			if (!is_maze_complete) {
				agent.resumeAt(Agent::FINISHED, maze_backup);
			}


			//最短経路の計算 割と時間がかかる(数秒)
			//引数は斜め走行をするかしないか
			//trueだと斜め走行をする
			agent.caclRunSequence(false);


			blink_led_task->set_time(100, 1900);
			motor_control_task->enable();

			/**********************************
			 * 計測走行
			 *********************************/
			//コマンドリストみたいなやつを取り出す
			const OperationList &runSequence = agent.getRunSequence();

			//Operationを先頭から順番に実行していく
			for (size_t i=0;i<runSequence.size();i++) {
				//i番目のを実行
				robotMove(runSequence[i]); //robotMode関数はOperation型を受け取ってそれを実行する関数
			}

			motion_control_task->push_motion(fast_straight_end);
			motion_control_task->wait_finish_motion();
		}
	}
}

