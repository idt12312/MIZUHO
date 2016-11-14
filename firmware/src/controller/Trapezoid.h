/*
 * Trapezoid.h
 *
 *  Created on: 2016/11/04
 *      Author: idt12312
 */

#ifndef CONTROLLER_TRAPEZOID_H_
#define CONTROLLER_TRAPEZOID_H_


#include <cstddef>


// 加速度(絶対値)
// 最大速度
// 始点　終点
class Trapezoid {
public:
	Trapezoid(float _dt, float L_, float _ax, float _v_max, float _v_start, float _v_end);
	virtual ~Trapezoid();

	float next();
	void reset();
	inline bool is_end() const { return end; }

private:
	const float dt;
	const float L;
	const float dir;
	float v_max;
	const float v_start;
	const float v_end;
	float ax;

	size_t N1;
	size_t N2;
	size_t N3;
	size_t cnt;

	bool end;

	inline float sign(float a) const { return a >= 0? 1 : -1; }
};


#endif /* CONTROLLER_TRAPEZOID_H_ */
