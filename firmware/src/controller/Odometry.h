/*
 * Odometry.h
 *
 *  Created on: 2016/10/31
 *      Author: idt12312
 */

#ifndef ODOMETRY_H_
#define ODOMETRY_H_

#include "arm_math.h"
#include <cmath>
#include "Geometry.h"

class Odometry {
public:
	Odometry(float _T) :T(_T) {}
	virtual ~Odometry() {}

	void update(const Velocity &v)
	{
		const float dx =  -arm_sin_f32(pos.theta) * v.v;
		const float dy = arm_cos_f32(pos.theta) * v.v;

		if (is_first) {
			prev_dx = dx;
			prev_dy = dy;
			prev_omega = v.omega;
			is_first = false;
		}

		// 台形積分
		pos.x += (dx+prev_dx)/2 * T;
		pos.y += (dy+prev_dy)/2 * T;
		pos.theta += (v.omega+prev_omega)/2 * T;

		prev_dx = dx;
		prev_dy = dy;
		prev_omega = v.omega;
	}

	void reset()
	{
		pos.x = pos.y = pos.theta = 0.0f;
		prev_dx = prev_dy = prev_omega = 0.0f;
		is_first = true;
	}

	void set_pos(const Position& _pos) { pos = _pos; }
	inline const Position& get_pos() const { return pos;}


private:
	const float T;
	Position pos;

	float prev_dx = 0.0f;
	float prev_dy = 0.0f;
	float prev_omega = 0.0f;
	bool is_first = true;
};

#endif /* ODOMETRY_H_ */
