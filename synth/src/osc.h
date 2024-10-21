#ifndef OSC_H
#define OSC_H

#define NR_VOICES (1)
#define MAX_POLYPHONIES NR_INPUTS
#include <math.h>
#include <stdint.h>
#include "common.h"
#include "input.h"
#include "processing/foirrhp.h"
#include "wavetable.h"
#include "processing/envelope.h"


enum OSC_PLAY_SETTING {
    OSC_PLAY_SETTING_GLIDE,
    OSC_PLAY_SETTING_POLYPHONY
};
typedef enum OSC_PLAY_SETTING osc_play_setting;

enum OSC_STATE {
    OSC_STATE_PLAYING,
    OSC_STATE_NOT_PLAYING
};
typedef enum OSC_STATE osc_state;

struct VOICE
{
    float phase;
    float freqOffset;
    float volume;
};
typedef struct VOICE voice;

struct POLYPHONY
{
    voice oscVoices[NR_VOICES];
    voice* endPtr;
    envelope env;
};
typedef struct POLYPHONY polyphony;

struct OSC
{
    polyphony polyphonies[NR_INPUTS]; //one polyphony output for each input
    osc_play_setting curOscPlaySetting;
    osc_state curOscState;
    wavetable oscWaveTable;

    float currentFrequency;    
    float glideSpeed;

    uint16_t nactiveVoices;
    float oneByNActiveVoices;
    float volume;

};
typedef struct OSC osc ;

void init_oscs(float s_per_tick);
void init_osc(osc *o);
float osc_square_wave(float freq, float *phase);
float osc_saw_wave(float freq, float *phase);
float osc_sine_wave(float freq, float *phase);
float osc_wt_wave(float freq, float *phase, wavetable *wt);
float osc_play_osc(osc *o, inputState *input);

#endif