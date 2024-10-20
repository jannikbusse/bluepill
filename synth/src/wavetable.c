#include "wavetable.h"
#include <math.h>

volatile float sine_table[WAVETABLE_ENTRIES];



void wt_populate_wavetable(float (*waveform)(float , float *), float* table)
{
    // float phaseadd = (freq * osc_s_per_tick) ;
	// *phase += phaseadd ;


    float phase = 0.0;
    for (int i = 0; i < WAVETABLE_ENTRIES; i++)
    {
        
        table[i] =((1.f+ sin(((float)i/(float)WAVETABLE_ENTRIES)*6.28318530717958647692f)) * 0.5f);
    }
}   
