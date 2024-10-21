#include "common.h"


const float s_PER_TICK  = (1.f/SAMPLE_FREQ_HZ);
const float uS_PER_TICK = s_PER_TICK * 1000000;
const float INPUT_EACH_N_SAMPLE = SAMPLE_FREQ_HZ/INPUT_FREQ_HZ ;

float key_assignments[] = 
{   261.63f, 
    293.66f,
    329.63f,
    349.23f,
    392.00f,
    440.00f,
    493.88f,
    523.25f
};
