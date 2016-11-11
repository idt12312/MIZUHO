/*
 * config.h
 *
 *  Created on: 2016/10/31
 *      Author: idt12312
 */

#ifndef CONFIG_H_
#define CONFIG_H_


/************************************************
 * マシンの物理的パラメータ
 ***********************************************/
#define WHEEL_RADIUS	0.01237f
// 中心からタイヤまでの距離
#define TREAD_WIDTH 	0.03289f
#define ENC_CNT_PER_ROT	(1024*4*4)
#define MOTOR_VOLTAGE	7.6f


/************************************************
 * フィールドの物理的パラメータ
 ***********************************************/
#define BLOCK_SIZE	0.18f


/************************************************
 * バッテリー監視タスクの設定
 ***********************************************/
#define BATTERY_MONITOR_TASK_PRIORITY	3
#define BATTERY_MONITOR_TASK_PERIOD		2000
#define BATTERY_MONITOR_TASK_STACK_SIZE 256
#define BATTERY_MONITOR_AVE_CNT			5
#define BATTERY_MONITOR_ALERT_THREHOLD	7.4f
#define ADC_TO_BATTERY_VOLTAGE(x)		((float)x / 4096.0f * 3.3f * 3.0f)


/************************************************
 * 壁検出タスクの設定
 ***********************************************/
#define WALL_DETECT_TASK_PRIORITY		2
#define WALL_DETECT_TASK_PERIOD			5
#define WALL_DETECT_TASK_STACK_SIZE		1024
#define WALL_DETECT_RL_THREHOLD			3400
#define WALL_DETECT_FRONT_THRETHOLD		3300
#define WALL_DETECT_CNT					4


/************************************************
 * モータ制御(姿勢制御)タスクの設定
 ***********************************************/
#define MOTOR_CONTROL_TASK_PRIORITY		1
#define MOTOR_CONTROL_TASK_PERIOD		1
#define MOTOR_CONTROL_TASK_STACK_SIZE	512


/************************************************
 * 軌道追従タスクの設定
 ***********************************************/
#define TRACKING_CONTROL_TASK_PRIORITY		2
#define TRACKING_CONTROL_TASK_PERIOD		5
#define TRACKING_CONTROL_TASK_STACK_SIZE	512


#endif /* CONFIG_H_ */
