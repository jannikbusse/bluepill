#include "timer.h"

void init_timer2() {
    // Enable the clock for TIM2
    rcc_periph_clock_enable(RCC_TIM2);
    // Enable the TIM2 interrupt in the NVIC
    nvic_enable_irq(NVIC_TIM2_IRQ);

    // Reset the timer configuration
    // timer_reset(TIM2);
    // Set the prescaler to 7200 (PSC + 1), which divides the 72 MHz system clock by 7200
    timer_set_prescaler(TIM2, 0);
    // Set the auto-reload value to 10000, for a 1-second interval (10kHz / 10000 = 1Hz)
    timer_set_period(TIM2, 543 - 1);
    // Enable the update interrupt (this will fire an interrupt when the counter resets)
    timer_enable_irq(TIM2, TIM_DIER_UIE);
    timer_enable_counter(TIM2);
}

void init_timer3() {
    // Enable the clock for TIM3
    rcc_periph_clock_enable(RCC_TIM3);
    
    // Enable the TIM3 interrupt in the NVIC
    nvic_enable_irq(NVIC_TIM3_IRQ);

    // Reset the timer configuration
    // timer_reset(TIM3);

    // Set the prescaler (you can adjust this value as needed)
    timer_set_prescaler(TIM3, 1500 - 1); // 10kHz (72MHz / 7200)

    // Set the auto-reload value (you can adjust this value as needed)
    timer_set_period(TIM3, 10000 - 1); // 1Hz (10kHz / 10000)

    // Enable the update interrupt
    timer_enable_irq(TIM3, TIM_DIER_UIE);

    // Start the timer
    timer_enable_counter(TIM3);
}