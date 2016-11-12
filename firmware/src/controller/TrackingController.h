/*
 * PositionController.h
 *
 *  Created on: 2016/11/02
 *      Author: idt12312
 */

#ifndef TRACKINGCONTROLLER_H_
#define TRACKINGCONTROLLER_H_


#include "Geometry.h"
#include "Trajectory.h"
#include "PIDController.h"
#include "config.h"

class TrackingController {
private:
	PIDController pos_x_controller;
	PIDController pos_y_controller;
	PIDController angle_controller;

	Velocity track_slalom(const Position &pos, const Position &ref)
	{
		Velocity ret;

		const float cos_theta = arm_cos_f32(pos.theta);
		const float sin_theta = arm_sin_f32(pos.theta);

		const float x_e = ref.x - pos.x;
		const float y_e = ref.y - pos.y;

		const float ref_x_on_robot_coord =  cos_theta * x_e + sin_theta * y_e;
		const float ref_y_on_robot_coord = -sin_theta * x_e + cos_theta * y_e;

		ret.v = pos_y_controller.update(0, ref_y_on_robot_coord);
		ret.omega = pos_x_controller.update(0, -ref_x_on_robot_coord);

		return ret;
	}

	Velocity track_pivot(const Position &pos, const Position &ref)
	{
		Velocity ret;

		ret.omega = angle_controller.update(pos.theta, ref.theta);

		return ret;
	}

public:
	TrackingController(const PIDParam &pos_x_param, const PIDParam &pos_y_param, const PIDParam &angle_param)
		:pos_x_controller(pos_x_param), pos_y_controller(pos_y_param), angle_controller(angle_param)
	{}

	Velocity update(const Position &pos, const TrajectoryTarget &ref)
	{
		if (ref.type == TrajectoryTarget::Type::SLALOM || ref.type == TrajectoryTarget::Type::STRAIGHT) {
			// 速度のフィードフォワード + 位置のフィードバック
			return Velocity(ref.v.v, ref.v.omega*TRACKING_CONTROL_OMEGA_FORWARD_GAIN) + track_slalom(pos, ref.pos);
		}
		else if (ref.type == TrajectoryTarget::Type::PIVOT) {
			// 速度のフィードフォワード + 位置のフィードバック
			return ref.v + track_pivot(pos, ref.pos);
		}
		else {
			return Velocity();
		}
	}

	void reset()
	{
		pos_x_controller.reset();
		pos_y_controller.reset();
		angle_controller.reset();
	}
};


#endif /* TRACKINGCONTROLLER_H_ */
