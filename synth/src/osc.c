#include "osc.h"

float osc_us_per_tick 	= 0;
float osc_s_per_tick 	= 1.f/44100.f;
//global variable that belongs to osc_play_glide
float activeFrequency = 100;

void init_oscs(float s_per_tick)
{
    osc_us_per_tick = s_per_tick * 1000000;
    osc_s_per_tick 	= s_per_tick;

}

float osc_sine_wave(float freq, float *phase)
{
	float phaseadd = freq * osc_s_per_tick ;
	*phase += phaseadd;
	if (*phase >= 1.f) {
		*phase -= 1.f;
	}
	
	float res =(1+ sin((*phase)*M_2PII)) * 0.5f;
	return res ;
}


float osc_square_wave(float freq, float *phase)
{
	float phaseadd = (freq * osc_s_per_tick) ;
	*phase += phaseadd ;
	if (*phase >= 1.f) {
		*phase -= 1.f;
	}

	return (*phase < 0.5f) ? 0 : 1;
}

float osc_saw_wave(float freq, float *phase)
{
	float phaseadd = (freq * osc_s_per_tick) ;
	*phase += phaseadd ;
	if (*phase >= 1.f) {
		*phase -= 1.f;
	}

	return *phase - (int)*phase;
}

inline __attribute__((always_inline)) float osc_wt_wave(float freq, float *phase, wavetable *wt)
{
	float *o = wt->table;
	
	float phaseadd = (freq * osc_s_per_tick) ;
	*phase += phaseadd ;
	if (*phase >= 1.f) {
		*phase -= 1.f;
	}
	return wt_lookup(*phase, wt);

}


void init_osc(osc *o)
{
    o->curOscPlaySetting = OSC_PLAY_SETTING_GLIDE;
	o->curOscState = OSC_STATE_NOT_PLAYING;
	o->nactiveVoices = NR_VOICES;
	o->oneByNActiveVoices = 1.f / o->nactiveVoices;
	o->volume = .4f;
	o->glideSpeed =0.0035f;
	o->currentFrequency = 0;
	wt_populate_wavetable(osc_square_wave, &o->oscWaveTable);

	for(uint16_t i = 0; i < MAX_POLYPHONIES; i++)
	{
		env_init_env_adsr(&(o->polyphonies[i].env), 0.05f, 0.01f, 0.8f, 0.1f);
		o->polyphonies[i].endPtr = o->polyphonies[i].oscVoices + sizeof(o->polyphonies[i].oscVoices)/ sizeof(o->polyphonies[i].oscVoices[i]);

		for(uint16_t v = 0; v < NR_VOICES; v++)
		{
			o->polyphonies[i].oscVoices[v].phase = 0;
			o->polyphonies[i].oscVoices[v].pan = 0.f;
			
			o->polyphonies[i].oscVoices[v].freqOffset = 1 + 0.011f * (((NR_VOICES/2) -v));
			// o->polyphonies[i].oscVoices[v].volume = FIXPOINT_DECIMAL_PLACES - (200000 * (c_abs64((NR_VOICES/2) -v)));
		}
	}

	//tmp
	o->polyphonies[0].oscVoices[0].pan = 0.f;
	o->polyphonies[0].oscVoices[1].pan = 0.4f;
	
	o->polyphonies[0].oscVoices[2].pan = 0.5f;	

	o->polyphonies[0].oscVoices[3].pan = 0.6f;
	o->polyphonies[0].oscVoices[4].pan = 1.f;

}
