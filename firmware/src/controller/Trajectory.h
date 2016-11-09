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
#include "arm_math.h"
#include "config.h"


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
		:odo(0.005), trapezoid(0.005, 1, 0.3, 0, 0, 0.5) {}
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
		:odo(0.005), trapezoid(0.005, 15, 5, 0, PI/2, PI) {}
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


#endif /* TRAJECTORY_TRAJECTORY_H_ */
