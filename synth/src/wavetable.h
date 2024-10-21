#ifndef WT_H
#define WT_H

#include "common.h"

#define WAVETABLE_ENTRIES 128 


struct WAVETABLE
{
    uint16_t nEntries;
    float table[WAVETABLE_ENTRIES];
};
typedef struct WAVETABLE wavetable;

extern wavetable sine_table[];

void wt_populate_wavetable(float (*waveform)(float , float *), wavetable* table);


#endif