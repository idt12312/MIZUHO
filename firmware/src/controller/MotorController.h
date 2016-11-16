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


// エンコーダの値を受け取り、モータへの出力電圧を計算する
// 現在の速度も返す
class MotorController {
public:
	MotorController();
	virtual ~MotorController();

	MotorVoltage update(const Velocity &ref, const EncValue& enc_value, float gyro_z);
	inline const Velocity& get_measured_velocity() const { return v_measured; }
	void reset();


private:
	PIDController left_motor_controller;
	PIDController right_motor_controller;
	const float T;
	Velocity v_measured;
};


#endif /* MOTORCONTROLLER_H_ */
