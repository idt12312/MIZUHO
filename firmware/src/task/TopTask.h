/*
 * TopTask.h
 *
 *  Created on: 2016/11/15
 *      Author: idt12312
 */

#ifndef TASK_TOPTASK_H_
#define TASK_TOPTASK_H_

#include "TaskBase.h"
#include "MotionControlTask.h"
#include "MotorControlTask.h"
#include "WallDetectTask.h"
#include "Maze.h"
#include "Agent.h"
#include "mazeData.h"

class TopTask: public TaskBase {
public:
	TopTask();
	virtual ~TopTask();

	void set_motor_control_task(MotorControlTask *p) { motor_control_task = p; }
	void set_motion_control_task(MotionControlTask *p) { motion_control_task = p; }
	void set_wall_detect_task(WallDetectTask *p) { wall_detect_task = p; }


private:
	MotorControlTask *motor_control_task;
	MotionControlTask *motion_control_task;
	WallDetectTask *wall_detect_task;

	//探索した迷路の壁情報がはいる
	Maze maze;
	//クラッシュした時のためのバックアップ
	//Maze maze_backup;


	//探索の指示を出す
	Agent agent;
	//前回のAgentの状態を保存しとく
	Agent::State prevState = Agent::State::IDLE;

	IndexVec robot_position;
	Direction robot_dir;


	static Straight search_straight_start;
	static Straight search_straight_half_start;
	static Straight search_straight_half_start_end;
	static Straight search_straight_end;
	static Straight search_straight;
	static Straight search_back;
	static PivotTurn search_turn180;
	static PivotTurn search_turn90;
	static SlalomTurn search_turn_right;
	static SlalomTurn search_turn_left;

	static Straight fast_straight_start;
	static Straight fast_straight_end;
	static SlalomTurn fast_turn_right;
	static SlalomTurn fast_turn_left;
	static Straight fast_straight[16];

	bool is_start_block = true;
	int prev_wall_cnt = 0;

	virtual void task();

	Direction getWallData();
	IndexVec getRobotPosion();
	void robotMove(const Direction &dir);
};

#endif /* TASK_TOPTASK_H_ */
