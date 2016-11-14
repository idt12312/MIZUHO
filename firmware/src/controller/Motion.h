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


/************************************************
 * 追従のための目標点
 ***********************************************/
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


/************************************************
 * Motionの基底
 ***********************************************/
// 終了したらend=trueにする
// 終了してもnextが呼ばれたら最後の値を返し続ける実装にする
class Motion {
public:
	Motion() :end(false), adjust_odometry_flag(false) {}
	virtual ~Motion(){}

	inline bool is_end() const { return end; }
	virtual TrackingTarget next() = 0;
	virtual void reset(const Position &pos) = 0;
	inline bool get_adjust_odometry_flag() const { return adjust_odometry_flag; }
	inline void set_adjust_odometry_flag() { adjust_odometry_flag = true; }

protected:
	bool end;
	bool adjust_odometry_flag;
};


/************************************************
 * 直進
 ***********************************************/
class Straight : public Motion {
public:
	Straight(float L, float v_start, float v_end);
	virtual ~Straight();

	TrackingTarget next();
	void reset(const Position &pos);

private:
	Odometry odo;
	Trapezoid trapezoid;
};


/************************************************
 * 超信地旋回
 ***********************************************/
class PivotTurn : public Motion {
public:
	PivotTurn(float angle);
	virtual ~PivotTurn();

	TrackingTarget next();
	void reset(const Position &pos);

private:
	Odometry odo;
	Trapezoid trapezoid;
};


/************************************************
 * スラロームターン
 ***********************************************/
class SlalomTurn : public Motion {
public:
	SlalomTurn(float angle);
	virtual ~SlalomTurn();

	TrackingTarget next();
	void reset(const Position &pos);

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
