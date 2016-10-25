/*
 * uart.c
 *
 *  Created on: 2016/09/29
 *      Author: idt12312
 */

#include <string.h>
#include <stdarg.h>
#include <stdio.h>
#include "stm32f4xx_conf.h"
#include "uart.h"


#define UART_DEVICE					USART3
#define UART_BAUDRATE				115200
#define UART_TX_BUFFER_SIZE			256
#define UART_RX_BUFFER_SIZE			16

#define UART_TXRX_PORT				GPIOC
#define UART_TX_PIN					GPIO_Pin_10
#define UART_RX_PIN					GPIO_Pin_11
#define UART_PORT_RCC_ENABLE()		RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC, ENABLE)
#define UART_RCC_ENABLE()			RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3, ENABLE)
#define UART_TX_PIN_AF_CONFIG()		GPIO_PinAFConfig(UART_TXRX_PORT, GPIO_PinSource10, GPIO_AF_USART3);
#define UART_RX_PIN_AF_CONFIG()		GPIO_PinAFConfig(UART_TXRX_PORT, GPIO_PinSource11, GPIO_AF_USART3);

#define UART_TX_DMA_STREAM			DMA1_Stream3
#define UART_RX_DMA_STREAM			DMA1_Stream1
#define UART_DMA_CHANNEL			DMA_Channel_4
#define UART_DMA_RCC_ENABLE()		RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_DMA1, ENABLE)
#define UART_TX_DMA_IRQ_CHANNEL		DMA1_Stream3_IRQn
#define UART_TX_DMA_IRQ_PRIORITY	1

#define MIN(x,y)	(x<y?x:y)


static uint8_t rx_buffer[UART_RX_BUFFER_SIZE];
static uint8_t tx_buffer[UART_TX_BUFFER_SIZE];
static size_t tx_buffer_head, tx_buffer_tail, tx_buffer_nitems;
static size_t tx_dma_sending_size;
static size_t rx_buffer_tail;

char strbuf[UART_TX_BUFFER_SIZE];


static void send_dma(const uint8_t *data, size_t size)
{
	DMA_DeInit(UART_TX_DMA_STREAM);

	DMA_InitTypeDef DMA_InitStructure;
	DMA_InitStructure.DMA_BufferSize        = size;
	DMA_InitStructure.DMA_Channel           = UART_DMA_CHANNEL;
	DMA_InitStructure.DMA_DIR               = DMA_DIR_MemoryToPeripheral;
	DMA_InitStructure.DMA_FIFOMode          = DMA_FIFOMode_Disable;
	DMA_InitStructure.DMA_FIFOThreshold     = DMA_FIFOThreshold_HalfFull;
	DMA_InitStructure.DMA_Memory0BaseAddr   = (uint32_t)data;
	DMA_InitStructure.DMA_MemoryBurst       = DMA_MemoryBurst_Single;
	DMA_InitStructure.DMA_MemoryDataSize    = DMA_MemoryDataSize_Byte;
	DMA_InitStructure.DMA_MemoryInc         = DMA_MemoryInc_Enable;
	DMA_InitStructure.DMA_Mode              = DMA_Mode_Normal;
	DMA_InitStructure.DMA_PeripheralBaseAddr= (uint32_t)(&(UART_DEVICE->DR));
	DMA_InitStructure.DMA_PeripheralBurst   = DMA_PeripheralBurst_Single;
	DMA_InitStructure.DMA_PeripheralDataSize= DMA_MemoryDataSize_Byte;
	DMA_InitStructure.DMA_PeripheralInc     = DMA_PeripheralInc_Disable;
	DMA_InitStructure.DMA_Priority          = DMA_Priority_Medium;

	DMA_Init(UART_TX_DMA_STREAM, &DMA_InitStructure);
	DMA_ITConfig(UART_TX_DMA_STREAM, DMA_IT_TC, ENABLE);
	DMA_Cmd(UART_TX_DMA_STREAM, ENABLE);
}


void Uart_init()
{
	// GPIOの設定
	UART_PORT_RCC_ENABLE();

	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Pin = UART_TX_PIN;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_Init(UART_TXRX_PORT, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin = UART_RX_PIN;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_Init(UART_TXRX_PORT, &GPIO_InitStructure);

	UART_TX_PIN_AF_CONFIG();
	UART_RX_PIN_AF_CONFIG();


	// UARTの設定
	UART_RCC_ENABLE();

	USART_InitTypeDef USART_InitStructure;
	USART_InitStructure.USART_BaudRate = UART_BAUDRATE;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	USART_InitStructure.USART_Parity = USART_Parity_No;
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
	USART_Init(UART_DEVICE, &USART_InitStructure);

	USART_DMACmd(UART_DEVICE,USART_DMAReq_Tx,ENABLE);
	USART_DMACmd(UART_DEVICE,USART_DMAReq_Rx,ENABLE);


	// 受信用DMAの設定
	// 送信用のDMAは送信するとに設定する
	UART_DMA_RCC_ENABLE();
	DMA_DeInit(UART_RX_DMA_STREAM);

	DMA_InitTypeDef DMA_InitStructure;
	DMA_InitStructure.DMA_BufferSize        = UART_RX_BUFFER_SIZE;
	DMA_InitStructure.DMA_Channel           = UART_DMA_CHANNEL;
	DMA_InitStructure.DMA_DIR               = DMA_DIR_PeripheralToMemory;
	DMA_InitStructure.DMA_FIFOMode          = DMA_FIFOMode_Disable;
	DMA_InitStructure.DMA_FIFOThreshold     = DMA_FIFOThreshold_HalfFull;
	DMA_InitStructure.DMA_Memory0BaseAddr   = (uint32_t)rx_buffer;
	DMA_InitStructure.DMA_MemoryBurst       = DMA_MemoryBurst_Single;
	DMA_InitStructure.DMA_MemoryDataSize    = DMA_MemoryDataSize_Byte;
	DMA_InitStructure.DMA_MemoryInc         = DMA_MemoryInc_Enable;
	DMA_InitStructure.DMA_Mode              = DMA_Mode_Circular;
	DMA_InitStructure.DMA_PeripheralBaseAddr= (uint32_t)(&(UART_DEVICE->DR));
	DMA_InitStructure.DMA_PeripheralBurst   = DMA_PeripheralBurst_Single;
	DMA_InitStructure.DMA_PeripheralDataSize= DMA_MemoryDataSize_Byte;
	DMA_InitStructure.DMA_PeripheralInc     = DMA_PeripheralInc_Disable;
	DMA_InitStructure.DMA_Priority          = DMA_Priority_Medium;
	DMA_Init(UART_RX_DMA_STREAM, &DMA_InitStructure);


	// DMAの送信完了割り込みを有効化
	NVIC_InitTypeDef NVIC_InitStructure;
	NVIC_InitStructure.NVIC_IRQChannel = UART_TX_DMA_IRQ_CHANNEL;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = UART_TX_DMA_IRQ_PRIORITY;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);

	DMA_ITConfig(UART_TX_DMA_STREAM, DMA_IT_TE, ENABLE);


	tx_buffer_head = 0;
	tx_buffer_tail = 0;
	tx_buffer_nitems = 0;
	tx_dma_sending_size = 0;

	// 動作開始
	DMA_Cmd(UART_RX_DMA_STREAM, ENABLE);
	USART_Cmd(UART_DEVICE, ENABLE);
}


void Uart_write_byte(uint8_t data)
{
	Uart_write((const uint8_t*)&data, 1);
}


size_t Uart_write(const uint8_t *data, size_t size)
{
	DMA_ITConfig(UART_TX_DMA_STREAM, DMA_IT_TC, DISABLE);

	// bufferに入りきる分だけいれる
	const size_t wrote_size = MIN(size, UART_TX_BUFFER_SIZE - tx_buffer_nitems);
	tx_buffer_nitems += wrote_size;
	for (size_t i=0;i<wrote_size;i++) {
		tx_buffer[tx_buffer_head] = data[i];
		tx_buffer_head = (tx_buffer_head + 1) % UART_TX_BUFFER_SIZE;
	}

	// DMAが動作していない時は動作を開始する
	if (tx_dma_sending_size == 0) {
		// bufferに入っているデータをbufferの終わりまでで分割してDMAにセットする
		if (tx_buffer_tail < tx_buffer_head) {
			tx_dma_sending_size = tx_buffer_head - tx_buffer_tail;
		}
		else {
			tx_dma_sending_size = UART_TX_BUFFER_SIZE - tx_buffer_tail;
		}
		send_dma(tx_buffer + tx_buffer_tail, tx_dma_sending_size);
	}

	DMA_ITConfig(UART_TX_DMA_STREAM, DMA_IT_TC, ENABLE);

	return wrote_size;
}


size_t Uart_write_str(const char *str)
{
	return Uart_write((const uint8_t *)str, strlen(str));
}


size_t Uart_printf(const char* fmt, ...)
{
	va_list vlist;
	va_start(vlist, fmt);
	vsprintf(strbuf, fmt, vlist);
	va_end(vlist);

	return Uart_write_str(strbuf);
}


size_t Uart_rcv_size()
{
	size_t size;

	// DMAが転送したデータ数を取得
	const size_t dma_cnt = UART_RX_BUFFER_SIZE - DMA_GetCurrDataCounter(UART_RX_DMA_STREAM);
	if (rx_buffer_tail <= dma_cnt) size = dma_cnt - rx_buffer_tail;
	else size = dma_cnt + (UART_RX_BUFFER_SIZE - rx_buffer_tail);

	return size;
}


uint8_t Uart_read_byte()
{
	uint8_t read_data;

	Uart_read(&read_data,1);
	return read_data;
}


size_t Uart_read(uint8_t *buffer, size_t size)
{
	// 受信しているサイズを取得
	const size_t available = Uart_rcv_size();

	// 読み出せる分だけ読み出す
	const size_t read_size = MIN(available, size);
	for (size_t i=0;i<read_size;i++) {
		buffer[i] =  rx_buffer[rx_buffer_tail];
		rx_buffer_tail = (rx_buffer_tail + 1) % UART_RX_BUFFER_SIZE;
	}

	return read_size;
}


void Uart_tx_dma_isr()
{
	// 送信した分だけtailを進める
	tx_buffer_tail = (tx_buffer_tail + tx_dma_sending_size) % UART_TX_BUFFER_SIZE;
	tx_buffer_nitems -= tx_dma_sending_size;

	// bufferが空になったらDMAを停止
	if (tx_buffer_nitems == 0) {
		tx_dma_sending_size = 0;
		DMA_ITConfig(UART_TX_DMA_STREAM, DMA_IT_TC, DISABLE);
		return;
	}

	// bufferに入っているデータをbufferの終わりまでで分割してDMAにセットする
	if (tx_buffer_tail < tx_buffer_head) {
		tx_dma_sending_size = tx_buffer_head - tx_buffer_tail;
	}
	else {
		tx_dma_sending_size = UART_TX_BUFFER_SIZE - tx_buffer_tail;
	}
	send_dma(tx_buffer + tx_buffer_tail, tx_dma_sending_size);
}
