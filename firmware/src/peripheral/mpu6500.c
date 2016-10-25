/*
 * mpu6500.c
 *
 *  Created on: 2016/10/02
 *      Author: idt12312
 */

#include <math.h>
#include "stm32f4xx_conf.h"
#include "mpu6500.h"

#define SPI_DEICE				SPI1
#define SPI_RCC_ENABLE()		RCC_APB2PeriphClockCmd(RCC_APB2Periph_SPI1, ENABLE)
// SPIのSCKのクロックの設定
// SPIのバス(APB1,APB2)のクロックを何分周(2,4,8,16,32,64,128,256)するかで指定
#define SPI_BAUD_PRESCALER		SPI_BaudRatePrescaler_64

#define SPI_PORT				GPIOB
#define SPI_PORT_RCC_ENABLE()	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE)
#define SPI_MOSI_PIN			GPIO_Pin_5
#define SPI_MISO_PIN			GPIO_Pin_4
#define SPI_SCK_PIN				GPIO_Pin_3
#define SPI_MOSI_PIN_AF_CONFIG() GPIO_PinAFConfig(SPI_PORT, GPIO_PinSource5, GPIO_AF_SPI1)
#define SPI_MISO_PIN_AF_CONFIG() GPIO_PinAFConfig(SPI_PORT, GPIO_PinSource4, GPIO_AF_SPI1)
#define SPI_SCK_PIN_AF_CONFIG() GPIO_PinAFConfig(SPI_PORT, GPIO_PinSource3, GPIO_AF_SPI1)

#define SPI_CS_PORT				GPIOC
#define SPI_CS_PIN				GPIO_Pin_12
#define SPI_CS_PORT_RCC_ENABLE()	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC, ENABLE)


// MPU6500のレジスタマップ
#define MPU6500_RA_SELF_TEST_X_GYRO	0x00
#define MPU6500_RA_SELF_TEST_Y_GYRO	0x01
#define MPU6500_RA_SELF_TEST_Z_GYRO	0x02
#define MPU6500_RA_SELF_TEST_X_ACCEL	0x0D
#define MPU6500_RA_SELF_TEST_Y_ACCEL	0x0E
#define MPU6500_RA_SELF_TEST_Z_ACCEL	0x0F
#define MPU6500_RA_XG_OFFSET_H		0x13
#define MPU6500_RA_XG_OFFSET_L		0x14
#define MPU6500_RA_YG_OFFSET_H		0x15
#define MPU6500_RA_YG_OFFSET_L		0x16
#define MPU6500_RA_ZG_OFFSET_H		0x17
#define MPU6500_RA_ZG_OFFSET_L		0x18
#define MPU6500_RA_SMPLRT_DIV		0x19
#define MPU6500_RA_CONFIG			0x1A
#define MPU6500_RA_GYRO_CONFIG		0x1B
#define MPU6500_RA_ACCEL_CONFIG		0x1C
#define MPU6500_RA_ACCEL_CONFIG2	0x1D
#define MPU6500_RA_LP_ACCEL_ODR		0x1E
#define MPU6500_RA_WOM_THR			0x1F
#define MPU6500_RA_FIFO_EN			0x23
// ...
// 外部I2C周りは省略
// ...
#define MPU6500_RA_INT_PIN_CFG		0x37
#define MPU6500_RA_INT_ENABLE		0x38
#define MPU6500_RA_INT_STATUS		0x3A
#define MPU6500_RA_ACCEL_XOUT_H		0x3B
#define MPU6500_RA_ACCEL_XOUT_L		0x3C
#define MPU6500_RA_ACCEL_YOUT_H		0x3D
#define MPU6500_RA_ACCEL_YOUT_L		0x3E
#define MPU6500_RA_ACCEL_ZOUT_H		0x3F
#define MPU6500_RA_ACCEL_ZOUT_L		0x40
#define MPU6500_RA_TEMP_OUT_H		0x41
#define MPU6500_RA_TEMP_OUT_L		0x42
#define MPU6500_RA_GYRO_XOUT_H		0x43
#define MPU6500_RA_GYRO_XOUT_L		0x44
#define MPU6500_RA_GYRO_YOUT_H		0x45
#define MPU6500_RA_GYRO_YOUT_L		0x46
#define MPU6500_RA_GYRO_ZOUT_H		0x47
#define MPU6500_RA_GYRO_ZOUT_L		0x48
// ...
// 外部I2C周りは省略
// ...
#define MPU6500_RA_SIGNAL_PATH_RESET	0x68
#define MPU6500_RA_MOT_DETECT_CTRL		0x69
#define MPU6500_RA_USER_CTRL		0x6A
#define MPU6500_RA_PWR_MGMT_1		0x6B
#define MPU6500_RA_PWR_MGMT_2		0x6C
#define MPU6500_RA_FIFO_COUNTH		0x72
#define MPU6500_RA_FIFO_COUNTL			0x73
#define MPU6500_RA_FIFO_R_W			0x74
#define MPU6500_RA_WHO_AM_I			0x75
#define MPU6500_RA_XA_OFFSET_H		0x77
#define MPU6500_RA_XA_OFFSET_L		0x78
#define MPU6500_RA_YA_OFFSET_H		0x7A
#define MPU6500_RA_YA_OFFSET_L		0x7B
#define MPU6500_RA_ZA_OFFSET_H		0x7D
#define MPU6500_RA_ZA_OFFSET_L		0x7E

#define MPU6500_DEVICE_ID			0x70
// ジャイロのゲインがフルスケールで250dpsの時の値
#define GYRO_FACTOR  131.0f


static void spi_init()
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

	GPIO_SetBits(SPI_CS_PORT, SPI_CS_PIN);


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
}

static void spi_cs_assert()
{
	GPIO_ResetBits(SPI_CS_PORT, SPI_CS_PIN);
}

static void spi_cs_dessert()
{
	GPIO_SetBits(SPI_CS_PORT, SPI_CS_PIN);
}
static uint8_t spi_xchg(uint8_t data)
{
	while (SPI_I2S_GetFlagStatus(SPI_DEICE, SPI_I2S_FLAG_TXE) == RESET);
	SPI_I2S_SendData(SPI_DEICE, data);

	while (SPI_I2S_GetFlagStatus(SPI_DEICE, SPI_I2S_FLAG_RXNE) == RESET);
	return SPI_I2S_ReceiveData(SPI_DEICE);
}

static void spi_write_reg(uint8_t reg_addr, uint8_t data)
{
	spi_cs_assert();

	// アドレスはwriteに指定
	spi_xchg(reg_addr & 0x7F);
	spi_xchg(data);

	spi_cs_dessert();
}

static void spi_read_reg(uint8_t reg_addr, uint8_t *buffer, size_t size)
{
	spi_cs_assert();

	// アドレスはreadに指定
	spi_xchg(reg_addr | 0x80);
	for (int i=0;i<size;i++) {
		buffer[i] = spi_xchg(0x00);
	}

	spi_cs_dessert();
}

void MPU6500_init()
{
	spi_init();


	// リセット
	spi_write_reg(MPU6500_RA_PWR_MGMT_1, 0x80);
	for (volatile int i=0;i<100000;i++);

	// MPU6500のデバイスIDをチェック
	uint8_t device_id;
	spi_read_reg(MPU6500_RA_WHO_AM_I, &device_id, 1);
	if (device_id != MPU6500_DEVICE_ID) {
		//error
		while (1);
	}

	// スリープ解除
	spi_write_reg(MPU6500_RA_PWR_MGMT_1, 0x00);

	// DLPF_CFG = 0 : GyroのLPFを無効化
	// FIFOは使わない
	spi_write_reg(MPU6500_RA_CONFIG, 0x00);

	// Gyroのフルスケールを+-250dpsに設定
	// FCHOICE_B = b00 : Band=8600Hz, Delay=0.064ms, Fs=32kHz
	spi_write_reg(MPU6500_RA_GYRO_CONFIG, 0x00);

	// TODO:オフセットを計算

}

float MPU6500_read_gyro_z()
{
	uint8_t buf[2];
	spi_read_reg(MPU6500_RA_GYRO_ZOUT_H, buf, 2);
	const int16_t gyro_z = (int16_t)(((uint16_t)buf[0] << 8) | ((uint16_t)buf[1]));

	// /GYRO_FACTORで[deg/s]
	// /180*PIで[rad/s]
	const float omega = (float)gyro_z / GYRO_FACTOR / 180.0f * M_PI;

	return omega;
}

