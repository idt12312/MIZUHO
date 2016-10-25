#include "stm32f4xx_conf.h"
#include "tick.h"

static uint32_t tickCnt = 0;

void Tick_init()
{
	if (SysTick_Config(SYSTICK_PERIOD)) {
		while(1);
	}
	SysTick_CLKSourceConfig(SysTick_CLKSource_HCLK);
}

void Tick_update()
{
	tickCnt++;
}

uint32_t Tick_get()
{
	return tickCnt;
}

void Tick_wait(uint32_t ms)
{
	uint32_t start = Tick_get();
	while(Tick_get() - start < ms);
}
