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
public:
	PIDController() {}
	PIDController(const PIDParam& _param)
	:param(_param) {}

	virtual ~PIDController() {}

	float update(float measured, float reference)
	{
		const float error = reference - measured;
		float error_diff = (error - prev_error)/param.T;
		if (is_first) {
			is_first = false;
			error_diff = 0.0f;
		}
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

	void reset()
	{
		error_sum = 0.0f;
		prev_error = 0.0f;
		is_first = true;
	}

	void set_param(const PIDParam& _param) { param = _param; }
	inline const PIDParam &get_param() { return param; }


private:
	// 設定パラメータ
	PIDParam param;

	// 内部状態
	float error_sum = 0.0f;
	float prev_error = 0.0f;

	bool is_first = true;
};

#endif /* PIDCONTROLLER_H_ */
