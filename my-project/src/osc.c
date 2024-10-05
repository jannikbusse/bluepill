#include "osc.h"

float osc_us_per_tick = 0;
float osc_s_per_tick = 0;

void init_oscs(float _us_per_tick)
{
    #define INIT_OSC_CALLED 1
    osc_us_per_tick = _us_per_tick;
    osc_s_per_tick = osc_us_per_tick / 1000000.0;
}



float osc_sine_wave(float freq, float *phase)
{

	float phaseadd = freq * 2  * ( osc_s_per_tick );
	*phase += phaseadd;
    *phase = fmod(*phase, 2 );
	
	float res = (  2*(*phase * 2)+1.0)/2.0;
	return res ;
}

float osc_square_wave(float freq, float *phase)
{

	float phaseadd = freq * 2 * ( osc_s_per_tick );
	*phase += phaseadd;
	if (*phase >= 1.0) {
		*phase -= 1.0;
	}
	uint8_t res = (*phase < 0.5) ? 0 : 1;

	return res ;
}
