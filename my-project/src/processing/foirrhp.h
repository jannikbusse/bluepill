#ifndef FOI_H
#define FOI_H
#include <stdint.h>
#include "../common.h"

float hp_filter_sample_fo(float sample);
void hp_set_cuttoff_freq(float freq);


#endif