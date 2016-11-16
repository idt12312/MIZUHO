/*
 * QuadratureDemodulator.cpp
 *
 *  Created on: 2016/10/28
 *      Author: idt12312
 */

#include "QuadratureDemodulator.h"
#include <cmath>
#include <cstdio>
#include "stm32f4xx.h"
#include "arm_math.h"

QuadratureDemodulator::QuadratureDemodulator(size_t __N, size_t _n)
	: N(__N), n(_n), m(__N/_n), Wc(new int16_t[__N/_n]), Ws(new int16_t[__N/_n])
{
	for (size_t i=0;i<m;i++) {
		Wc[i] = (int16_t)(std::cos(-2.0f*M_PI/m*i)* (float)(1<<sin_scale));
		Ws[i] = (int16_t)(std::sin(-2.0f*M_PI/m*i)* (float)(1<<sin_scale));
	}
}


uint32_t QuadratureDemodulator::calc(const int16_t *x)
{
	int32_t Xim = 0;
	int32_t Xre = 0;
	for (size_t i=0;i<m;i++) {
		int32_t xsum = 0;
		for (size_t j=0;j<n;j++) {
			xsum += x[i+m*j];
		}
		Xre += Wc[i]*xsum;
		Xim += Ws[i]*xsum;
	}

	// sin cosをスケーリングしていたので、その分を戻す
	Xre = Xre >> sin_scale;
	Xim = Xre >> sin_scale;

	// ノルムを計算
	// int32_tのまま2乗するとオーバーフローするのでint64_tに
	const int64_t Xre_f = (int64_t)Xre;
	const int64_t Xim_f = (int64_t)Xim;
	const uint64_t X_square = Xre_f*Xre_f + Xim_f*Xim_f;

	return (uint32_t)std::sqrt(X_square/N);
}


QuadratureDemodulator::Result QuadratureDemodulator::calc(const SensorRawData *raw_data)
{
	int32_t Xim[4] = {0};
	int32_t Xre[4] = {0};

	for (size_t i=0;i<m;i++) {
		uint32_t x13sum = 0;
		uint32_t x24sum = 0;
		for (size_t j=0;j<n;j++) {
			// unsigned16bitを飽和ありで足し算
			x13sum = __UQADD16(x13sum, raw_data->sensor13[i+m*j]);
			x24sum = __UQADD16(x24sum, raw_data->sensor24[i+m*j]);
		}

		const uint32_t x1sum = x13sum&0x0000ffff;
		Xre[0] += Wc[i]*x1sum;
		Xim[0] += Ws[i]*x1sum;

		const uint32_t x3sum = x13sum>>16;
		Xre[2] += Wc[i]*x3sum;
		Xim[2] += Ws[i]*x3sum;

		const uint32_t x2sum = x24sum&0x0000ffff;
		Xre[1] += Wc[i]*x2sum;
		Xim[1] += Ws[i]*x2sum;

		const uint32_t x4sum = x24sum>>16;
		Xre[3] += Wc[i]*x4sum;
		Xim[3] += Ws[i]*x4sum;
	}

	uint32_t result[4];
	for (int i=0;i<4;i++) {
		// sin cosをスケーリングしていたので、その分を戻す
		Xre[i] = Xre[i] >> sin_scale;
		Xim[i] = Xim[i] >> sin_scale;

		// ノルムを計算
		// int32_tのまま2乗するとオーバーフローするのでint64_tに
		const int64_t Xre_f = (int64_t)Xre[i];
		const int64_t Xim_f = (int64_t)Xim[i];
		const uint64_t X_square = Xre_f*Xre_f + Xim_f*Xim_f;

		result[i] = (uint32_t)std::sqrt(X_square/N);
	}

	return Result(result[0],result[1],result[2],result[3]);
}
