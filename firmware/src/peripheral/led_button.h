/*
 * led_button.h
 *
 *  Created on: 2016/10/23
 *      Author: idt12312
 */

#ifndef LED_BUTTON_H_
#define LED_BUTTON_H_

#ifdef	__cplusplus
extern "C"
{
#endif

#include <stdbool.h>
#include <stdint.h>

#define LED_1	0x01
#define LED_2	0x02
#define LED_3	0x04
#define LED_4	0x08
#define LED_5	0x10
#define LED_1234	0x0f
#define LED_ALL	0x1f


void Led_init();
void Led_on(uint8_t pin);
void Led_off(uint8_t pin);
void Led_toggle(uint8_t pin);

void Button_init();
bool ButtonL_get();
bool ButtonR_get();

#ifdef	__cplusplus
}
#endif

#endif /* LED_BUTTON_H_ */
