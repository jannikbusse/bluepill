#define STM32F4 1


#include <libopencm3/stm32/rcc.h>
#include <libopencm3/stm32/gpio.h>
#include <libopencm3/stm32/flash.h>
#include "timer.h"
#include "dac.h"
#include "adc.h"
#include <math.h>
#include"osc.h"
#include "music.h"
#include "input.h"
#include "wavetable.h"
#include "leds/led.h"


volatile bool audioSampleEventInterruptFlag = false; //true if a interrupt occurs
volatile bool inputSampleEventInterruptFlag = false; 

inputState_t inpState;
audio_sample_t currentSample;

uint16_t sample_increment = 0;
uint64_t tick_counter = 0;

led_config_t ledConfig;

volatile uint16_t curVoltSignal = 0; //already filtered
volatile uint32_t dummy_variable_not_used = 0;

void tim2_isr(void) {
        // Clear the interrupt flag
        timer_clear_flag(TIM2, TIM_SR_UIF);
		gpio_toggle(GPIOB, GPIO1);
}



void init_clock(void);
void init_clock()
{
	rcc_clock_setup_pll(&rcc_hse_8mhz_3v3[RCC_CLOCK_3V3_180MHZ]);
	// rcc_clock_setup_pll(&rcc_hse_16mhz_3v3[RCC_CLOCK_3V3_180MHZ]);
	// rcc_clock_setup_pll(&rcc_hse_8mhz_3v3[3]);
}



int main(void) {
	init_clock();
	rcc_periph_clock_enable(RCC_GPIOC);
	rcc_periph_clock_enable(RCC_GPIOB);
	rcc_periph_clock_enable(RCC_GPIOA);

	wt_populate_wavetable(osc_sine_wave, &sine_table);
	
	init_input(&inpState);
	init_dac();
	init_adc();
	init_music(s_PER_TICK);
	init_leds(&ledConfig);
	init_timer2();

	mod_init_poti_connections(&inpState);
	currentSample.left = 0;	
	currentSample.right = 0;

	//set mode to output
	gpio_mode_setup(GPIOB, GPIO_MODE_OUTPUT, GPIO_PUPD_NONE, GPIO2);
	gpio_mode_setup(GPIOB, GPIO_MODE_OUTPUT, GPIO_PUPD_NONE, GPIO1);
	// enableTimer2();

	gpio_clear(GPIOB, GPIO2);
	//Just wait
	for(int i = 0; i < 10000000 ; i++)
	{
		__asm volatile ("nop");
	}

	
	while(1)
	{


		if(++sample_increment >= INPUT_EACH_N_SAMPLE)
		{
			sample_increment = 0;
			inputSampleEventInterruptFlag = true;
		}

		if(inputSampleEventInterruptFlag)
		{
			inputSampleEventInterruptFlag = false;
			input_update(&inpState);
		}

		if(!OUT_BUFFER_FULL)
		{
			gpio_set(GPIOB, GPIO1);
			currentSample.left = 0;
			currentSample.right = 0;
			music_play(0, &inpState, &currentSample);
			music_write_to_buffer(&currentSample);
			gpio_clear(GPIOB, GPIO1);

		}
	}
}	
