/*
 * enc.c
 *
 *  Created on: 2016/09/29
 *      Author: idt12312
 */

#include "stm32f4xx_conf.h"
#include "enc.h"

#define ENC1_TIM_DEVICE				TIM4
#define ENC1_TIM_RCC_ENABLE()		RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE)

#define ENC1_PORT					GPIOB
#define ENC1_PORT_RCC_ENABLE()		RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE)
#define ENC1_A_PIN					GPIO_Pin_6
#define ENC1_B_PIN					GPIO_Pin_7
#define ENC1_A_PIN_AF_CONFIG()		GPIO_PinAFConfig(ENC1_PORT, GPIO_PinSource6, GPIO_AF_TIM4)
#define ENC1_B_PIN_AF_CONFIG()		GPIO_PinAFConfig(ENC1_PORT, GPIO_PinSource7, GPIO_AF_TIM4)


#define ENC2_TIM_DEVICE				TIM8
#define ENC2_TIM_RCC_ENABLE()		RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM8, ENABLE)

#define ENC2_PORT					GPIOC
#define ENC2_PORT_RCC_ENABLE()		RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC, ENABLE)
#define ENC2_A_PIN					GPIO_Pin_6
#define ENC2_B_PIN					GPIO_Pin_7
#define ENC2_A_PIN_AF_CONFIG()		GPIO_PinAFConfig(ENC2_PORT, GPIO_PinSource6, GPIO_AF_TIM8)
#define ENC2_B_PIN_AF_CONFIG()		GPIO_PinAFConfig(ENC2_PORT, GPIO_PinSource7, GPIO_AF_TIM8)


void Enc_init()
{
	ENC1_PORT_RCC_ENABLE();

	GPIO_InitTypeDef  GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Pin =  ENC1_A_PIN | ENC1_B_PIN;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_Init(ENC1_PORT, &GPIO_InitStructure);
	ENC1_A_PIN_AF_CONFIG();
	ENC1_B_PIN_AF_CONFIG();


	ENC1_TIM_RCC_ENABLE();

	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	TIM_TimeBaseStructure.TIM_Period = 0xffff;
	TIM_TimeBaseStructure.TIM_Prescaler = 0;
	TIM_TimeBaseStructure.TIM_ClockDivision = 0;
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseStructure.TIM_RepetitionCounter = 0;
	TIM_TimeBaseInit(ENC1_TIM_DEVICE, &TIM_TimeBaseStructure);
	TIM_EncoderInterfaceConfig(ENC1_TIM_DEVICE, TIM_EncoderMode_TI12,TIM_ICPolarity_Rising, TIM_ICPolarity_Rising);
	ENC1_TIM_DEVICE->CNT = 0;
	TIM_Cmd(ENC1_TIM_DEVICE, ENABLE);


	ENC2_PORT_RCC_ENABLE();

	GPIO_InitStructure.GPIO_Pin =  ENC2_A_PIN | ENC2_B_PIN;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_Init(ENC2_PORT, &GPIO_InitStructure);
	ENC2_A_PIN_AF_CONFIG();
	ENC2_B_PIN_AF_CONFIG();


	ENC2_TIM_RCC_ENABLE();

	TIM_TimeBaseStructure.TIM_Period = 0xffff;
	TIM_TimeBaseStructure.TIM_Prescaler = 0;
	TIM_TimeBaseStructure.TIM_ClockDivision = 0;
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseStructure.TIM_RepetitionCounter = 0;
	TIM_TimeBaseInit(ENC2_TIM_DEVICE, &TIM_TimeBaseStructure);
	TIM_EncoderInterfaceConfig(ENC2_TIM_DEVICE, TIM_EncoderMode_TI12,TIM_ICPolarity_Rising, TIM_ICPolarity_Rising);
	ENC2_TIM_DEVICE->CNT = 0;
	TIM_Cmd(ENC2_TIM_DEVICE, ENABLE);
}


void Enc_read(int32_t enc_value[2])
{
	uint16_t enc1_cnt = ENC1_TIM_DEVICE->CNT;
	ENC1_TIM_DEVICE->CNT = 0;

	uint16_t enc2_cnt = ENC2_TIM_DEVICE->CNT;
	ENC2_TIM_DEVICE->CNT = 0;

	if (enc1_cnt > 32767) enc_value[0] = (int16_t)enc1_cnt;
	else enc_value[0] = enc1_cnt;

	// 右タイヤは正転逆転が逆になる
	if (enc2_cnt > 32767) enc_value[1] = -(int16_t)enc2_cnt;
	else enc_value[1] = -enc2_cnt;
}
