/*
 * MotionControlTask.cpp
 *
 *  Created on: 2016/11/14
 *      Author: idt12312
 */


#include "MotionControlTask.h"
#include "config.h"
#include "tasks.h"


MotionControlTask::MotionControlTask()
:TaskBase("motion ctrl", TRACKING_CONTROL_TASK_PRIORITY, TRACKING_CONTROL_TASK_STACK_SIZE),
 odometry(MOTOR_CONTROL_TASK_PERIOD_SEC)
{
	motion_queue = xQueueCreate(10, sizeof(Motion*));
	pos_queue = xQueueCreate(1, sizeof(Position));
	motion_end_semaphore = xSemaphoreCreateBinary();
}


MotionControlTask::~MotionControlTask()
{
	vQueueDelete(motion_queue);
	vQueueDelete(pos_queue);
	vSemaphoreDelete(motion_end_semaphore);
}


void MotionControlTask::reset()
{
	odometry.reset();
	tracking_controller.reset();

	motion = nullptr;
	last_goal_pos = Position(0,0,0);

	is_wall_detect_point_notified = true;
	is_first = true;
	req_reset_odometry = true;
}


Position MotionControlTask::get_pos()
{
	Position pos;
	xQueuePeek(pos_queue, &pos, 0);
	return pos;
}


void MotionControlTask::push_motion(Motion &motion)
{
	Motion *motion_ptr = &motion;
	xQueueSend(motion_queue, &motion_ptr, 0);
}


void MotionControlTask::wait_finish_motion()
{
	// ブロックする前にセマフォをクリアしておく(念のため)
	xQueueReset(motion_end_semaphore);
	xSemaphoreTake(motion_end_semaphore, portMAX_DELAY);
}


void MotionControlTask::update_odometry()
{
	// motor controllerから送られてきたマシンの速度情報を受け取り、
	// odometryを更新する
	Velocity measured_velocity;
	while (motor_control_task->get_measured_velocity(&measured_velocity)) {
		odometry.update(measured_velocity);
	}
	xQueueOverwrite(pos_queue, &odometry.get_pos());
}


void MotionControlTask::update_motion()
{
	// 新しいMotionをセット
	if (xQueueReceive(motion_queue, &motion, 0) == pdTRUE) {
		is_wall_detect_point_notified = false;
		Position current_pos = odometry.get_pos();
		odometry.reset();

		// odometryをリセットする
		// 直前の目標点と現在の座標のずれを計算し、odometryを補正する
		Position pos_err;
		// 直進
		if (-PI/4 < current_pos.theta && current_pos.theta < PI/4) {
			pos_err.x = current_pos.x;
			pos_err.y = current_pos.y - last_goal_pos.y;
			pos_err.theta = current_pos.theta;
		}
		// 右折
		else if (-PI*3/4 < current_pos.theta && current_pos.theta < -PI/4) {
			pos_err.y = current_pos.x - last_goal_pos.x;
			pos_err.x = -(current_pos.y - last_goal_pos.y);
			pos_err.theta = current_pos.theta - last_goal_pos.theta;
		}
		// 左折
		else if (PI/4 < current_pos.theta && current_pos.theta < PI*3/4) {
			pos_err.y = -(current_pos.x - last_goal_pos.x);
			pos_err.x = current_pos.y - last_goal_pos.y;
			pos_err.theta = current_pos.theta - last_goal_pos.theta;
		}
		// 180度旋回
		else {
			pos_err.x = -(current_pos.x - last_goal_pos.x);
			pos_err.y = -(current_pos.y - last_goal_pos.y);
			pos_err.theta = current_pos.theta - last_goal_pos.theta;
		}

		// 壁に当てて補正する時に使う
		if (req_reset_odometry) {
			req_reset_odometry = false;
			pos_err.y = 0.0f;
			pos_err.theta = 0.0f;
		}

		if (is_first) {
			pos_err = Position(0,0,0);
			is_first = false;
		}

		odometry.set_pos(pos_err);
		motion->reset(odometry.get_pos());
		tracking_controller.reset();
		wall_filtered = 0;
	}
	else {
		motion = nullptr;
	}
}


void MotionControlTask::task()
{
	TickType_t last_wake_tick = xTaskGetTickCount();
	while (1) {
		vTaskDelayUntil(&last_wake_tick, TRACKING_CONTROL_TASK_PERIOD);
		last_wake_tick = xTaskGetTickCount();

		// オドメトリの更新
		update_odometry();

		// motionが終了していたら新たなmotionをqueueから取り出す
		if (motion == nullptr) {
			update_motion();
		}


		// motionが空(queueが空)の時はその場にとどまる
		if (motion == nullptr) {
			motor_control_task->set_reference(Velocity(0,0));
			continue;
		}

		// 次の目標点をセット
		const TrackingTarget target = motion->next();


		// 直進時の横壁壁補正のための壁情報のフィルタリング
		// 機体が右によっている or 右方向を向いているほどwall_filteredの値は大きくなる
		if (target.type == TrackingTarget::Type::STRAIGHT) {
			const WallDetect::WallInfo wall_info = wall_detect_task->get_wall_info();
			if (wall_info.left && wall_info.right) {
				wall_filtered = (1.0f-TRACKING_CONTROL_WALL_FILTER_GAIN)*wall_filtered + TRACKING_CONTROL_WALL_FILTER_GAIN * (wall_info.right_value - wall_info.left_value);
			}/*
			else if (wall_info.left) {
				wall_filtered = (1.0f-TRACKING_CONTROL_WALL_FILTER_GAIN)*wall_filtered + TRACKING_CONTROL_WALL_FILTER_GAIN * ( - wall_info.left_value);
			}
			else if (wall_info.right) {
				wall_filtered = (1.0f-TRACKING_CONTROL_WALL_FILTER_GAIN)*wall_filtered + TRACKING_CONTROL_WALL_FILTER_GAIN * wall_info.right_value;
			}*/
			else {
				wall_filtered = 0;
			}
		}
		else {
			wall_filtered = 0;
		}


		// 壁情報を使ってオドメトリを修正
		Position current_pos = odometry.get_pos();
		current_pos.x += TRACKING_CONTROL_WALL_TO_X_GAIN*wall_filtered;
		odometry.set_pos(current_pos);

		// 目標点に追従をする制御
		Velocity motor_ref = tracking_controller.update(odometry.get_pos(), target);

		// 壁情報から目標角速度を修正(フィードフォワード入力)
		motor_ref.omega += TRACKING_CONTROL_WALL_TO_OMEGA_GAIN*wall_filtered;


		motor_control_task->set_reference(motor_ref);


		// 次の壁を読む地点に到達したことを通知
		const Position goal_pos = motion->get_gial_pos();
		if (!is_wall_detect_point_notified) {
			if (target.type == TrackingTarget::Type::STRAIGHT) {
				const float to_goal_dist = std::abs(goal_pos.y) - std::abs(odometry.get_pos().y);
				if (to_goal_dist < WALL_DETECT_POINT) {
					xSemaphoreGive(motion_end_semaphore);
					is_wall_detect_point_notified = true;
				}
			}
			else if (target.type == TrackingTarget::Type::SLALOM) {
				const float to_goal_dist = std::abs(goal_pos.x) - std::abs(odometry.get_pos().x);
				if (to_goal_dist < WALL_DETECT_POINT) {
					xSemaphoreGive(motion_end_semaphore);
					is_wall_detect_point_notified = true;
				}
			}
			else {
				if (motion->is_end()) {
					xSemaphoreGive(motion_end_semaphore);
					is_wall_detect_point_notified = true;
				}
			}
		}

		// 1つのmotionが終わったことを通知する
		if (motion->is_end()) {
			last_goal_pos = goal_pos;
			req_reset_odometry = motion->is_req_reset_odometry();
			motion = nullptr;
		}
	}
}
