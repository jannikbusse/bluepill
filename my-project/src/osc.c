#include "osc.h"

float osc_us_per_tick 	= 0;
float osc_s_per_tick 	= 1.f/44100.f;

void init_oscs(float s_per_tick)
{
    osc_us_per_tick = s_per_tick * 1000000;
    osc_s_per_tick 	= s_per_tick;

}

float osc_sine_wave(float freq, float *phase)
{
	//TODO FIXPOINT LOGIC
	float phaseadd = freq * 2  * ( osc_s_per_tick );
	*phase += phaseadd;
    *phase = fmod(*phase, 2 );
	
	float res = (  2*(*phase * 2)+1.0)/2.0;
	return res ;
}


float osc_square_wave(float freq, float *phase)
{
	float phaseadd = (freq  * osc_s_per_tick) ;
	*phase += phaseadd ;
	if (*phase >= 1.f) {
		*phase -= 1.f;
	}

	return (*phase < 0.5f) ? 0 : 1;
}

float osc_saw_wave(float freq, float *phase)
{
	float phaseadd = (freq  * osc_s_per_tick) ;
	*phase += phaseadd ;
	if (*phase >= 1.f) {
		*phase -= 1.f;
	}

	return *phase - (int)*phase;
}

static float osc_play_polyphonies(osc *o, inputState *input)
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
				float powerad = ((o->waveform(fr, &(o->polyphonies[i].oscVoices[v].phase))))/(o->nactiveVoices);
				res += powerad;
			}
		}
	}
	return res*o->volume;
}

static float osc_play_glide(osc *o, inputState *input)
{
	static float activeFrequency = 0;
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
	
	

	for(voice* p = o->polyphonies[0].oscVoices; p < o->polyphonies[0].endPtr; p++ )
	{
		env_update_envelope(&(o->polyphonies[0].env), input->activeKeyEvent);

		float fr =o->currentFrequency * p->freqOffset;
		float powerad = ((o->waveform(fr, &(p->phase))));
		res += powerad;
	}

	return res * o->volume * o->oneByNActiveVoices * o->polyphonies[0].env.current_scalar;
	// return res * o->volume * o->oneByNActiveVoices;
}

float osc_play_osc(osc *o, inputState *input )
{
	//state logic
	switch (o->curOscPlaySetting)
	{
	case OSC_PLAY_SETTING_GLIDE:
		return osc_play_glide(o, input);
		break;

	case OSC_PLAY_SETTING_POLYPHONY:
		return osc_play_polyphonies(o, input);
		break;
	
	default:
		break;
	}

    return 0;
}

void init_osc(osc *o)
{
	o->waveform = osc_saw_wave;
    o->curOscPlaySetting = OSC_PLAY_SETTING_GLIDE;
	o->curOscState = OSC_STATE_NOT_PLAYING;
	o->nactiveVoices = NR_VOICES;
	o->oneByNActiveVoices = 1.f / o->nactiveVoices;
	o->volume = .04f;
	o->glideSpeed =0.05f;
	o->currentFrequency = 0;
	for(uint16_t i = 0; i < MAX_POLYPHONIES; i++)
	{
		env_init_env(&(o->polyphonies[i].env));
		o->polyphonies[i].endPtr = o->polyphonies[i].oscVoices + sizeof(o->polyphonies[i].oscVoices)/ sizeof(o->polyphonies[i].oscVoices[i]);

		for(uint16_t v = 0; v < NR_VOICES; v++)
		{
			o->polyphonies[i].oscVoices[v].phase = 0;
			o->polyphonies[i].oscVoices[v].freqOffset = 1 + 0.2f * (((NR_VOICES/2) -v));
			// o->polyphonies[i].oscVoices[v].volume = FIXPOINT_DECIMAL_PLACES - (200000 * (c_abs64((NR_VOICES/2) -v)));
		}
	}

}
