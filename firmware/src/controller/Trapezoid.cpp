/*
 * Trapezoid.cpp
 *
 *  Created on: 2016/11/14
 *      Author: idt12312
 */


#include <cmath>
#include "Trapezoid.h"

Trapezoid::Trapezoid(float _dt, float L_, float _ax, float _v_max, float _v_start, float _v_end)
:dt(_dt), L(std::abs(L_)), dir(sign(L_)), v_max(std::abs(_v_max)), v_start(std::abs(_v_start)), v_end(std::abs(_v_end))
{
	_ax = std::abs(_ax);
	N1 = (size_t)(-(2*v_start-std::sqrt(2*v_start*v_start + 2*v_end*v_end + 4*L*_ax))/(2*_ax*dt));
	N3 = (size_t)(-(2*v_end-std::sqrt(2*v_start*v_start + 2*v_end*v_end + 4*L*_ax))/(2*_ax*dt));

	ax = (L - dt*(N1*v_start + N3*v_end)) / (0.5*dt*dt*(N1*N1+N3*N3));

	if (v_start+ax*N1*dt < _v_max) {
		N2 = 0;
	}
	else {
		N1 = (size_t)((v_max - v_start)/(dt*ax));
		N3 = (size_t)((v_max - v_end)/(dt*ax));

		const float L2 = L - (0.5*ax*((N1*dt)*(N1*dt)+(N3*dt)*(N3*dt)) + v_start*N1*dt+v_end*N3*dt);
		N2 = (size_t)(L2/(dt*v_max));
		v_max = L2/(dt*N2);
	}

	reset();
}


Trapezoid::~Trapezoid()
{

}


float Trapezoid::next()
{
	float v;
	if (cnt <= N1) {
		v = v_start + ax*dt*cnt;
	}
	else if (cnt <= N1+N2) {
		v = v_max;
	}
	else if (cnt < N1+N2+N3) {
		v = v_end + ax*dt*(N1+N2+N3-cnt);
	}

	if (!end) cnt++;

	if (cnt >= N1+N2+N3) {
		end = true;
		v = v_end;
	}

	return dir*v;
}


void Trapezoid::reset()
{
	cnt = 0;
	end = false;
}

