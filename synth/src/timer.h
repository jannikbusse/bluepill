#ifndef __TIMER__
#define __TIMER__

#include <libopencm3/stm32/timer.h>
#include <libopencm3/cm3/nvic.h>
#include <libopencm3/stm32/gpio.h>
#include <libopencm3/stm32/rcc.h>

#define TIMER2_AUTO_RELOAD (89 -1)

void init_timer2(void);
void init_timer3(void);

inline void __attribute__((always_inline)) enableTimer2(void)
{
    timer_enable_counter(TIM2);
}

inline void __attribute__((always_inline)) disableTimer2(void)
{
    timer_disable_counter(TIM2);
    timer_set_counter(TIM2, TIMER2_AUTO_RELOAD);
    
}


#endif