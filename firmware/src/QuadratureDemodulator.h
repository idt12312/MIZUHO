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
#include <memory>
#include "irsensor.h"

class QuadratureDemodulator {
private:
	// (1<<sacle)がsin, cosを整数で表現するときのfull rangeの値
	const size_t sin_scale = 15;
	const size_t N;
	const size_t n;
	const size_t m;
	std::unique_ptr<int16_t[]> Wc;
	std::unique_ptr<int16_t[]> Ws;

public:
	// Nがnの整数倍になる必要がある
	QuadratureDemodulator(size_t __N, size_t _n);
	virtual ~QuadratureDemodulator() {}

	struct Result {
		uint32_t sensor1 = 0;
		uint32_t sensor2 = 0;
		uint32_t sensor3 = 0;
		uint32_t sensor4 = 0;
		Result(uint32_t _s1, uint32_t _s2, uint32_t _s3, uint32_t _s4)
			:sensor1(_s1), sensor2(_s2), sensor3(_s3), sensor4(_s4) {}
	};

	uint32_t calc(const int16_t *x);
	Result calc(const SensorRawData *raw_data);
};

#endif /* QUADRATUREDEMODULATOR_H_ */
