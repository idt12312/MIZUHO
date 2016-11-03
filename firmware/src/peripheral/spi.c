/*
 * spi.c
 *
 *  Created on: 2016/11/01
 *      Author: idt12312
 */


#include "stm32f4xx_conf.h"
#include "spi.h"

#include "FreeRTOS.h"
#include "task.h"
#include "semphr.h"


#define SPI_DEICE					SPI1
#define SPI_RCC_ENABLE()			RCC_APB2PeriphClockCmd(RCC_APB2Periph_SPI1, ENABLE)
// SPIのSCKのクロックの設定
// SPIのバス(APB1,APB2)のクロックを何分周(2,4,8,16,32,64,128,256)するかで指定
#define SPI_BAUD_PRESCALER			SPI_BaudRatePrescaler_64

#define SPI_PORT					GPIOB
#define SPI_PORT_RCC_ENABLE()		RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE)
#define SPI_MOSI_PIN				GPIO_Pin_5
#define SPI_MISO_PIN				GPIO_Pin_4
#define SPI_SCK_PIN					GPIO_Pin_3
#define SPI_MOSI_PIN_AF_CONFIG() 	GPIO_PinAFConfig(SPI_PORT, GPIO_PinSource5, GPIO_AF_SPI1)
#define SPI_MISO_PIN_AF_CONFIG() 	GPIO_PinAFConfig(SPI_PORT, GPIO_PinSource4, GPIO_AF_SPI1)
#define SPI_SCK_PIN_AF_CONFIG() 	GPIO_PinAFConfig(SPI_PORT, GPIO_PinSource3, GPIO_AF_SPI1)

#define SPI_CS_PORT					GPIOC
#define SPI_CS_PIN					GPIO_Pin_12
#define SPI_CS_PORT_RCC_ENABLE()	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC, ENABLE)

#define SPI_TX_DMA_STREAM			DMA2_Stream3
#define SPI_RX_DMA_STREAM			DMA2_Stream2
#define SPI_DMA_CHANNEL				DMA_Channel_3

#define SPI_DMA_RCC_ENABLE()		RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_DMA2, ENABLE)
#define SPI_RX_DMA_IRQ_CHANNEL		DMA2_Stream2_IRQn
#define SPI_RX_DMA_IRQ_PRIORITY		6

static bool	running;
static xSemaphoreHandle spi_dma_complete_semaphore;


void Spi_init()
{
	SPI_PORT_RCC_ENABLE();

	GPIO_InitTypeDef  GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Pin =  SPI_MOSI_PIN | SPI_MISO_PIN | SPI_SCK_PIN;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_Init(SPI_PORT, &GPIO_InitStructure);

	SPI_MOSI_PIN_AF_CONFIG();
	SPI_MISO_PIN_AF_CONFIG();
	SPI_SCK_PIN_AF_CONFIG();


	SPI_CS_PORT_RCC_ENABLE();
	GPIO_InitStructure.GPIO_Pin =  SPI_CS_PIN;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_Init(SPI_CS_PORT, &GPIO_InitStructure);

	Spi_cs_dessert();


	SPI_RCC_ENABLE();

	// SCKはアイドル時に1, 後縁で読み取り
	SPI_InitTypeDef  SPI_InitStructure;
	SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;
	SPI_InitStructure.SPI_Mode = SPI_Mode_Master;
	SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b;
	SPI_InitStructure.SPI_CPOL = SPI_CPOL_High;
	SPI_InitStructure.SPI_CPHA = SPI_CPHA_2Edge;
	SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;
	SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BAUD_PRESCALER;
	SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;
	SPI_Init(SPI_DEICE, &SPI_InitStructure);

	SPI_Cmd(SPI_DEICE, ENABLE);

	spi_dma_complete_semaphore = xSemaphoreCreateBinary();
	running = false;
}


void Spi_cs_assert()
{
	GPIO_ResetBits(SPI_CS_PORT, SPI_CS_PIN);
}


void Spi_cs_dessert()
{
	GPIO_SetBits(SPI_CS_PORT, SPI_CS_PIN);
}


void Spi_start_xchg(const uint8_t *tx_buf, uint8_t *rx_buf, size_t size)
{
	// RTOSが起動していない時はbusy waitで通信を待つ
	if (xTaskGetSchedulerState() == taskSCHEDULER_NOT_STARTED) {
		for (size_t i=0;i<size;i++) {
			while (SPI_I2S_GetFlagStatus(SPI_DEICE, SPI_I2S_FLAG_TXE) == RESET);
			SPI_I2S_SendData(SPI_DEICE, tx_buf[i]);

			while (SPI_I2S_GetFlagStatus(SPI_DEICE, SPI_I2S_FLAG_RXNE) == RESET);
			rx_buf[i] = SPI_I2S_ReceiveData(SPI_DEICE);
		}
		return;
	}

	// RTOSが起動している時はセマフォを使う
	SPI_DMA_RCC_ENABLE();
	DMA_DeInit(SPI_TX_DMA_STREAM);
	DMA_DeInit(SPI_RX_DMA_STREAM);

	DMA_InitTypeDef DMA_InitStructure;
	DMA_InitStructure.DMA_BufferSize        = size;
	DMA_InitStructure.DMA_Channel           = SPI_DMA_CHANNEL;
	DMA_InitStructure.DMA_DIR               = DMA_DIR_MemoryToPeripheral;
	DMA_InitStructure.DMA_FIFOMode          = DMA_FIFOMode_Disable;
	DMA_InitStructure.DMA_FIFOThreshold     = DMA_FIFOThreshold_HalfFull;
	DMA_InitStructure.DMA_Memory0BaseAddr   = (uint32_t)tx_buf;
	DMA_InitStructure.DMA_MemoryBurst       = DMA_MemoryBurst_Single;
	DMA_InitStructure.DMA_MemoryDataSize    = DMA_MemoryDataSize_Byte;
	DMA_InitStructure.DMA_MemoryInc         = DMA_MemoryInc_Enable;
	DMA_InitStructure.DMA_Mode              = DMA_Mode_Normal;
	DMA_InitStructure.DMA_PeripheralBaseAddr= (uint32_t)(&(SPI_DEICE->DR));
	DMA_InitStructure.DMA_PeripheralBurst   = DMA_PeripheralBurst_Single;
	DMA_InitStructure.DMA_PeripheralDataSize= DMA_MemoryDataSize_Byte;
	DMA_InitStructure.DMA_PeripheralInc     = DMA_PeripheralInc_Disable;
	DMA_InitStructure.DMA_Priority          = DMA_Priority_Medium;
	DMA_Init(SPI_TX_DMA_STREAM, &DMA_InitStructure);

	DMA_InitStructure.DMA_DIR               = DMA_DIR_PeripheralToMemory;
	DMA_InitStructure.DMA_Memory0BaseAddr   = (uint32_t)rx_buf;
	DMA_Init(SPI_RX_DMA_STREAM, &DMA_InitStructure);


	// DMAの送信完了割り込みを有効化
	NVIC_InitTypeDef NVIC_InitStructure;
	NVIC_InitStructure.NVIC_IRQChannel = SPI_RX_DMA_IRQ_CHANNEL;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = SPI_RX_DMA_IRQ_PRIORITY;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);

	DMA_ITConfig(SPI_RX_DMA_STREAM, DMA_IT_TC, ENABLE);

	running = true;

	SPI_I2S_DMACmd(SPI_DEICE, SPI_I2S_DMAReq_Rx, ENABLE);
	SPI_I2S_DMACmd(SPI_DEICE, SPI_I2S_DMAReq_Tx, ENABLE);

	DMA_Cmd(SPI_RX_DMA_STREAM, ENABLE);
	DMA_Cmd(SPI_TX_DMA_STREAM, ENABLE);
}


bool Spi_busy()
{
	return running;
}


void Spi_block(uint32_t block_time)
{
	if (xTaskGetSchedulerState() == taskSCHEDULER_NOT_STARTED) {
		return ;
	}
	xSemaphoreTake(spi_dma_complete_semaphore, block_time);
}


void Spi_dma_complete_isr()
{
	running = false;

	DMA_ITConfig(SPI_RX_DMA_STREAM, DMA_IT_TE, DISABLE);

	// セマフォをgiveしてコンテキストスイッチ
	portBASE_TYPE xHigherPriorityTaskWoken;
	xSemaphoreGiveFromISR(spi_dma_complete_semaphore, &xHigherPriorityTaskWoken);
	portEND_SWITCHING_ISR(xHigherPriorityTaskWoken);
}
