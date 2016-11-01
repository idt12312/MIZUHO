/*
 * mpu6500.c
 *
 *  Created on: 2016/10/02
 *      Author: idt12312
 */

#include <string.h>
#include <math.h>
#include "mpu6500.h"
#include "spi.h"


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

#define GYRO_OFFSET_CNT 100

static float gyro_z_offset = 0.0;


static void spi_write_reg(uint8_t reg_addr, uint8_t data)
{
	Spi_cs_assert();

	uint8_t send_data[2];
	send_data[0] = reg_addr & 0x7F;  // アドレスはwriteに指定
	send_data[1] = data;

	uint8_t dummy[2];
	Spi_start_xchg(send_data, dummy, 2);

	Spi_cs_dessert();
}

static void spi_read_reg(uint8_t reg_addr, uint8_t *buffer, size_t size)
{
	Spi_cs_assert();

	// 先頭バイトで読み取るレジスタのアドレスを指定するので
	// サイズはsize+1とする
	// Spi_start_xchg, memcpyにおいても同様
	uint8_t send_data[size+1];
	uint8_t rcv_data[size+1];
	send_data[0] = reg_addr | 0x80; // アドレスはreadに指定

	Spi_start_xchg(send_data, rcv_data, size+1);
	Spi_block(1);

	memcpy(buffer, rcv_data+1, size);

	Spi_cs_dessert();
}

void MPU6500_init()
{
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

}


float MPU6500_read_gyro_z()
{
	uint8_t buf[2];
	spi_read_reg(MPU6500_RA_GYRO_ZOUT_H, buf, 2);
	const int16_t gyro_z = (int16_t)(((uint16_t)buf[0] << 8) | ((uint16_t)buf[1]));

	// /GYRO_FACTORで[deg/s]
	// /180*PIで[rad/s]
	const float omega = (float)gyro_z / GYRO_FACTOR / 180.0f * M_PI;

	return omega - gyro_z_offset;
}


void MPU6500_calib_offset()
{
	float sum = 0.0;
	for (int i=0;i<GYRO_OFFSET_CNT;i++) {
		sum += MPU6500_read_gyro_z();
	}
	gyro_z_offset = sum / GYRO_OFFSET_CNT;
}
