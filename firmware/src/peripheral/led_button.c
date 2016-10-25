/*
 * led_button.c
 *
 *  Created on: 2016/10/23
 *      Author: idt12312
 */


#include "led_button.h"
#include "stm32f4xx_conf.h"


#define LED_1_PORT	GPIOB
#define LED_1_PIN	GPIO_Pin_12
#define LED_2_PORT	GPIOA
#define LED_2_PIN	GPIO_Pin_2
#define LED_3_PORT	GPIOA
#define LED_3_PIN	GPIO_Pin_1
#define LED_4_PORT	GPIOB
#define LED_4_PIN	GPIO_Pin_9
#define LED_5_PORT	GPIOA
#define LED_5_PIN	GPIO_Pin_12

#define SW_L_PORT	GPIOC
#define SW_L_PIN	GPIO_Pin_9
#define SW_R_PORT	GPIOA
#define SW_R_PIN	GPIO_Pin_15



void Led_init()
{
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);

	GPIO_InitTypeDef  GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_9 | GPIO_Pin_12;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_Init(GPIOB, &GPIO_InitStructure);


	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);

	GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_1 | GPIO_Pin_2 | GPIO_Pin_12;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_Init(GPIOA, &GPIO_InitStructure);

	// LED off
	GPIO_ResetBits(GPIOB, GPIO_Pin_9 | GPIO_Pin_12);
	GPIO_ResetBits(GPIOA, GPIO_Pin_1 | GPIO_Pin_2 | GPIO_Pin_12);
}


void Led_on(uint8_t pin)
{
	if (pin & LED_1) GPIO_SetBits(LED_1_PORT, LED_1_PIN);
	if (pin & LED_2) GPIO_SetBits(LED_2_PORT, LED_2_PIN);
	if (pin & LED_3) GPIO_SetBits(LED_3_PORT, LED_3_PIN);
	if (pin & LED_4) GPIO_SetBits(LED_4_PORT, LED_4_PIN);
	if (pin & LED_5) GPIO_SetBits(LED_5_PORT, LED_5_PIN);
}


void Led_off(uint8_t pin)
{
	if (pin & LED_1) GPIO_ResetBits(LED_1_PORT, LED_1_PIN);
	if (pin & LED_2) GPIO_ResetBits(LED_2_PORT, LED_2_PIN);
	if (pin & LED_3) GPIO_ResetBits(LED_3_PORT, LED_3_PIN);
	if (pin & LED_4) GPIO_ResetBits(LED_4_PORT, LED_4_PIN);
	if (pin & LED_5) GPIO_ResetBits(LED_5_PORT, LED_5_PIN);
}


void Led_toggle(uint8_t pin)
{
	if (pin & LED_1) GPIO_ToggleBits(LED_1_PORT, LED_1_PIN);
	if (pin & LED_2) GPIO_ToggleBits(LED_2_PORT, LED_2_PIN);
	if (pin & LED_3) GPIO_ToggleBits(LED_3_PORT, LED_3_PIN);
	if (pin & LED_4) GPIO_ToggleBits(LED_4_PORT, LED_4_PIN);
	if (pin & LED_5) GPIO_ToggleBits(LED_5_PORT, LED_5_PIN);
}


void Button_init()
{
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC, ENABLE);

	GPIO_InitTypeDef  GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_9;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_Init(GPIOC, &GPIO_InitStructure);


	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);

	GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_15;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
}


bool ButtonL_get()
{
	return GPIO_ReadInputDataBit(SW_L_PORT, SW_L_PIN) == 0;
}


bool ButtonR_get()
{
	return GPIO_ReadInputDataBit(SW_R_PORT, SW_R_PIN) == 0;
}

