#ifndef TICK_H_
#define TICK_H_

#ifdef __cplusplus
 extern "C" {
#endif

#include <stdint.h>

#define TICK_SEC 1000 //(TICK_SEC)count is sec
#define SYSTICK_PERIOD SystemCoreClock / TICK_SEC

void Tick_init();
void Tick_update();
uint32_t Tick_get();
void Tick_wait(uint32_t ms);

#ifdef __cplusplus
 }
#endif

#endif /* TICK_H_ */
