/*
 * isr.c
 *
 *  Created on: 2016/09/29
 *      Author: idt12312
 */

#include "stm32f4xx_conf.h"
#include "uart.h"
#include "tick.h"


void SysTick_Handler()
{
	Tick_update();
}

void DMA1_Stream3_IRQHandler()
{
	Uart_tx_dma_isr();
	DMA_ClearITPendingBit(DMA1_Stream3, DMA_IT_TCIF3);
}
