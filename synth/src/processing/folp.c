#include "folp.h"



float lp_alpha = 0;
float lastOutSample = 0;



float lp_filter_sample_fo(float sample)
{
    float res = (lp_alpha * sample) + ((1-lp_alpha) * lastOutSample);
    lastOutSample = res;
    return res;
}


void lp_set_cuttoff_freq(float freq)
{
    if(freq < 20)
    {
        freq = 20;
    }
    // lp_alpha = (M_2PII * freq) / (SAMPLE_FREQ_HZ + M_2PII*freq);
    lp_alpha = (M_2PII * freq) / (SAMPLE_FREQ_HZ + M_2PII*freq);
}
