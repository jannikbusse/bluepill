#include "foirrhp.h"


// uint64_t alpha = 0;

// const uint64_t SAMPLE_FRQ_BY_2PI_TIMES_FIX_DEC= (((uint64_t)SAMPLE_FREQ_HZ * 1/((uint64_t)2*(uint64_t)M_PI_FIX)) * FIXPOINT_DECIMAL_PLACES * 100;

// //in Hz * 100
// static inline uint64_t calc_alpha(uint32_t cutOffFreq)
// {
//     return  1/(1 + (SAMPLE_FRQ_BY_2PI_TIMES_FIX_DEC )/(cutOffFreq+1));
// }

// void hp_set_cuttoff_freq(uint32_t freq)
// {
//     alpha = calc_alpha(freq);
    
// }

// void hp_filter_sample_fo(uint32_t sample)
// {

// }