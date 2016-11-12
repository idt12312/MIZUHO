/*
 * MotorController.h
 *
 *  Created on: 2016/11/02
 *      Author: idt12312
 */

#ifndef MOTORCONTROLLER_H_
#define MOTORCONTROLLER_H_

#include "PIDController.h"
#include "Geometry.h"
#include "enc.h"
#include "motor.h"
#include "config.h"
#include "arm_math.h"

// エンコーダの値を受け取り、モータへの出力電圧を計算する
// 現在の速度も返す
class MotorController {
private:
	PIDController left_motor_controller;
	PIDController right_motor_controller;
	const float T;
	Velocity v_measured;

public:
	MotorController(const PIDParam& param)
		:left_motor_controller(param), right_motor_controller(param), T(param.T) {}

	MotorVoltage update(const Velocity &ref, const EncValue& enc_value, float gyro_z)
	{
		const float enc_left_omega = (float)enc_value.left / ENC_CNT_PER_ROT * 2*PI / T;
		const float enc_right_omega = (float)enc_value.right / ENC_CNT_PER_ROT * 2*PI / T;

		v_measured.v = (enc_left_omega + enc_right_omega) / 2 * WHEEL_RADIUS;
		v_measured.omega = (-enc_left_omega + enc_right_omega) / 2 * WHEEL_RADIUS / TREAD_WIDTH;

		// 角速度はジャイロから得たものを利用し、もう一度左右のタイヤの角速度を計算
		const float left_omega = (v_measured.v - v_measured.omega*TREAD_WIDTH) / WHEEL_RADIUS;
		const float right_omega = (v_measured.v + v_measured.omega*TREAD_WIDTH) / WHEEL_RADIUS;

		const float ref_left_omega = (ref.v - ref.omega*TREAD_WIDTH) / WHEEL_RADIUS;
		const float ref_right_omega = (ref.v + ref.omega*TREAD_WIDTH) / WHEEL_RADIUS;

		MotorVoltage motor_voltage;
		motor_voltage.left = left_motor_controller.update(left_omega, ref_left_omega);
		motor_voltage.right = right_motor_controller.update(right_omega, ref_right_omega);

		return motor_voltage;
	}

	inline const Velocity& get_measured_velocity() const { return v_measured; }

	void reset()
	{
		left_motor_controller.reset();
		right_motor_controller.reset();
	}
};


#endif /* MOTORCONTROLLER_H_ */
