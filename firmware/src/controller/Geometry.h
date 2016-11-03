/*
 * Geometry.h
 *
 *  Created on: 2016/11/03
 *      Author: idt12312
 */

#ifndef CONTROLLER_GEOMETRY_H_
#define CONTROLLER_GEOMETRY_H_

#include "arm_math.h"
#include <cmath>


struct Velocity {
	float v;
	float omega;

	Velocity(float _v=0.0f, float _omega=0.0f)
		:v(_v), omega(_omega){}
};


struct Position {
	float x;
	float y;
	float theta;

	Position(float _x=0.0f, float _y=0.0f, float _theta=PI/2)
		:x(_x), y(_y), theta(_theta) {}

	static float distance(const Position &pos1, const Position &pos2)
	{
		float dist;
		const float dx = pos2.x - pos1.x;
		const float dy = pos2.y - pos1.y;
		arm_sqrt_f32(dx*dx + dy*dy, &dist);

		return dist;
	}

	static float look_at(const Position &from, const Position &at)
	{
		const float dx = at.x - from.x;
		const float dy = at.y - from.y;
		return std::atan2(dy, dx);
	}
};




#endif /* CONTROLLER_GEOMETRY_H_ */
