/*
 * mpu6500.h
 *
 *  Created on: 2016/10/02
 *      Author: idt12312
 */

#ifndef MPU6500_H_
#define MPU6500_H_

#ifdef	__cplusplus
extern "C"
{
#endif

void MPU6500_init();

// Z軸周りの角速度を[rad/s]で返す
float MPU6500_read_gyro_z();

#ifdef	__cplusplus
}
#endif

#endif /* MPU6500_H_ */
