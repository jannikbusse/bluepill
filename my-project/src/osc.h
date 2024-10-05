#ifndef OSC_H
#define OSC_H

#include <math.h>
#include <stdint.h>
#define M_PII 3.14159265358979323846
#define M_PI_FIX 31415926

struct OSC
{
    uint32_t phase;
    uint32_t (*waveform)(uint32_t freq, uint32_t *phase);
};

typedef struct OSC osc ;
void init_oscs(uint32_t s_per_tick_fix);
uint32_t osc_square_wave(uint32_t freq, uint32_t *phase);
uint32_t osc_sine_wave(uint32_t freq, uint32_t *phase);

static inline uint32_t osc_play_osc(uint32_t freq, osc *o)
{
    return o->waveform(freq, &(o->phase));
}

#endif