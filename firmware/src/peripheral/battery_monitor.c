
#include "battery_monitor.h"

#include "stm32f4xx_conf.h"


void BatteryMonitor_init()
{
	// GPIOをアナログ入力に設定
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC, ENABLE);

	GPIO_InitTypeDef  GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_0;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AN;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_Init(GPIOC, &GPIO_InitStructure);


	// ADCの設定
	// Continuousモードで変換し続ける
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC3, ENABLE);

	ADC_InitTypeDef  ADC_InitStructure;
	ADC_InitStructure.ADC_Resolution = ADC_Resolution_12b;
	ADC_InitStructure.ADC_ScanConvMode = DISABLE;
	ADC_InitStructure.ADC_ContinuousConvMode = DISABLE;
	ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;
	ADC_InitStructure.ADC_NbrOfConversion = 1;
	ADC_Init(ADC3, &ADC_InitStructure);

	ADC_RegularChannelConfig(ADC3, ADC_Channel_10, 1, ADC_SampleTime_15Cycles);

	ADC_Cmd(ADC3, ENABLE);

	ADC_SoftwareStartConv(ADC3);
}


uint16_t BatteryMonitor_read()
{
	ADC_SoftwareStartConv(ADC3);
	while (ADC_GetFlagStatus(ADC3, ADC_FLAG_EOC) == RESET);
	return ADC_GetConversionValue(ADC3);
}
