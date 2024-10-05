#define STM32F1 1
#define FIXPOINT_DECIMAL_PLACES 10000000

#include <libopencm3/stm32/rcc.h>
#include <libopencm3/stm32/spi.h>
#include <libopencm3/stm32/gpio.h>
#include <libopencm3/stm32/flash.h>
#include "timer.h"
#include "dac.h"
#include <math.h>
#include"osc.h"
#include "music.h"
#include "input.h"

volatile bool audioSampleEventInterruptFlag = false; //true if a interrupt occurs

#define CLOCK_SPEED_HZ 	72000000f
#define SAMPLE_FREQ_HZ	44100
#define s_PER_TICK_FIX	227
#define uS_PER_TICK 	22.7


void init_clock(void);
void init_clock()
{
	rcc_clock_setup_pll(&rcc_hse_configs[RCC_CLOCK_HSE8_72MHZ]);
}

void tim2_isr(void) {
    // Check if the update interrupt flag is set
    if (timer_get_flag(TIM2, TIM_SR_UIF)) {
        // Clear the interrupt flag
        timer_clear_flag(TIM2, TIM_SR_UIF);
		audioSampleEventInterruptFlag = true;
		gpio_toggle(GPIOC, GPIO13);
    }
}

void tim3_isr(void) {
    if (timer_get_flag(TIM3, TIM_SR_UIF)) { // Check if update interrupt flag is set
        timer_clear_flag(TIM3, TIM_SR_UIF); // Clear the interrupt flag
		music_advance_note();
		// gpio_toggle(GPIOC, GPIO13);  // Assuming an LED is connected to PC13

	}
}

int main(void) {

	init_clock();
	rcc_periph_clock_enable(RCC_GPIOC);
	rcc_periph_clock_enable(RCC_GPIOB);
	rcc_periph_clock_enable(RCC_GPIOA);

	// init_input();

	init_dac();
	init_music(s_PER_TICK_FIX);
	set_volume(0.0145);
	//set mode to output
	gpio_set_mode(GPIOC, GPIO_MODE_OUTPUT_2_MHZ, GPIO_CNF_OUTPUT_PUSHPULL, GPIO13);
	gpio_set_mode(GPIOB, GPIO_MODE_OUTPUT_2_MHZ, GPIO_CNF_OUTPUT_PUSHPULL, GPIO12);

	//Just wait
	for(int i = 0; i < 10000000 ; i++)
	{
		__asm volatile ("nop");
	}


	init_timer2();
	// init_timer3();

	while(1)
	{
		if(audioSampleEventInterruptFlag)
		{
			//pass current value to analog out
			// float v = ((sinf(t)+1.0f)/2.0f) ;
			// if(inputPressed(INPUT1))
			// {
			// 	music_play(300);
			// }
			// else if(inputPressed(INPUT2))
			// {
			// 	music_play(600);
			// }
			// else
			// {
			// 	music_play(0);
			// }
			music_play(300);
			//handle audio generation of new element
			audioSampleEventInterruptFlag = false;
		}
		gpio_toggle(GPIOB, GPIO12);

	}
}	
