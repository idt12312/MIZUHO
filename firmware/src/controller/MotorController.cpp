/*
 * MotorController.cpp
 *
 *  Created on: 2016/11/14
 *      Author: idt12312
 */


#include "MotorController.h"
#include "config.h"
#include "arm_math.h"


MotorController::MotorController()
:T(MOTOR_CONTROL_TASK_PERIOD_SEC)
{
	PIDParam pid_param;
	pid_param.T = MOTOR_CONTROL_TASK_PERIOD_SEC;
	pid_param.Kp = MOTOR_CONTROL_P_GAIN;
	pid_param.Ki = MOTOR_CONTROL_I_GAIN;
	pid_param.Kd = MOTOR_CONTROL_D_GAIN;

	left_motor_controller.set_param(pid_param);
	right_motor_controller.set_param(pid_param);
}


MotorController::~MotorController()
{

}


MotorVoltage MotorController::update(const Velocity &ref, const EncValue& enc_value, float gyro_z)
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


void MotorController::reset()
{
	left_motor_controller.reset();
	right_motor_controller.reset();
}
