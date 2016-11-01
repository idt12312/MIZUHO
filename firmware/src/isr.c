/*
 * isr.c
 *
 *  Created on: 2016/09/29
 *      Author: idt12312
 */

#include "stm32f4xx_conf.h"
#include "uart.h"
#include "irsensor.h"
#include "spi.h"


void DMA1_Stream3_IRQHandler()
{
	Uart_tx_dma_isr();
	DMA_ClearITPendingBit(DMA1_Stream3, DMA_IT_TCIF3);
}


void DMA2_Stream0_IRQHandler()
{
	if (DMA_GetITStatus(DMA2_Stream0, DMA_IT_HTIF0)) {
		IrSensor_adc_dma_half_complete_isr();
		DMA_ClearITPendingBit(DMA2_Stream0, DMA_IT_HTIF0);
	}
	else if (DMA_GetITStatus(DMA2_Stream0, DMA_IT_TCIF0)) {
		IrSensor_adc_dma_complete_isr();
		DMA_ClearITPendingBit(DMA2_Stream0, DMA_IT_TCIF0);
	}
}


void DMA2_Stream2_IRQHandler()
{
	Spi_dma_complete_isr();
	DMA_ClearITPendingBit(DMA2_Stream2, DMA_IT_TCIF2);
}
