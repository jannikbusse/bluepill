#define STM32F1 1
#include <libopencm3/stm32/rcc.h>
#include <libopencm3/stm32/spi.h>
#include <libopencm3/stm32/gpio.h>
#include <libopencm3/stm32/flash.h>
#include "timer.h"
#include "dac.h"
#include <math.h>

#define M_PI 3.1415926f
volatile bool audioSampleEventInterruptFlag = false; //true if a interrupt occurs

#define CLOCK_SPEED_HZ 	24000000f
#define uS_PER_TICK 	22.7

float c_major_scale[] = {
 523.25,
 587.33,
 659.25,
 698.46,
 783.99,
 880.00,
 987.77,
0.f
};

volatile uint16_t notes_to_play[] = {0,7,1,7,1,7,2,7,3,7,3,7,3,7,3,7,3,7};
volatile  idx = 0;
volatile uint16_t length = sizeof(notes_to_play) / sizeof(notes_to_play[0]);

double curFreq = 300;

void init_clock(void)
{
	rcc_clock_setup_pll(&rcc_hse_configs[RCC_CLOCK_HSE8_24MHZ]);
}

void tim2_isr(void) {
    // Check if the update interrupt flag is set
    if (timer_get_flag(TIM2, TIM_SR_UIF)) {
        // Clear the interrupt flag
        timer_clear_flag(TIM2, TIM_SR_UIF);
		audioSampleEventInterruptFlag = true;
    }
}

void tim3_isr(void) {
    if (timer_get_flag(TIM3, TIM_SR_UIF)) { // Check if update interrupt flag is set
        timer_clear_flag(TIM3, TIM_SR_UIF); // Clear the interrupt flag
		idx = (idx +1 ) % length;
		// gpio_toggle(GPIOC, GPIO13);  // Assuming an LED is connected to PC13

	}
}

float square_wave(float freq, float *phase);
float square_wave(float freq, float *phase)
{

	float phaseadd = freq * 2 * ( (uS_PER_TICK)/1000000.0 );
	*phase += phaseadd;
	if (*phase >= 1.0f) {
		*phase -= 1.0f;
	}
	uint8_t res = (*phase < 0.5f) ? 0 : 1;

	return res ;
}
float sine_wave(float freq, float *phase)
{

	float phaseadd = freq * ( (uS_PER_TICK)/1000000.0 ) ;
	*phase += phaseadd;
	
	float res = sinf(*phase / M_PI);

	return res ;
}

int main(void) {

	init_clock();
	rcc_periph_clock_enable(RCC_GPIOC);
	rcc_periph_clock_enable(RCC_GPIOB);
	rcc_periph_clock_enable(RCC_GPIOA);

	init_dac();
	//set mode to output
	gpio_set_mode(GPIOC, GPIO_MODE_OUTPUT_2_MHZ, GPIO_CNF_OUTPUT_PUSHPULL, GPIO13);
	
	//Just wait
	for(int i = 0; i < 10000000 ; i++)
	{
		__asm volatile ("nop");
	}
	init_timer2();
	init_timer3();
	float phase1 = 0;
	float phase2 = 0;
	float phase3 = 0;

	while(1)
	{
		if(audioSampleEventInterruptFlag)
		{
			//pass current value to analog out
			// float v = ((sinf(t)+1.0f)/2.0f) ;
			float p1 = square_wave(300, &phase1);
			// float p2 = sine_wave( c_major_scale[(notes_to_play[idx]+2 )% 7], &phase2);
			// float p3 = sine_wave( c_major_scale[(notes_to_play[idx]+4 )% 7], &phase3);

			// float res = p1 + p2 + p3;
			// if (notes_to_play[idx] == 7) res = 0;
			// res /=3.;
			write_relative_voltage((p1*0.01f));
		
			//handle audio generation of new element
			audioSampleEventInterruptFlag = false;
		}

	}
}	
