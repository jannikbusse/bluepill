#include "timer.h"

void init_timer2() {
    // Enable the clock for TIM2
    rcc_periph_clock_enable(RCC_TIM2);
    // Enable the TIM2 interrupt in the NVIC
    nvic_enable_irq(NVIC_TIM2_IRQ);

    timer_set_prescaler(TIM2, 0);
    
    // Set the auto-reload value for 1 Hz (1000 ms)
    timer_set_period(TIM2, TIMER2_AUTO_RELOAD); // 999 + 1 = 1000
    // Enable the update interrupt (this will fire an interrupt when the counter resets)
    timer_enable_irq(TIM2, TIM_DIER_UIE);
    
}


