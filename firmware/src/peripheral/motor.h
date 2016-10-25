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

void Motor_init();
void Motor_set(const float _duty[2]);

#ifdef	__cplusplus
}
#endif

#endif /* MOTOR_H_ */
