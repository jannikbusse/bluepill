#ifndef WT_H
#define WT_H

#include "common.h"

#define WAVETABLE_ENTRIES 2048 


struct WAVETABLE
{
    uint16_t nEntries;
    float table[WAVETABLE_ENTRIES];
};
typedef struct WAVETABLE wavetable;

extern wavetable sine_table;

void wt_populate_wavetable(float (*waveform)(float , float *), wavetable* table);

/**
 * @brief Look up a value in the wavetable. Phase needs to be between 0 and 1!
 */
static inline float __attribute__((always_inline)) wt_lookup(float phase, wavetable *wt)
{
    return wt->table[(uint32_t)(phase * ((float)WAVETABLE_ENTRIES))];
}


#endif