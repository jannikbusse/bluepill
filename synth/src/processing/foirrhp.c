#include "foirrhp.h"

const float FS_BY_2_PI = SAMPLE_FREQ_HZ/2*M_PII;

float alpha             = 0;
float lastOutputSample  = 0;
float lastInputSample   = 0;


//in Hz * 100
static inline float calc_alpha(float cutOffFreq)
{
    return  1.f/(1.f+(FS_BY_2_PI/cutOffFreq));
}

void hp_set_cuttoff_freq(float freq)
{
    alpha = calc_alpha(freq);
    
}

float hp_filter_sample_fo(float sample)
{

    float res = alpha*( lastOutputSample + sample - lastInputSample);
    lastOutputSample = res;
    lastInputSample = sample;
    return res;
}