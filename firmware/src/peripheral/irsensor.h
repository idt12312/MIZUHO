/*
 * irsensor.h
 *
 *  Created on: 2016/10/20
 *      Author: idt12312
 */

#ifndef IRSENSOR_H_
#define IRSENSOR_H_

#ifdef	__cplusplus
extern "C"
{
#endif

#include <stdbool.h>
#include <stddef.h>

typedef struct {
	uint32_t*	sensor13;
	uint32_t*	sensor24;
	size_t		size;
} SensorRawData;


// SensorRawDataからデータを取り出すときに使うマクロ
// snesor3の10番目のデータを取り出すときは
// SensorRawData raw;
// val = SENSOR3(raw, 10);
#define SENSOR1(x, i)	(uint16_t)(0x0000ffff & x.sensor13[i])
#define SENSOR2(x, i)	(uint16_t)(0x0000ffff & x.sensor24[i])
#define SENSOR3(x, i)	(uint16_t)((0xffff0000 & x.sensor13[i])>>16)
#define SENSOR4(x, i)	(uint16_t)((0xffff0000 & x.sensor24[i])>>16)


void IrSensor_init();
void IrSensor_start();
bool IrSensor_busy();
void IrSensor_get(SensorRawData *raw_data);

// DNAの半分転送が終わった割り込みから呼ぶ
void IrSensor_adc_dma_half_complete_isr();
// DMAの完全に転送が終わった割り込みから呼ぶ
void IrSensor_adc_dma_complete_isr();

#ifdef	__cplusplus
}
#endif


#endif /* IRSENSOR_H_ */
