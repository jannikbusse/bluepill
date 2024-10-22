#ifndef OSC_H
#define OSC_H

#define NR_VOICES (5)
#define MAX_POLYPHONIES NR_INPUTS
#include <math.h>
#include <stdint.h>
#include "common.h"
#include "input.h"
#include "processing/foirrhp.h"
#include "wavetable.h"
#include "processing/envelope.h"
#include "processing/modulator.h"


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
    float pan; //left = 0, right = 1
    float volume;
};
typedef struct VOICE voice;

struct POLYPHONY
{
    voice oscVoices[NR_VOICES];
    voice* endPtr;
    envelope env;
    mod_connection pitchModConnection;
};
typedef struct POLYPHONY polyphony;

struct OSC
{
    polyphony polyphonies[NR_INPUTS]; //one polyphony output for each input
    osc_play_setting curOscPlaySetting;
    osc_state curOscState;
    wavetable oscWaveTable;
    mod_connection volEnvModCon;
    mod_connection mp_mod_pitchLfo;
    
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

//global variable that belongs to osc_play_glide
extern float activeFrequency;
inline __attribute__((always_inline)) void osc_play_glide(osc *o, inputState *input, audio_sample_t *out)
{
	if(input->activeKey != KEY_UNPRESSED)
	{
		activeFrequency = key_assignments[input->activeKey];
	}

	float res = 0;

	float glideadd = activeFrequency - o->currentFrequency;
	if(glideadd > 0)
	{
		o->currentFrequency += MMIN(glideadd, o->glideSpeed);
	}
	else
	{
		o->currentFrequency -= MMIN(-glideadd, o->glideSpeed);
	}
    env_update_envelope(&(o->polyphonies[0].env), input->activeKeyEvent);

	for(voice* p = o->polyphonies[0].oscVoices; p < o->polyphonies[0].endPtr; p++ )
	{

		float raw_freq = o->currentFrequency * p->freqOffset;
		float lfo_modulated = mod_modulate(&(o->mp_mod_pitchLfo),raw_freq, raw_freq*0.05f);
		float fr = mod_modulate(&(o->polyphonies[0].pitchModConnection), lfo_modulated, lfo_modulated * 4.f);
		float powerad = ((osc_wt_wave(fr, &(p->phase), &o->oscWaveTable)));
		out->right += powerad * (1-p->pan);
		out->left += powerad * p->pan;
	}

	out->left  *= o->volume * o->oneByNActiveVoices * o->polyphonies[0].env.current_scalar;
	out->right *= o->volume * o->oneByNActiveVoices * o->polyphonies[0].env.current_scalar;
}

inline __attribute__((always_inline)) void osc_play_polyphonies(osc *o, inputState *input, audio_sample_t *out)
{

	float res = 0;
	uint8_t activePolyphonies = 0;
	for(uint8_t i = 0; i < NR_INPUTS; i++)
	{
		if(input->keys[i].pressed && activePolyphonies < MAX_POLYPHONIES)
		{
			for(uint8_t v = 0; v <o->nactiveVoices; v ++)
			{
				float fr = (key_assignments[i]);
				float powerad = ((osc_wt_wave(fr, &(o->polyphonies[i].oscVoices[v].phase), &o->oscWaveTable)));
				res += powerad;
			}
		}
	}
	out->left =  res * o->volume * o->oneByNActiveVoices;
	out->right = out->left;
}



inline __attribute__((always_inline)) void osc_play_osc(osc *o, inputState *input , audio_sample_t *out)
{
	//state logic
	switch (o->curOscPlaySetting)
	{
	case OSC_PLAY_SETTING_GLIDE:
		osc_play_glide(o, input, out);
        return;
		break;

	case OSC_PLAY_SETTING_POLYPHONY:
		osc_play_polyphonies(o, input, out);
        return;
        break;
	
	default:
		break;
	}
}

#endif