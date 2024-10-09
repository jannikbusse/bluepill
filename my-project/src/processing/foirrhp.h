#ifndef FOI_H
#define FOI_H
#include <stdint.h>
#include "../common.h"

void hp_filter_sample_fo(uint32_t sample);
void hp_set_cuttoff_freq(uint32_t freq);


#endif