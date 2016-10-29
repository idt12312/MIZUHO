/*
 * QuadratureDemodulator.h
 *
 *  Created on: 2016/10/28
 *      Author: idt12312
 */

#ifndef QUADRATUREDEMODULATOR_H_
#define QUADRATUREDEMODULATOR_H_

#include <cstdint>
#include <cstddef>
#include "irsensor.h"

class QuadratureDemodulator {
private:
	// (1<<sacle)がsin, cosを整数で表現するときのfull rangeの値
	const size_t sin_scale = 15;
	const size_t N;
	const size_t n;
	const size_t m;
	int16_t *Wc;
	int16_t *Ws;

public:
	// Nがnの整数倍になる必要がある
	QuadratureDemodulator(size_t _N, size_t _n);
	virtual ~QuadratureDemodulator();

	uint32_t calc(const int16_t *x);
	void calc(const SensorRawData *raw_data, uint32_t result[4]);
};

#endif /* QUADRATUREDEMODULATOR_H_ */
