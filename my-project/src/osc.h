#ifndef OSC_H
#define OSC_H

#include <math.h>
#include <stdint.h>
#define M_PII 3.14159265358979323846

struct OSC
{
    float phase;
    float (*waveform)(float freq, float *phase);

};

typedef struct OSC osc ;
void init_oscs(float _us_per_tick);
float osc_square_wave(float freq, float *phase);
float osc_sine_wave(float freq, float *phase);

static inline float osc_play_osc(float freq, osc *o)
{
    return o->waveform(freq, &(o->phase));
}

#endif