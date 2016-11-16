/*
 * MotorControlTask.cpp
 *
 *  Created on: 2016/11/14
 *      Author: idt12312
 */

#include "MotorControlTask.h"
#include "Geometry.h"
#include "motor.h"
#include "enc.h"
#include "mpu6500.h"
#include "config.h"


MotorControlTask::MotorControlTask()
:TaskBase("motor ctrl", MOTOR_CONTROL_TASK_PRIORITY, MOTOR_CONTROL_TASK_STACK_SIZE)
{
	measured_velocity_queue = xQueueCreate(10, sizeof(Velocity));
	reference_velocity_queue = xQueueCreate(1, sizeof(Velocity));
}


MotorControlTask::~MotorControlTask()
{
	vQueueDelete(reference_velocity_queue);
	vQueueDelete(measured_velocity_queue);
}


void MotorControlTask::set_reference(const Velocity &ref)
{
	xQueueOverwrite(reference_velocity_queue, &ref);
}


bool MotorControlTask::get_measured_velocity(Velocity *ref)
{
	return xQueueReceive(measured_velocity_queue, ref, 0) == pdTRUE;
}


void MotorControlTask::enable()
{
	control_enable = true;
	control_reset_req = true;
}


void MotorControlTask::disable()
{
	control_enable = false;
}


void MotorControlTask::task()
{
	TickType_t last_wake_tick = xTaskGetTickCount();
	while (1) {
		vTaskDelayUntil(&last_wake_tick, MOTOR_CONTROL_TASK_PERIOD);
		last_wake_tick = xTaskGetTickCount();

		const float gyro = MPU6500_read_gyro_z();

		EncValue enc_value;
		Enc_read(&enc_value);

		Velocity ref;
		if (xQueuePeek(reference_velocity_queue, &ref, MOTOR_CONTROL_TASK_PERIOD) == pdFALSE) continue;


		if (control_reset_req) {
			motor_controller.reset();
			control_reset_req = false;
		}

		MotorVoltage motor_voltage = motor_controller.update(ref, enc_value, gyro);
		if (!control_enable) {
			motor_voltage.left = motor_voltage.right = 0.0f;
		}
		Motor_set_voltage(&motor_voltage);

		Velocity measured_velocity = motor_controller.get_measured_velocity();
#if ODOMETRY_USE_GYRO==1
		measured_velocity.omega = gyro;
#endif
		xQueueSend(measured_velocity_queue, &measured_velocity, 0);
	}
}
