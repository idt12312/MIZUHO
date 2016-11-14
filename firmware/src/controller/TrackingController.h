/*
 * TracklingController.h
 *
 *  Created on: 2016/11/02
 *      Author: idt12312
 */

#ifndef TRACKINGCONTROLLER_H_
#define TRACKINGCONTROLLER_H_


#include "Motion.h"
#include "Geometry.h"
#include "PIDController.h"


class TrackingController {
public:
	TrackingController();
	virtual ~TrackingController();

	Velocity update(const Position &pos, const TrackingTarget &ref);
	void reset();


private:
	PIDController pos_x_controller;
	PIDController pos_y_controller;
	PIDController angle_controller;

	Velocity track_slalom(const Position &pos, const Position &ref);
	Velocity track_pivot(const Position &pos, const Position &ref);
};


#endif /* TRACKINGCONTROLLER_H_ */
