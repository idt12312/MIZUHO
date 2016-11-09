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

	Velocity operator+(const Velocity lhs) const
	{
		return Velocity(this->v + lhs.v, this->omega + lhs.omega);
	}

	Velocity operator-(const Velocity lhs) const
	{
		return Velocity(this->v - lhs.v, this->omega - lhs.omega);
	}
};


struct Position {
	float x;
	float y;
	float theta;

	Position(float _x=0.0f, float _y=0.0f, float _theta=0.0f)
		:x(_x), y(_y), theta(_theta) {}
};




#endif /* CONTROLLER_GEOMETRY_H_ */
