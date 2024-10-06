#include "osc.h"

uint32_t osc_us_per_tick 	= 0;
uint32_t osc_s_per_tick 	= 227;

void init_oscs(uint32_t s_per_tick_fix)
{
    osc_us_per_tick = s_per_tick_fix * 1000000;
    osc_s_per_tick 	= s_per_tick_fix;
}

uint32_t osc_sine_wave(uint32_t freq, uint32_t *phase)
{
	//TODO FIXPOINT LOGIC
	float phaseadd = freq * 2  * ( osc_s_per_tick );
	*phase += phaseadd;
    *phase = fmod(*phase, 2 );
	
	float res = (  2*(*phase * 2)+1.0)/2.0;
	return res ;
}

//Frequency in HERTZ 440 HZ = 440 * FIXPOINT_DECIMAL_PLACES
uint32_t osc_square_wave(uint32_t freq, uint32_t *phase)
{
	uint32_t phaseadd = freq  * osc_s_per_tick ;
	*phase += phaseadd ;
	if (*phase >= FIXPOINT_DECIMAL_PLACES) {
		*phase -= FIXPOINT_DECIMAL_PLACES;
	}
	uint32_t res = (*phase < FIXPOINT_DECIMAL_PLACES>>1) ? 0 : FIXPOINT_DECIMAL_PLACES;


	return res ;
}
