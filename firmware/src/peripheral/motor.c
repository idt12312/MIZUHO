/*
 * motor.c
 *
 *  Created on: 2016/09/29
 *      Author: idt12312
 */
#include "stm32f4xx_conf.h"
#include "motor.h"
#include "config.h"

// PWM周波数 = TIMのバスクロック / (MOTOR_TIM_PERIAD * MOTOR_TIM_PRESCALER)
// ここでは 50kHz = 84MHz(TIM1のあるAPB2) / (1 * 1680)
#define MOTOR_TIM_PERIAD 			1680
#define MOTOR_TIM_PRESCALER 		1

#define MOTOR_TIM_DEVICE			TIM1
#define MOTOR_TIM_RCC_ENABLE()		RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1, ENABLE)

#define MOTOR_PORT					GPIOA
#define MOTOR_PORT_RCC_ENABLE()		RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE)
#define MOTOR1_IN1_PIN				GPIO_Pin_10
#define MOTOR1_IN2_PIN				GPIO_Pin_11
#define MOTOR2_IN1_PIN				GPIO_Pin_8
#define MOTOR2_IN2_PIN				GPIO_Pin_9
#define MOTOR1_IN1_PIN_AF_CONFIG()	GPIO_PinAFConfig(MOTOR_PORT, GPIO_PinSource10, GPIO_AF_TIM1)
#define MOTOR1_IN2_PIN_AF_CONFIG()	GPIO_PinAFConfig(MOTOR_PORT, GPIO_PinSource11, GPIO_AF_TIM1)
#define MOTOR2_IN1_PIN_AF_CONFIG()	GPIO_PinAFConfig(MOTOR_PORT, GPIO_PinSource8, GPIO_AF_TIM1)
#define MOTOR2_IN2_PIN_AF_CONFIG()	GPIO_PinAFConfig(MOTOR_PORT, GPIO_PinSource9, GPIO_AF_TIM1)
#define MOTOR1_IN1_CCR				CCR3
#define MOTOR1_IN2_CCR				CCR4
#define MOTOR2_IN1_CCR				CCR1
#define MOTOR2_IN2_CCR				CCR2

#define MOTOR_DUTY_SATURATION		0.99f


void Motor_init()
{
	// GPIOを設定
	MOTOR_PORT_RCC_ENABLE();

	GPIO_InitTypeDef  GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Pin =  MOTOR1_IN1_PIN | MOTOR1_IN2_PIN | MOTOR2_IN1_PIN | MOTOR2_IN2_PIN;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_Init(MOTOR_PORT, &GPIO_InitStructure);

	MOTOR1_IN1_PIN_AF_CONFIG();
	MOTOR1_IN2_PIN_AF_CONFIG();
	MOTOR2_IN1_PIN_AF_CONFIG();
	MOTOR2_IN2_PIN_AF_CONFIG();


	// PWMを出すTIMを設定
	// TimeBaseではPWMの周期を設定
	MOTOR_TIM_RCC_ENABLE();

	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	TIM_TimeBaseStructure.TIM_Period = MOTOR_TIM_PERIAD - 1;
	TIM_TimeBaseStructure.TIM_Prescaler = MOTOR_TIM_PRESCALER - 1;
	TIM_TimeBaseStructure.TIM_ClockDivision = 0;
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseStructure.TIM_RepetitionCounter = 0;
	TIM_TimeBaseInit(MOTOR_TIM_DEVICE, &TIM_TimeBaseStructure);

	// 各PWM出力ピンに対応するコンペアレジスタを設定
	TIM_OCInitTypeDef  TIM_OCInitStructure;
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
	TIM_OCInitStructure.TIM_OutputNState = TIM_OutputNState_Disable;
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;
	TIM_OCInitStructure.TIM_OCIdleState = TIM_OCIdleState_Reset;
	TIM_OCInitStructure.TIM_Pulse = 0;
	TIM_OC1Init(MOTOR_TIM_DEVICE, &TIM_OCInitStructure);
	TIM_OC2Init(MOTOR_TIM_DEVICE, &TIM_OCInitStructure);
	TIM_OC3Init(MOTOR_TIM_DEVICE, &TIM_OCInitStructure);
	TIM_OC4Init(MOTOR_TIM_DEVICE, &TIM_OCInitStructure);

	// CCRの値はTIMのoverflowイベント発生に同期して反映されるように設定
	TIM_OC1PreloadConfig(MOTOR_TIM_DEVICE, TIM_OCPreload_Enable);
	TIM_OC2PreloadConfig(MOTOR_TIM_DEVICE, TIM_OCPreload_Enable);
	TIM_OC3PreloadConfig(MOTOR_TIM_DEVICE, TIM_OCPreload_Enable);
	TIM_OC4PreloadConfig(MOTOR_TIM_DEVICE, TIM_OCPreload_Enable);

	// TIM1とTIM8のみTIM_CtrlPWMOutputsを呼ばないとPWMが出力されない
	if (MOTOR_TIM_DEVICE == TIM1 || MOTOR_TIM_DEVICE == TIM8) {
		TIM_CtrlPWMOutputs(MOTOR_TIM_DEVICE,ENABLE);
	}

	// 動作開始
	TIM_Cmd(MOTOR_TIM_DEVICE, ENABLE);
}


void Motor_set_duty(const float duty[2])
{

	// dutyの絶対値をMOTOR_DUTY_SATURATION以内に収める
	float duty_sat[2];
	for (int i=0;i<2;i++) {
		if (duty[i] > MOTOR_DUTY_SATURATION) duty_sat[i] = MOTOR_DUTY_SATURATION;
		else if (duty[i] < -MOTOR_DUTY_SATURATION) duty_sat[i] = -MOTOR_DUTY_SATURATION;
		else duty_sat[i] = duty[i];
	}


	// ** DRV8835 **
	// State IN1 IN2
	// Free   0   0
	// For    1   0
	// Rev    0   1
	// Brk    1   1
	if (duty_sat[0] >= 0) {
		MOTOR_TIM_DEVICE->MOTOR1_IN1_CCR = (uint32_t)((float)MOTOR_TIM_PERIAD * duty_sat[0]);
		MOTOR_TIM_DEVICE->MOTOR1_IN2_CCR = 0;
	}
	else {
		MOTOR_TIM_DEVICE->MOTOR1_IN1_CCR = 0;
		MOTOR_TIM_DEVICE->MOTOR1_IN2_CCR = (uint32_t)((float)MOTOR_TIM_PERIAD * (-duty_sat[0]));
	}


	// 右タイヤは正転逆転が逆になる
	if (duty_sat[1] >= 0) {
		MOTOR_TIM_DEVICE->MOTOR2_IN1_CCR = 0;
		MOTOR_TIM_DEVICE->MOTOR2_IN2_CCR = (uint32_t)((float)MOTOR_TIM_PERIAD * duty_sat[1]);
	}
	else {
		MOTOR_TIM_DEVICE->MOTOR2_IN1_CCR = (uint32_t)((float)MOTOR_TIM_PERIAD * (-duty_sat[1]));
		MOTOR_TIM_DEVICE->MOTOR2_IN2_CCR = 0;
	}
}

void Motor_set_voltage(const MotorVoltage *volt)
{
	float duty[2];
	duty[0] = volt->left / MOTOR_VOLTAGE;
	duty[1] = volt->right / MOTOR_VOLTAGE;

	Motor_set_duty(duty);
}
