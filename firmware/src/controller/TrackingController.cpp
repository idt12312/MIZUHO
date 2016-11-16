/*
 * TrackingController.cpp
 *
 *  Created on: 2016/11/14
 *      Author: idt12312
 */


#include "TrackingController.h"
#include "config.h"


TrackingController::TrackingController()
{
	PIDParam pos_x_controller_param;
	pos_x_controller_param.T  = TRACKING_CONTROL_TASK_PERIOD_SEC;
	pos_x_controller_param.Kp = TRACKING_CONTROL_X_P_GAIN;
	pos_x_controller_param.Kd = TRACKING_CONTROL_X_D_GAIN;
	pos_x_controller.set_param(pos_x_controller_param);

	PIDParam pos_y_controller_param;
	pos_y_controller_param.T  = TRACKING_CONTROL_TASK_PERIOD_SEC;
	pos_y_controller_param.Kp = TRACKING_CONTROL_Y_P_GAIN;
	pos_y_controller_param.Kd = TRACKING_CONTROL_Y_D_GAIN;
	pos_y_controller.set_param(pos_y_controller_param);

	PIDParam angle_controller_param;
	angle_controller_param.T  = TRACKING_CONTROL_TASK_PERIOD_SEC;
	angle_controller_param.Kp = TRACKING_CONTROL_ANGLE_P_GAIN;
	angle_controller_param.Ki = TRACKING_CONTROL_ANGLE_I_GAIN;
	angle_controller_param.Kd = TRACKING_CONTROL_ANGLE_D_GAIN;
	angle_controller.set_param(angle_controller_param);
}


TrackingController::~TrackingController()
{

}


Velocity TrackingController::track_slalom(const Position &pos, const Position &ref)
{
	Velocity ret;

	const float cos_theta = arm_cos_f32(pos.theta);
	const float sin_theta = arm_sin_f32(pos.theta);

	const float x_e = ref.x - pos.x;
	const float y_e = ref.y - pos.y;

	const float ref_x_on_robot_coord =  cos_theta * x_e + sin_theta * y_e;
	const float ref_y_on_robot_coord = -sin_theta * x_e + cos_theta * y_e;

	ret.v = pos_y_controller.update(0, ref_y_on_robot_coord);
	if (ref.y >= 0) {
		ret.omega = pos_x_controller.update(0, -ref_x_on_robot_coord);
	}
	else {
		ret.omega = pos_x_controller.update(0, ref_x_on_robot_coord);
	}

	return ret;
}


Velocity TrackingController::track_pivot(const Position &pos, const Position &ref)
{
	Velocity ret;

	ret.omega = angle_controller.update(pos.theta, ref.theta);

	return ret;
}


Velocity TrackingController::update(const Position &pos, const TrackingTarget &ref)
{
	if (ref.type == TrackingTarget::Type::SLALOM || ref.type == TrackingTarget::Type::STRAIGHT) {
		// 速度のフィードフォワード + 位置のフィードバック
		return Velocity(ref.v.v, ref.v.omega*TRACKING_CONTROL_OMEGA_FORWARD_GAIN) + track_slalom(pos, ref.pos);
	}
	else if (ref.type == TrackingTarget::Type::PIVOT) {
		// 速度のフィードフォワード + 位置のフィードバック
		return Velocity(ref.v.v, ref.v.omega*TRACKING_CONTROL_OMEGA_FORWARD_GAIN) + track_pivot(pos, ref.pos);
	}
	else {
		return Velocity();
	}
}


void TrackingController::reset()
{
	pos_x_controller.reset();
	pos_y_controller.reset();
	angle_controller.reset();
}
