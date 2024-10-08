#ifndef OSC_H
#define OSC_H

#define NR_VOICES (1)
#define MAX_POLYPHONIES NR_INPUTS
#include <math.h>
#include <stdint.h>
#include "common.h"
#include "input.h"

enum OSC_STATE {
    OSC_STATE_GLIDE,
    OSC_STATE_POLYPHONY
};
typedef enum OSC_STATE osc_state;

struct VOICE
{
    uint32_t phase;
    uint64_t freqOffset;
    uint64_t volume;
};
typedef struct VOICE voice;

struct POLYPHONY
{
    voice oscVoices[NR_VOICES];
};
typedef struct POLYPHONY polyphony;

struct OSC
{
    polyphony polyphonies[NR_INPUTS]; //one polyphony output for each input
    osc_state currentOscState;
    uint32_t (*waveform)(uint32_t freq, uint32_t *phase);
    uint8_t nactiveVoices;
    //0 - FIXPOINT_DECIMAL_PLACES
    uint64_t volume;

};
typedef struct OSC osc ;

void init_oscs(uint32_t s_per_tick_fix);
void init_osc(osc *o);
uint32_t osc_square_wave(uint32_t freq, uint32_t *phase);
uint32_t osc_sine_wave(uint32_t freq, uint32_t *phase);
uint32_t osc_play_osc(osc *o, inputState *input);

#endif