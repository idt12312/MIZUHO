/*
 * Trapezoid.h
 *
 *  Created on: 2016/11/04
 *      Author: idt12312
 */

#ifndef CONTROLLER_TRAPEZOID_H_
#define CONTROLLER_TRAPEZOID_H_

#include <algorithm>
#include <cmath>
#include <cstddef>

// 加速度(絶対値)
// 最大速度
// 始点　終点
class Trapezoid {
private:
	const float dt;
	const float ax;
	const float v_max;
	const float v_start;
	const float x_start;
	const float x_end;
	const float x_mid;
	float dir;

	float v;
	float x;
	int32_t cnt;

	bool end;
	bool is_first;

	inline float sign(float a)
	{
		return a >= 0? 1 : -1;
	}

	float saturate(float a, float a_max)
	{
		const float a_sat_abs = std::min(std::abs(a), std::abs(a_max));
		return a_sat_abs * sign(a);
	}

public:
	Trapezoid(float _dt, float _ax, float _v_max, float _v_start, float _x_start, float _x_end)
	:dt(_dt), ax(std::abs(_ax)), v_max(std::abs(_v_max)), v_start(_v_start), x_start(_x_start), x_end(_x_end), x_mid((x_start+x_end)/2)
	{
		dir = sign(x_end-x_start);
		reset();
	}

	virtual ~Trapezoid(){}

	float next()
	{
		if (cnt == 0 && !is_first) {
			end = true;
			return v;
		}
		is_first = false;

	    if (std::abs(x) < std::abs(x_mid)) {
	        v = v+dir*ax*dt;
	        cnt++;
	    }
	    else {
	        v = v-dir*ax*dt;
	        cnt--;
	    }

	    const float v_sat = saturate(v, v_max);
	    x += v_sat*dt;

	    return v_sat;
	}

	void reset()
	{
		x = x_start;
		v = v_start;
		cnt = 0;
		end = false;
		is_first = true;
	}

	inline bool is_end() const { return end; }
};


#endif /* CONTROLLER_TRAPEZOID_H_ */
