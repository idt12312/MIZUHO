/*
 * PIDController.h
 *
 *  Created on: 2016/10/28
 *      Author: idt12312
 */

#ifndef PIDCONTROLLER_H_
#define PIDCONTROLLER_H_

#include <cfloat>

struct PIDParam {
	float T = 0.001f;
	float Kp = 0.0f;
	float Ki = 0.0f;
	float Kd = 0.0f;
	float Kf = 0.0f;
	float saturation = FLT_MAX;
};

class PIDController {
private:
	// 設定パラメータ
	PIDParam param;

	// 内部状態
	float reference = 0.0f;
	float error_sum = 0.0f;
	float prev_error = 0.0f;

public:
	PIDController() {}
	PIDController(const PIDParam& _param)
	:param(_param) {}

	virtual ~PIDController() {}

	float update(float measured)
	{
		const float error = reference - measured;
		const float error_diff = (error - prev_error)/param.T;
		error_sum = error_sum + error*param.T;

		// 偏差の積分は飽和させる
		if (error_sum > param.saturation) error_sum = param.saturation;
		else if (error_sum < -param.saturation) error_sum = param.saturation;

		const float control_input = param.Kp*error
				+ param.Kd*error_diff
				+ param.Ki*error_sum
				+ param.Kf*reference;

		prev_error = error;

		return control_input;
	}

	float update(float measured, float _reference)
	{
		reference = _reference;
		return update(measured);
	}

	void reset()
	{
		reference = 0.0f;
		error_sum = 0.0f;
		prev_error = 0.0f;
	}

	void set_ref(float ref) { reference = ref; }
	void set_param(const PIDParam& _param) { param = _param; }

	inline float get_ref() { return reference; }
	inline const PIDParam &get_param() { return param; }
};

#endif /* PIDCONTROLLER_H_ */