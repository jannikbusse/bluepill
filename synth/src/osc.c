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

float osc_wt_wave(float freq, float *phase, wavetable *wt)
{
	float *o = wt->table;
	
	float phaseadd = (freq * osc_s_per_tick) ;
	*phase += phaseadd ;
	if (*phase >= 1.f) {
		*phase -= 1.f;
	}
	return wt_lookup(*phase, wt);
}

void osc_play_polyphonies(osc *o, inputState_t *input, audio_sample_t *out)
{

	float res = 0;
	uint8_t activePolyphonies = 0;
	for(uint8_t i = 0; i < NR_KEYS; i++)
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
	out->left =  res * o->volume;
	out->right = out->left;
}


void osc_play_glide(osc *o, inputState_t *input, audio_sample_t *out)
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
		float lfo_modulated = raw_freq + mod_modulate(&(o->mp_mod_pitchLfo),raw_freq);
		float fr = lfo_modulated + mod_modulate(&(o->polyphonies[0].pitchModConnection), lfo_modulated);
		float powerad = ((osc_wt_wave(fr, &(p->phase), &o->oscWaveTable)));
		out->right += powerad * (1-p->pan);
		out->left += powerad * p->pan;
	}

	out->left  *= o->volume  * o->polyphonies[0].env.current_scalar;
	out->right *= o->volume  * o->polyphonies[0].env.current_scalar;
}

void osc_play_osc(osc *o, inputState_t *input , audio_sample_t *out)
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


void init_osc(osc *o)
{
    o->curOscPlaySetting = OSC_PLAY_SETTING_GLIDE;
	o->curOscState = OSC_STATE_NOT_PLAYING;
	o->nactiveVoices = NR_VOICES;
	o->volume = .4f;
	o->glideSpeed =0.035f;
	o->currentFrequency = 0;
	wt_populate_wavetable(osc_saw_wave, &o->oscWaveTable);

	for(uint16_t i = 0; i < MAX_POLYPHONIES; i++)
	{
		env_init_env_adsr(&(o->polyphonies[i].env), 0.05f, 0.01f, 0.8f, 0.1f);
		o->polyphonies[i].endPtr = o->polyphonies[i].oscVoices + NR_VOICES;

		for(uint16_t v = 0; v < NR_VOICES; v++)
		{
			o->polyphonies[i].oscVoices[v].phase = 0;
			o->polyphonies[i].oscVoices[v].pan = 0.f;
			
			o->polyphonies[i].oscVoices[v].freqOffset = 1 + 0.012f * (((NR_VOICES/2) -v));
			o->polyphonies[i].oscVoices[v].pan = 0.5 + 0.162f * (((NR_VOICES/2) -v));
			// o->polyphonies[i].oscVoices[v].volume = FIXPOINT_DECIMAL_PLACES - (200000 * (c_abs64((NR_VOICES/2) -v)));
		}
	}
	//tmp

}