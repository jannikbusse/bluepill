#define STM32F1 1


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
volatile bool inputSampleEventInterruptFlag = false; 


#define CLOCK_SPEED_HZ 			72000000f
#define SAMPLE_FREQ_HZ			44100
#define s_PER_TICK_FIX			227
#define uS_PER_TICK 			22.7
#define INPUT_EACH_N_SAMPLE		400 //roughly 100 times per second

uint16_t sample_increment = 0;
volatile uint16_t curVoltSignal = 0; //already filtered

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

		if(!OUT_BUFFER_EMPTY)
		{
			write_voltage_to_dac(outputBuffer[bufferTail]);
			curVoltSignal = outputBuffer[bufferTail++] & ~(0b1111<<12);
		}

		audioSampleEventInterruptFlag = true;
    }
}

// void tim3_isr(void) {
//     if (timer_get_flag(TIM3, TIM_SR_UIF)) { // Check if update interrupt flag is set
//         timer_clear_flag(TIM3, TIM_SR_UIF); // Clear the interrupt flag
// 		music_advance_note();
// 		// gpio_toggle(GPIOC, GPIO13);  // Assuming an LED is connected to PC13
// 	}
// }

int main(void) {
	init_clock();
	rcc_periph_clock_enable(RCC_GPIOC);
	rcc_periph_clock_enable(RCC_GPIOB);
	rcc_periph_clock_enable(RCC_GPIOA);
	

	init_input();
	init_dac();
	init_music(s_PER_TICK_FIX);
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
			audioSampleEventInterruptFlag = false;
			

			if(++sample_increment >= INPUT_EACH_N_SAMPLE)
			{
				sample_increment = 0;
				inputSampleEventInterruptFlag = true;
			}

			if(curVoltSignal == 0)
			{
				gpio_clear(GPIOC, GPIO13);
			}
			else
			{
				gpio_set(GPIOC, GPIO13);
			}

		}

		if(inputSampleEventInterruptFlag)
		{
			inputSampleEventInterruptFlag = false;
			input_update();
		}
		if(!OUT_BUFFER_FULL)
		{
			if(pressed1)
				music_play(300);
			if(pressed2)
				music_play(450);
		}
		


	}
}	
