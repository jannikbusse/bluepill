#define STM32F4 1


#include <libopencm3/stm32/rcc.h>
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

inputState inpState;


uint16_t sample_increment = 0;
uint64_t tick_counter = 0;

volatile uint16_t curVoltSignal = 0; //already filtered
volatile uint32_t dummy_variable_not_used = 0;


void init_clock(void);
void init_clock()
{
	rcc_clock_setup_pll(&rcc_hse_8mhz_3v3[RCC_CLOCK_3V3_180MHZ]);
	// rcc_clock_setup_pll(&rcc_hse_8mhz_3v3[3]);
}

void dummy_usage_function(void);
void dummy_usage_function()
{
	dummy_variable_not_used = sin_table[0];
	dummy_variable_not_used = current_Waveform[0];
}


int main(void) {
	init_clock();
	rcc_periph_clock_enable(RCC_GPIOC);
	rcc_periph_clock_enable(RCC_GPIOB);
	rcc_periph_clock_enable(RCC_GPIOA);

	init_input(&inpState);
	init_dac();
	init_music(s_PER_TICK);
	//set mode to output
	gpio_mode_setup(GPIOB, GPIO_MODE_OUTPUT, GPIO_PUPD_NONE, GPIO2);
	gpio_mode_setup(GPIOB, GPIO_MODE_OUTPUT, GPIO_PUPD_NONE, GPIO1);
	gpio_clear(GPIOB, GPIO2);
	//Just wait
	for(int i = 0; i < 10000000 ; i++)
	{
		__asm volatile ("nop");
	}

	for(int i = 0; i < BUFF_SIZE; i++)
	{
		outputBuffer[i] = 123456;
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
			uint32_t buff = music_play(0, &inpState);
			music_write_to_buffer(buff);


		}
	}
}	
