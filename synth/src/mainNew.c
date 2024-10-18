#define STM32F4 1


#include <libopencm3/stm32/rcc.h>
#include <libopencm3/stm32/spi.h>
#include <libopencm3/stm32/gpio.h>
#include <libopencm3/cm3/scb.h>
#include <libopencm3/stm32/flash.h>
#include "timer.h"

volatile float res = 0;
void tim2_isr(void) {
    // Check if the update interrupt flag is set
    if (timer_get_flag(TIM2, TIM_SR_UIF)) {
        // Clear the interrupt flag
        timer_clear_flag(TIM2, TIM_SR_UIF);
    }
}

void delay(void) {
    for (int i = 0; i < 10000000; i++) {
        __asm__("nop");
    }
}

void init_clock(void)
{
    rcc_clock_setup_pll(&rcc_hse_8mhz_3v3[RCC_CLOCK_3V3_168MHZ]);
}

int main(void) {
	
	init_clock();
	rcc_periph_clock_enable(RCC_GPIOC);
	rcc_periph_clock_enable(RCC_GPIOB);
	//set mode to output
    gpio_mode_setup(GPIOB, GPIO_MODE_OUTPUT, GPIO_PUPD_NONE, GPIO2);
	init_timer2();
   	while (1) {
        // Toggle the LED on PA5
		res = 1.f;
        gpio_toggle(GPIOB, GPIO2);
		for(int i = 0; i < 999; i ++)
		{
			res = res * 1.214214f;
		}
    }

}	
