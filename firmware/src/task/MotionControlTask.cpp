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
		Position current_pos = odometry.get_pos();
		odometry.reset();

		// odometryをリセットする
		// 直前の目標点と現在の座標のずれを計算し、odometryを補正する
		Position pos_err = current_pos;
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
		if (motion->get_adjust_odometry_flag()) {
			pos_err.y = 0.0f;
			pos_err.theta = 0.0f;
		}

		odometry.set_pos(pos_err);
		motion->reset(odometry.get_pos());
		tracking_controller.reset();
	}
	else {
		motion = nullptr;
	}
}


void MotionControlTask::wall_trace_control()
{
	const WallDetect::WallInfo wall_info = wall_detect_task->get_wall_info();
	if (wall_info.left && wall_info.right) {
		// 右によっている　右のセンサの値が大きくなる rl_diffが大きくなる
		// pos.xを大きくする 原点が左に動いたことになる 機体は左に行こうとする
		// rl_diffは+-3500のオーダ　中心付近では多分+-1000くらいで変動
		Position current_pos = odometry.get_pos();
		current_pos.x += TRACKING_CONTROL_WALL_P_GAIN * wall_info.rl_diff;
		odometry.set_pos(current_pos);
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

		// 壁トレース制御
		// 壁情報を使ってodometoryのx座標を修正する
		// 補正は直進時のみかける
		if (target.type == TrackingTarget::Type::STRAIGHT) {
			wall_trace_control();
		}

		// 目標点に追従をする制御
		const Velocity motor_ref = tracking_controller.update(odometry.get_pos(), target);
		motor_control_task->set_reference(motor_ref);

		// 1つのmotionが終わったことを通知する
		if (motion->is_end()) {
			xSemaphoreGive(motion_end_semaphore);
			last_goal_pos = motion->get_gial_pos();
			motion = nullptr;
		}
	}
}