/*
 * irsensor.c
 *
 *  Created on: 2016/10/20
 *      Author: idt12312
 */

#include "stm32f4xx_conf.h"
#include "irsensor.h"

// 赤外線LEDを駆動するPWMの周波数
// 10kHz = 84MHz / 8400
#define IRSENSOR_PWM_TIM_PERIAD 8400

#define IRSENSOR_PWM_DUTY 0.5f

// パルス何個分を計測するか
#define N_PULSE 4

// PWMの１周期中に何回サンプルするか
// これにPWM周波数をかけるとADCのサンプリング周波数になる
#define N_MUL 16


// 一回の計測で1chあたり何回サンプルするか
#define N_SAMPLE N_MUL*N_PULSE

// ADCの結果を保存するバッファのサイズ
#define N_ADC_CNT N_SAMPLE*2

// ADCトリガ用タイマの周期
#define ADC_TRIG_TIM_PERIOD IRSENSOR_PWM_TIM_PERIAD/N_MUL


typedef enum {
	SENSOR13,
	SENSOR24
} Sensor;


// adc_result[i]の
// 前半部分はch1と3で, 上位16bitがch3, 下位16bitがch1
// 後半部分はch2と4で, 上位16bitがch4, 下位16bitがch2
// ダブルバッファリングをするので二つ確保してある
static uint32_t adc_result[2][N_ADC_CNT];

static bool measuring;
static Sensor measuring_sensor;



static void pwm_init()
{
	// GPIOを設定
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);

	GPIO_InitTypeDef  GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_6 | GPIO_Pin_7;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_Init(GPIOA, &GPIO_InitStructure);

	// DACのピンは入力にしとく
	GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_4 | GPIO_Pin_5;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_OD;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_Init(GPIOA, &GPIO_InitStructure);

	GPIO_PinAFConfig(GPIOA, GPIO_PinSource6, GPIO_AF_TIM3);
	GPIO_PinAFConfig(GPIOA, GPIO_PinSource7, GPIO_AF_TIM3);


	// PWMを出すTIMを設定
	// TimeBaseではPWMの周期を設定
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);

	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	TIM_TimeBaseStructure.TIM_Period = IRSENSOR_PWM_TIM_PERIAD - 1;
	TIM_TimeBaseStructure.TIM_Prescaler = 0;
	TIM_TimeBaseStructure.TIM_ClockDivision = 0;
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseStructure.TIM_RepetitionCounter = 0;
	TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure);

	// 各PWM出力ピンに対応するコンペアレジスタを設定
	// ここでは共に出力0に設定
	// pwm_startでPWMモードに設定をしている
	TIM_OCInitTypeDef  TIM_OCInitStructure;
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
	TIM_OCInitStructure.TIM_OutputNState = TIM_OutputNState_Disable;
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;
	TIM_OCInitStructure.TIM_OCIdleState = TIM_OCIdleState_Reset;
	TIM_OCInitStructure.TIM_Pulse = 0;
	TIM_OC1Init(TIM3, &TIM_OCInitStructure);
	TIM_OC2Init(TIM3, &TIM_OCInitStructure);

	// CCRの値はTIMのoverflowイベントとは非同期で反映される
	TIM_OC1PreloadConfig(TIM3, TIM_OCPreload_Disable);
	TIM_OC2PreloadConfig(TIM3, TIM_OCPreload_Disable);
}


static void pwm_start(Sensor sensor)
{
	if (sensor == SENSOR13) {
		TIM3->CCR1 = (uint16_t)(IRSENSOR_PWM_TIM_PERIAD * IRSENSOR_PWM_DUTY);
		TIM3->CCR2 = 0;
	}
	else {
		TIM3->CCR1 = 0;
		TIM3->CCR2 = (uint16_t)(IRSENSOR_PWM_TIM_PERIAD * IRSENSOR_PWM_DUTY);
	}

	// PWM出力開始
	TIM3->CNT = 0;
	TIM_Cmd(TIM3, ENABLE);
}


static void pwm_stop()
{
	TIM3->CCR1 = 0;
	TIM3->CCR2 = 0;
	TIM_Cmd(TIM3, DISABLE);
}


static void adc_init()
{
	// GPIOをアナログ入力に設定
	GPIO_InitTypeDef  GPIO_InitStructure;

	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC, ENABLE);
	GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_4 | GPIO_Pin_5;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AN;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_Init(GPIOC, &GPIO_InitStructure);

	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);
	GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_0 | GPIO_Pin_1;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AN;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_Init(GPIOB, &GPIO_InitStructure);


	// ADCのトリガとなるタイマの設定
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2 , ENABLE);

	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	TIM_TimeBaseStructure.TIM_Period = ADC_TRIG_TIM_PERIOD-1;
	TIM_TimeBaseStructure.TIM_Prescaler = 0;
	TIM_TimeBaseStructure.TIM_ClockDivision = 0;
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseStructure.TIM_RepetitionCounter = 0;
	TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure);


	TIM_OCInitTypeDef  TIM_OCInitStructure;
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
	TIM_OCInitStructure.TIM_OutputNState = TIM_OutputNState_Disable;
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;
	TIM_OCInitStructure.TIM_OCIdleState = TIM_OCIdleState_Reset;
	TIM_OCInitStructure.TIM_Pulse = ADC_TRIG_TIM_PERIOD-2;
	TIM_OC2Init(TIM2, &TIM_OCInitStructure);


	// ADCの設定
	// TIM2 TRGOをトリガに変換をする
	// チャンネルの設定はadc_startで
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC2, ENABLE);

	ADC_InitTypeDef  ADC_InitStructure;
	ADC_InitStructure.ADC_Resolution = ADC_Resolution_12b;
	ADC_InitStructure.ADC_ScanConvMode = ENABLE;
	ADC_InitStructure.ADC_ContinuousConvMode = DISABLE;
	ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_T2_CC2;
	ADC_InitStructure.ADC_ExternalTrigConvEdge = ADC_ExternalTrigConvEdge_Rising;
	ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;
	ADC_InitStructure.ADC_NbrOfConversion = 1;
	ADC_Init(ADC1, &ADC_InitStructure);
	ADC_Init(ADC2, &ADC_InitStructure);

	// ADCのクロック 21MHz = 84MHz / 4
	// デュアルモードでADC1,2を同時に使う
	// DMA access mode 2(32bit幅で転送し, 上位16bitがADC2, 下位16bitがADC1)
	ADC_CommonInitTypeDef ADC_CommonInitStructure;
	ADC_CommonInitStructure.ADC_Mode = ADC_DualMode_RegSimult;
	ADC_CommonInitStructure.ADC_Prescaler = ADC_Prescaler_Div4;
	ADC_CommonInitStructure.ADC_DMAAccessMode = ADC_DMAAccessMode_2;
	ADC_CommonInitStructure.ADC_TwoSamplingDelay = ADC_TwoSamplingDelay_5Cycles;
	ADC_CommonInit(&ADC_CommonInitStructure);

	// ADC1,2の変換が両方とも終わったらDMAで転送
	ADC_MultiModeDMARequestAfterLastTransferCmd(ENABLE);

	ADC_Cmd(ADC1, ENABLE);
	ADC_Cmd(ADC2, ENABLE);


	// ADCの変換結果を転送するDMAの設定
	// ADCをdualモードで使っているので、ADCx.DRではなくADC.CDRのデータを転送する
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_DMA2, ENABLE);

	DMA_InitTypeDef DMA_InitStructure;
	DMA_InitStructure.DMA_Channel = DMA_Channel_0;
	DMA_InitStructure.DMA_Memory0BaseAddr = (uint32_t)&adc_result[0];
	DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)&ADC->CDR;
	DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralToMemory;
	DMA_InitStructure.DMA_BufferSize = N_ADC_CNT;
	DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
	DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;
	DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Word;
	DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Word;
	DMA_InitStructure.DMA_Mode = DMA_Mode_Circular;
	DMA_InitStructure.DMA_Priority = DMA_Priority_High;
	DMA_InitStructure.DMA_FIFOMode = DMA_FIFOMode_Disable;
	DMA_InitStructure.DMA_FIFOThreshold = DMA_FIFOThreshold_HalfFull;
	DMA_InitStructure.DMA_MemoryBurst = DMA_MemoryBurst_Single;
	DMA_InitStructure.DMA_PeripheralBurst = DMA_PeripheralBurst_Single;
	DMA_Init(DMA2_Stream0, &DMA_InitStructure);

	// ダブルバッファリングを使う
	DMA_DoubleBufferModeConfig(DMA2_Stream0, (uint32_t)&adc_result[1], DMA_Memory_0);
	DMA_DoubleBufferModeCmd(DMA2_Stream0, ENABLE);

	DMA_Cmd(DMA2_Stream0, ENABLE);

	// 転送が完全に終わったら割り込む
	DMA_ClearITPendingBit(DMA2_Stream0, DMA_IT_TCIF0);
	DMA_ITConfig(DMA2_Stream0, DMA_IT_TC, ENABLE);

	// 転送が半分終わったら割り込む
	DMA_ClearITPendingBit(DMA2_Stream0, DMA_IT_HTIF0);
	DMA_ITConfig(DMA2_Stream0, DMA_IT_HT, ENABLE);


	NVIC_InitTypeDef NVIC_InitStructure;
	NVIC_InitStructure.NVIC_IRQChannel = DMA2_Stream0_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 10;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 10;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
}


static void adc_start(Sensor sensor)
{
	TIM_Cmd(TIM2, DISABLE);
	if (sensor == SENSOR13) {
		ADC_RegularChannelConfig(ADC1, ADC_Channel_14, 1, ADC_SampleTime_15Cycles);
		ADC_RegularChannelConfig(ADC2, ADC_Channel_9, 1, ADC_SampleTime_15Cycles);
	}
	else {
		ADC_RegularChannelConfig(ADC1, ADC_Channel_8, 1, ADC_SampleTime_15Cycles);
		ADC_RegularChannelConfig(ADC2, ADC_Channel_15, 1, ADC_SampleTime_15Cycles);
	}
	TIM2->CNT = 0;
	TIM_Cmd(TIM2, ENABLE);
}


static void adc_stop()
{
	TIM_Cmd(TIM2, DISABLE);
}


void IrSensor_init()
{
	pwm_init();
	adc_init();

	measuring = false;
}


// まずはSENSOR13をADC1とADC2にサンプルする
// DMAの半分転送が終わった割り込みをトリガにSENSOR24をサンプルする
void IrSensor_start()
{
	if (measuring) return;

	pwm_start(SENSOR13);
	adc_start(SENSOR13);

	measuring_sensor = SENSOR13;
	measuring = true;
}


bool IrSensor_busy()
{
	return measuring;
}


void IrSensor_get(SensorRawData *raw_data)
{
	if (measuring) return;

	// ダブルバッファリングしていたので, どちらのバッファを使ったかを取得する
	const uint32_t ready_buffer =  (1 - DMA_GetCurrentMemoryTarget(DMA2_Stream0));

	// 前半部分がsensor1とsensor3
	raw_data->sensor13 = &adc_result[ready_buffer][0];
	// 後半部分がsensor2とsensor4
	raw_data->sensor24 = &adc_result[ready_buffer][N_SAMPLE];

	raw_data->size = N_SAMPLE;
}


void IrSensor_adc_dma_half_complete_isr()
{
	pwm_start(SENSOR24);
	adc_start(SENSOR24);
	measuring_sensor = SENSOR24;
}


void IrSensor_adc_dma_complete_isr()
{
	adc_stop();
	pwm_stop();
	measuring = false;
}
