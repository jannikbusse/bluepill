#include "wavetable.h"
#include <math.h>

wavetable sine_table;


void wt_populate_wavetable(float (*waveform)(float , float *), wavetable* table)
{

    float phase = 0.0;
    for (int i = 0; i < WAVETABLE_ENTRIES; i++)
    {
        
        table->table[i] = waveform((float)SAMPLE_FREQ_HZ / (float)WAVETABLE_ENTRIES, &phase);
    }
}   
