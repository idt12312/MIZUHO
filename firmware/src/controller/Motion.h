/*
 * Motion.h
 *
 *  Created on: 2016/11/01
 *      Author: idt12312
 */

#ifndef MOTION_MOTION_H_
#define MOTION_MOTION_H_


#include "Geometry.h"
#include "Trapezoid.h"
#include "Odometry.h"
#include "config.h"
#include <cstdio>

struct TrackingTarget {
	enum class Type {
		STRAIGHT,
		SLALOM,
		PIVOT
	} type;
	Position pos;
	Velocity v;

	TrackingTarget(Type _type, const Position &_pos, const Velocity &_v)
		:  type(_type), pos(_pos), v(_v) {}
};


// 終了したらend=trueにする
// 終了してもnextが呼ばれたら最後の値を返し続ける実装にする
class Motion {
protected:
	bool end;
	bool adjust_odometry_flag;

public:
	Motion() :end(false), adjust_odometry_flag(false) {}
	virtual ~Motion(){}

	inline bool is_end() const {return end;}
	virtual TrackingTarget next() = 0;
	virtual void reset(const Position &pos) = 0;
	inline bool get_adjust_odometry_flag() const {return adjust_odometry_flag;}
	inline void set_adjust_odometry_flag() { adjust_odometry_flag = true; }
};



class Straight : public Motion {
public:
	Straight(float L, float v_start=0.0f, float v_end=0.0f)
		:odo(0.005),
		 trapezoid(TRACKING_CONTROL_TASK_PERIOD_SEC, L, STRAIGHT_ACCERALATION, STRAIGHT_MAX_VELOCITY, v_start, v_end)
	{
	}

	virtual ~Straight() {}

	TrackingTarget next()
	{
		if (trapezoid.is_end()) end = true;
		const Velocity next_v = Velocity(trapezoid.next(), 0);
		odo.update(next_v);

		return TrackingTarget(TrackingTarget::Type::STRAIGHT, odo.get_pos(), next_v);
	}

	void reset(const Position &pos)
	{
		odo.reset();
		trapezoid.reset();
		end = false;
		while (1) {
			TrackingTarget target = next();
			if (std::abs(pos.y) < std::abs(target.pos.y)) break;
		}
	}

private:
	Odometry odo;
	Trapezoid trapezoid;
};


class PivotTurn : public Motion {
public:
	PivotTurn(float angle)
		:odo(0.005),
		 trapezoid(TRACKING_CONTROL_TASK_PERIOD_SEC, angle, PIVOT_ROTATION_ACCERALATION, PIVOT_ROTATION_VELOCITY, 0, 0)
	{
	}

	virtual ~PivotTurn() {}

	TrackingTarget next()
	{
		if (trapezoid.is_end()) end = true;
		const Velocity next_v = Velocity(0, trapezoid.next());
		odo.update(next_v);

		return TrackingTarget(TrackingTarget::Type::PIVOT, odo.get_pos(), next_v);
	}

	void reset(const Position &pos)
	{
		odo.reset();
		trapezoid.reset();
		end = false;
		/*while (1) {
			TrajectoryTarget target = next();
			if (std::abs(pos.theta) < std::abs(target.pos.theta)) break;
		}*/
	}

private:
	Odometry odo;
	Trapezoid trapezoid;
};


class SlalomTurn : public Motion {
public:
	SlalomTurn(float angle)
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
	virtual ~SlalomTurn() {}

	TrackingTarget next()
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

	void reset(const Position &pos)
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

private:
	Odometry odo;
	const float v;
	Trapezoid trapezoid;

	size_t cnt;
	size_t first_straight_len_cnt;
	size_t curve_len_cnt;
	size_t second_straight_len_cnt;
};


#endif /* MOTION_MOTION_H_ */
