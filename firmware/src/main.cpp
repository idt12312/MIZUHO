#include "stm32f4xx.h"
#include <cstdio>

#include "FreeRTOS.h"
#include "task.h"

#include "uart.h"
#include "enc.h"
#include "motor.h"
#include "spi.h"
#include "mpu6500.h"
#include "irsensor.h"
#include "motor.h"
#include "led_button.h"
#include "battery_monitor.h"

#include "BlinkLedTask.h"
#include "BatteryMonitorTask.h"
#include "WallDetectTask.h"
#include "MotorControlTask.h"
#include "MotionControlTask.h"
#include "TopTask.h"
#include "ButtonTask.h"

static BlinkLedTask blink_led_task(LED_5, 500);
static BatteryMonitorTask battery_monitor_task;
static WallDetectTask wall_detect_task;
static MotorControlTask motor_control_task;
static MotionControlTask motion_control_task;
static TopTask top_task;
static ButtonTask button_task;

void* __dso_handle = 0;

static void peripheral_init()
{
	// 割り込み優先度をpre-emption priorityに4bit割り当てる
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_4);

	Led_init();
	Button_init();
	Uart_init();
	Enc_init();
	Spi_init();
	MPU6500_init();
	BatteryMonitor_init();

	// TODO:irsensro -> motorの順に初期化しないとmotorが動かなくなる
	IrSensor_init();
	Motor_init();

	// デバッガによって処理が停止したとき
	// モータのタイマ, ADCのトリガとなるタイマ, LEDのPWM用タイマを停止する
	DBGMCU_Config(DBGMCU_STOP, ENABLE);
	DBGMCU_APB1PeriphConfig(DBGMCU_TIM1_STOP, ENABLE);
	DBGMCU_APB1PeriphConfig(DBGMCU_TIM2_STOP, ENABLE);
	DBGMCU_APB2PeriphConfig(DBGMCU_TIM3_STOP, ENABLE);
}


static void tasks_init()
{
	blink_led_task.create_task();
	battery_monitor_task.create_task();
	wall_detect_task.create_task();
	motor_control_task.create_task();
	motion_control_task.create_task();
	top_task.create_task();
	button_task.create_task();

	motion_control_task.set_motor_control_task(&motor_control_task);
	motion_control_task.set_wall_detec_task(&wall_detect_task);

	top_task.set_motor_control_task(&motor_control_task);
	top_task.set_motion_control_task(&motion_control_task);
	top_task.set_wall_detect_task(&wall_detect_task);

	button_task.set_motor_control_task(&motor_control_task);
}


int main()
{
	peripheral_init();
	std::printf("peripheral initialization is completed\n");

	tasks_init();

	std::printf("Start FreeRTOS Scheduler\n");
	vTaskStartScheduler();

	while (1);
}
