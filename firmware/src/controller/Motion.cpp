/*
 * Motion.cpp
 *
 *  Created on: 2016/11/14
 *      Author: idt12312
 */


#include "Motion.h"
#include "config.h"
#include <cstdio>



/************************************************
 * 直進
 ***********************************************/
Straight::Straight(float L, float v_start, float v_end)
:odo(0.005),
 trapezoid(TRACKING_CONTROL_TASK_PERIOD_SEC, L, STRAIGHT_ACCERALATION, STRAIGHT_MAX_VELOCITY, v_start, v_end)
{

}


Straight::~Straight()
{

}


TrackingTarget Straight::next()
{
	if (trapezoid.is_end()) end = true;
	const Velocity next_v = Velocity(trapezoid.next(), 0);
	odo.update(next_v);

	return TrackingTarget(TrackingTarget::Type::STRAIGHT, odo.get_pos(), next_v);
}


void Straight::reset(const Position &pos)
{
	odo.reset();
	trapezoid.reset();
	end = false;
	while (1) {
		TrackingTarget target = next();
		if (std::abs(pos.y) < std::abs(target.pos.y)) break;
	}
}



/************************************************
 * 超信地旋回
 ***********************************************/
PivotTurn::PivotTurn(float angle)
:odo(0.005),
 trapezoid(TRACKING_CONTROL_TASK_PERIOD_SEC, angle, PIVOT_ROTATION_ACCERALATION, PIVOT_ROTATION_VELOCITY, 0, 0)
{
}

PivotTurn::~PivotTurn()
{

}

TrackingTarget PivotTurn::next()
{
	if (trapezoid.is_end()) end = true;
	const Velocity next_v = Velocity(0, trapezoid.next());
	odo.update(next_v);

	return TrackingTarget(TrackingTarget::Type::PIVOT, odo.get_pos(), next_v);
}

void PivotTurn::reset(const Position &pos)
{
	odo.reset();
	trapezoid.reset();
	end = false;
}



/************************************************
 * スラロームターン
 ***********************************************/
SlalomTurn::SlalomTurn(float angle)
:odo(0.005),
 v(SLALOM_VELOCITY),
 trapezoid(TRACKING_CONTROL_TASK_PERIOD_SEC, angle, SLALOM_ROTATION_ACCERALATION, SLALOM_ROTATION_VELOCITY, 0, 0)
{
	odo.reset();
	curve_len_cnt = 0;
	while (!trapezoid.is_end()) {
		const float omega = trapezoid.next();
		odo.update(Velocity(v,omega));
		curve_len_cnt++;
	}
	const float curve_length = std::abs(odo.get_pos().x);

	if (curve_length > BLOCK_SIZE/2) {
		//error
		printf("error\n");
		while (1);
	}

	const float straight_length = BLOCK_SIZE/2 - curve_length;
	first_straight_len_cnt = (size_t)(straight_length/(v*0.005));
	curve_len_cnt += first_straight_len_cnt;
	second_straight_len_cnt = curve_len_cnt + first_straight_len_cnt;


	odo.reset();
	trapezoid.reset();
	cnt = 0;
}
SlalomTurn::~SlalomTurn()
{

}

TrackingTarget SlalomTurn::next()
{
	Velocity next_v;
	if (cnt < first_straight_len_cnt) {
		next_v = Velocity(v, 0);
	}
	else if (cnt < curve_len_cnt) {
		next_v = Velocity(v, trapezoid.next());
	}
	else if (cnt < second_straight_len_cnt) {
		next_v = Velocity(v, 0);
	}
	else {
		end = true;
		next_v = Velocity(v,0);
	}
	cnt++;
	odo.update(next_v);
	return TrackingTarget(TrackingTarget::Type::SLALOM, odo.get_pos(), next_v);
}

void SlalomTurn::reset(const Position &pos)
{
	odo.reset();
	trapezoid.reset();
	end = false;
	cnt = 0;
	while (1) {
		TrackingTarget target = next();
		if (pos.y < target.pos.y) break;
	}
}

