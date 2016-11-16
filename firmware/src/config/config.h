/*
 * config.h
 *
 *  Created on: 2016/10/31
 *      Author: idt12312
 */

#ifndef CONFIG_H_
#define CONFIG_H_


/************************************************
 * デバッグ用の設定
 ***********************************************/
#define ODOMETRY_USE_GYRO				1
#define PRINT_WALL_SENSOR_RAW_VALUE		0
#define ENABLE_BATTERY_MONITOR_ABORT	1
#define ENABLE_BATTERY_MONITOR_PRINT	1


/************************************************
 * マシンの物理的パラメータ
 ***********************************************/
#define MACHINE_WHEEL_RADIUS	0.01237f
// 中心からタイヤまでの距離
#define MACHINE_TREAD_WIDTH 	0.03289f
#define MACHINE_ENC_CNT_PER_ROT	(1024*4*4)
#define MACHINE_MOTOR_VOLTAGE	7.6f

#define MACHINE_AXLE_TO_TAIL	0.034f
#define MACHINE_TAIL_COMP_LEN	0.001f


/************************************************
 * マシンの走行パラメータ
 ***********************************************/
#define STRAIGHT_DEFAULT_VELOCITY		0.1f
#define STRAIGHT_MAX_VELOCITY			0.2f
#define STRAIGHT_ACCERALATION			1.0f
#define PIVOT_ROTATION_VELOCITY			3.0f
#define PIVOT_ROTATION_ACCERALATION		15.0f
#define SLALOM_VELOCITY					0.1f
#define SLALOM_ROTATION_VELOCITY		1.5f
#define SLALOM_ROTATION_ACCERALATION	10.0f


/************************************************
 * フィールドの物理的パラメータ
 ***********************************************/
#define BLOCK_SIZE	0.18f


/************************************************
 * バッテリー監視タスクの設定
 ***********************************************/
#define BATTERY_MONITOR_TASK_PRIORITY	4
#define BATTERY_MONITOR_TASK_PERIOD		2000
#define BATTERY_MONITOR_TASK_STACK_SIZE 256
#define BATTERY_MONITOR_AVE_CNT			5
#define BATTERY_MONITOR_ALERT_THREHOLD	7.3f
#define ADC_TO_BATTERY_VOLTAGE(x)		((float)x / 4096.0f * 3.3f * 3.0f)


/************************************************
 * 壁検出タスクの設定
 ***********************************************/
#define WALL_DETECT_TASK_PRIORITY		2
#define WALL_DETECT_TASK_PERIOD			5
#define WALL_DETECT_TASK_STACK_SIZE		1024
#define WALL_DETECT_RL_THREHOLD			3300
#define WALL_DETECT_FRONT_THRETHOLD_NOSIDE		3500
#define WALL_DETECT_FRONT_THRETHOLD_WITH_SIDE		3600
#define WALL_DETECT_CNT					4


/************************************************
 * モータ制御(姿勢制御)の設定
 ***********************************************/
#define MOTOR_CONTROL_TASK_PRIORITY		1
#define MOTOR_CONTROL_TASK_PERIOD		1
#define MOTOR_CONTROL_TASK_PERIOD_SEC	((float)MOTOR_CONTROL_TASK_PERIOD/1000.0f)
#define MOTOR_CONTROL_TASK_STACK_SIZE	512
#define MOTOR_CONTROL_P_GAIN			1.0f
#define MOTOR_CONTROL_I_GAIN			0.01f
#define MOTOR_CONTROL_D_GAIN			0.0001f


/************************************************
 * 軌道追従制御の設定
 ***********************************************/
#define TRACKING_CONTROL_TASK_PRIORITY		2
#define TRACKING_CONTROL_TASK_PERIOD		5
#define TRACKING_CONTROL_TASK_PERIOD_SEC	((float)TRACKING_CONTROL_TASK_PERIOD/1000.0f)
#define TRACKING_CONTROL_TASK_STACK_SIZE	512
#define TRACKING_CONTROL_OMEGA_FORWARD_GAIN	1.05f
#define TRACKING_CONTROL_X_P_GAIN			80.0f
#define TRACKING_CONTROL_X_D_GAIN			1.0f
#define TRACKING_CONTROL_Y_P_GAIN			60.0f
#define TRACKING_CONTROL_Y_D_GAIN			0.05f
#define TRACKING_CONTROL_ANGLE_P_GAIN		3.0f
#define TRACKING_CONTROL_ANGLE_I_GAIN		0.05f
#define TRACKING_CONTROL_ANGLE_D_GAIN		0.0f
#define TRACKING_CONTROL_WALL_P_GAIN		(0.8f/1000.0f/1000.0f)


/************************************************
 * Top Taskの設定
 ***********************************************/
#define TOP_TASK_PRIORITY		3
#define TOP_TASK_STACK_SIZE		1024



#endif /* CONFIG_H_ */
