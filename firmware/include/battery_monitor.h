/*
 * battery_alert.h
 *
 *  Created on: 2016/10/25
 *      Author: idt12312
 */

#ifndef BATTERY_MONITOR_H_
#define BATTERY_MONITOR_H_

#ifdef	__cplusplus
extern "C"
{
#endif

#include <stdint.h>

void BatteryMonitor_init();
uint16_t BatteryMonitor_read();

#ifdef	__cplusplus
}
#endif

#endif /* BATTERY_MONITOR_H_ */
