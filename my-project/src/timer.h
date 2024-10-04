#ifndef __TIMER__
#define __TIMER__

#include <libopencm3/stm32/timer.h>
#include <libopencm3/cm3/nvic.h>
#include <libopencm3/stm32/gpio.h>
#include <libopencm3/stm32/rcc.h>

void init_timer2(void);
void init_timer3(void);


#endif