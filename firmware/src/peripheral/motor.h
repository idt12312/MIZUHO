/*
 * motor.h
 *
 *  Created on: 2016/09/29
 *      Author: idt12312
 */

#ifndef MOTOR_H_
#define MOTOR_H_

#ifdef	__cplusplus
extern "C"
{
#endif

typedef struct {
	float left;
	float right;
} MotorVoltage;

void Motor_init();
void Motor_set_duty(const float _duty[2]);
void Motor_set_voltage(const MotorVoltage *volt);

#ifdef	__cplusplus
}
#endif

#endif /* MOTOR_H_ */
