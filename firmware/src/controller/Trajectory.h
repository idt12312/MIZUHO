/*
 * Trajectory.h
 *
 *  Created on: 2016/11/01
 *      Author: idt12312
 */

#ifndef TRAJECTORY_TRAJECTORY_H_
#define TRAJECTORY_TRAJECTORY_H_


#include "Geometry.h"
#include "Trapezoid.h"
#include "Odometry.h"
#include "config.h"
#include <cstdio>

struct TrajectoryTarget {
	enum class Type {
		STRAIGHT,
		SLALOM,
		PIVOT
	} type;
	Position pos;
	Velocity v;

	TrajectoryTarget(Type _type, const Position &_pos, const Velocity &_v)
		:  type(_type), pos(_pos), v(_v) {}
};


// 終了したらend=trueにする
// 終了してもnextが呼ばれたら最後の値を返し続ける実装にする
class Trajectory {
protected:
	bool end;

public:
	Trajectory() :end(false) {}
	virtual ~Trajectory(){}

	inline bool is_end() {return end;}
	virtual TrajectoryTarget next() = 0;
};



class Straight : public Trajectory {
public:
	Straight()
		:odo(0.005), trapezoid(0.005, 0.5, 1, 0.3, 0, 0) {}
	virtual ~Straight() {}

	TrajectoryTarget next()
	{
		if (trapezoid.is_end()) end = true;
		const Velocity next_v = Velocity(trapezoid.next(), 0);
		odo.update(next_v);

		return TrajectoryTarget(TrajectoryTarget::Type::STRAIGHT, odo.get_pos(), next_v);
	}

private:
	Odometry odo;
	Trapezoid trapezoid;
};


class PivotTurnRight90 : public Trajectory {
public:
	PivotTurnRight90()
		:odo(0.005), trapezoid(0.005, PI/2, 15, 5, 0, 0) {}
	virtual ~PivotTurnRight90() {}

	TrajectoryTarget next()
	{
		if (trapezoid.is_end()) end = true;
		const Velocity next_v = Velocity(0, trapezoid.next());
		odo.update(next_v);

		return TrajectoryTarget(TrajectoryTarget::Type::PIVOT, odo.get_pos(), next_v);
	}

private:
	Odometry odo;
	Trapezoid trapezoid;
};


class SlalomTurnRight90 : public Trajectory {
public:
	SlalomTurnRight90()
		:odo(0.005), trapezoid(0.005, -PI/2, 20, 5, 0, 0)
	{
		odo.reset();
		curve_len_cnt = 0;
		while (!trapezoid.is_end()) {
			const float omega = trapezoid.next();
			odo.update(Velocity(v,omega));
			curve_len_cnt++;
		}
		curve_length = std::abs(odo.get_pos().x);

		if (curve_length > BLOCK_SIZE/2) {
			//error
			printf("error\n");
			while (1);
		}

		straight_length = BLOCK_SIZE/2 - curve_length;
		first_straight_len_cnt = (size_t)(straight_length/(v*0.005));
		curve_len_cnt += first_straight_len_cnt;
		second_straight_len_cnt = curve_len_cnt + first_straight_len_cnt;


		odo.reset();
		trapezoid.reset();
		cnt = 0;
	}
	virtual ~SlalomTurnRight90() {}

	TrajectoryTarget next()
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
			next_v = Velocity(0,0);
		}
		cnt++;
		odo.update(next_v);
		return TrajectoryTarget(TrajectoryTarget::Type::SLALOM, odo.get_pos(), next_v);
	}

private:
	Odometry odo;
	Trapezoid trapezoid;

	size_t cnt;
	float curve_length;
	float straight_length;
	size_t first_straight_len_cnt;
	size_t curve_len_cnt;
	size_t second_straight_len_cnt;
	const float v = 0.2f;
};


#endif /* TRAJECTORY_TRAJECTORY_H_ */
