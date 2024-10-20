#include "wavetable.h"


extern volatile float sine_table[];


void wt_populate_wavetable(float (*waveform)(float , float *), float* table)
{
    // float phaseadd = (freq * osc_s_per_tick) ;
	// *phase += phaseadd ;


    float phase = 0.0;
    for (int i = 0; i < WAVETABLE_ENTRIES; i++)
    {
        
        table[i] = waveform(440.f, &phase);
    }
}   
