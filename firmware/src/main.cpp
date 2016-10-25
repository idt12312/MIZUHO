#include "stm32f4xx.h"
#include <stdio.h>

#include "FreeRTOS.h"
#include "task.h"

#include "uart.h"
#include "enc.h"
#include "motor.h"
#include "mpu6500.h"
#include "irsensor.h"
#include "tick.h"
#include "motor.h"
#include "led_button.h"
#include "battery_monitor.h"


extern "C" {
void vApplicationStackOverflowHook( TaskHandle_t xTask, char *pcTaskName );
void vApplicationMallocFailedHook();
void vApplicationIdleHook();
void vApplicationTickHook();
}


void vApplicationStackOverflowHook( TaskHandle_t xTask, char *pcTaskName )
{
	printf("\n[ERROR] Stack Overflow %s\n", pcTaskName);
	while(1);
}

void vApplicationMallocFailedHook()
{
	printf("\n[ERROR] Malloc Failed\n");
}

void vApplicationIdleHook()
{

}

void vApplicationTickHook()
{
	Tick_update();
}


static void test_led()
{
	while (1) {
		for (int i=0;i<5;i++) {
			Led_on(1<<i);
			Tick_wait(500);
		}
		for (int i=0;i<5;i++) {
			Led_off(1<<i);
			Tick_wait(500);
		}
		for (int i=0;i<5;i++) {
			Led_toggle(1<<i);
			Tick_wait(500);
			Led_toggle(1<<i);
			Tick_wait(500);
		}
	}
}


static void test_button()
{
	while (1) {
		Tick_wait(200);
		printf("L:%d R:%d\n", ButtonL_get(), ButtonR_get());
	}
}


static void test_gyro()
{
	int cnt=0;
	float angle=0;

	while (1) {
		Tick_wait(10);
		float omega = MPU6500_read_gyro_z();
		angle += omega * 0.01;
		cnt++;
		if (cnt > 10) {
			cnt = 0;
			printf("%f %f\n", omega/3.1415*180, angle/3.1415*180);
		}
	}
}


static void test_motor()
{
	float duty[2];
	while (1) {
		duty[0] = duty[1] = 0.0f;
		if (ButtonL_get()) duty[0] = 0.2;
		if (ButtonR_get()) duty[1] = 0.2;
		Motor_set(duty);
		Tick_wait(10);
	}
}


static void test_enc()
{
	int32_t enc_sum[2] = {0};

	while (1) {
		Tick_wait(200);

		int32_t enc[2];
		Enc_read(enc);
		enc_sum[0] += enc[0];
		enc_sum[1] += enc[1];

		printf("%6d %6d %6d %6d\n", enc[0], enc[1], enc_sum[0], enc_sum[1]);
	}
}


static void test_battery_monitor()
{
	while (1) {
		float batt_valtage = (float)BatteryMonitor_read() / 4096.0 * 3.3 * 3;
		printf("%f\n", batt_valtage);
		Tick_wait(300);
	}
}

static void irsensor_dump()
{
	SensorRawData raw;
	IrSensor_start();
	while (IrSensor_busy());
	IrSensor_get(&raw);

	for (size_t i=0;i<raw.size;i++) {
		printf("%4u %4u %4u %4u\n",
				SENSOR1(raw, i),
				SENSOR2(raw, i),
				SENSOR3(raw, i),
				SENSOR4(raw, i));
		Tick_wait(20);
		float batt_valtage = (float)BatteryMonitor_read() / 4096.0 * 3.3 * 3;
		printf("%f\n", batt_valtage);
	}
}


static void peripheral_init()
{
	Led_init();
	Button_init();
	Uart_init();
	Enc_init();
	MPU6500_init();
	BatteryMonitor_init();

	// TODO:irsensro -> motorの順に初期化しないとmotorが動かなくなる
	IrSensor_init();
	Motor_init();

	// デバッガによって処理が停止したとき
	// モータのタイマとADCのトリガとなるタイマを停止する
	DBGMCU_APB1PeriphConfig(DBGMCU_TIM1_STOP, ENABLE);
	DBGMCU_APB1PeriphConfig(DBGMCU_TIM2_STOP, ENABLE);
}


void led_blink_task(void*)
{
	TickType_t last_wake_tick = xTaskGetTickCount();

	while (1)  {
		vTaskDelayUntil(&last_wake_tick, 1000);
		last_wake_tick = xTaskGetTickCount();

		Led_toggle(LED_5);
	}
}

int main()
{
	peripheral_init();
	printf("peripheral initialization is completed\n");

	xTaskCreate(led_blink_task, "LED Blink Task", 64, NULL, 1, NULL);

	vTaskStartScheduler();

	while (1);
}
