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
		PIVOT,
		NO_CONTROL
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
	Motion(const Position &_goal_pos, bool _req_reset_odometry) :end(false), req_reset_odometry(_req_reset_odometry), goal_pos(_goal_pos) {}
	virtual ~Motion(){}

	inline bool is_end() const { return end; }
	virtual TrackingTarget next() = 0;
	virtual void reset(const Position &pos) = 0;
	inline bool is_req_reset_odometry() const { return req_reset_odometry; }
	inline Position get_gial_pos() const { return goal_pos; }

protected:
	bool end;
	bool req_reset_odometry;
	Position goal_pos;
};


/************************************************
 * 直進
 ***********************************************/
class Straight : public Motion {
public:
	Straight(float L, float v_start, float v_end, float ax, float v_max, bool _req_reset_odometry=false);
	virtual ~Straight();

	TrackingTarget next();
	void reset(const Position &pos);

private:
	Odometry odo;
	Trapezoid trapezoid;
	size_t cnt = 0;
	float _v_end;
};


/************************************************
 * 超信地旋回
 ***********************************************/
class PivotTurn : public Motion {
public:
	PivotTurn(float angle, float rot_ax, float omega_max, bool _req_reset_odometry=false);
	virtual ~PivotTurn();

	TrackingTarget next();
	void reset(const Position &pos);

private:
	Odometry odo;
	Trapezoid trapezoid;
	size_t cnt = 0;
};


/************************************************
 * スラロームターン
 ***********************************************/
class SlalomTurn : public Motion {
public:
	SlalomTurn(float angle, float v_const, float rot_ax, float omega_max, bool _req_reset_odometry=false);
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
