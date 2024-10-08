#include "osc.h"


uint32_t osc_us_per_tick 	= 0;
uint32_t osc_s_per_tick 	= 227;

void init_oscs(uint32_t s_per_tick_fix)
{
    osc_us_per_tick = s_per_tick_fix * 1000000;
    osc_s_per_tick 	= s_per_tick_fix;
}

uint32_t osc_sine_wave(uint32_t freq, uint32_t *phase)
{
	//TODO FIXPOINT LOGIC
	float phaseadd = freq * 2  * ( osc_s_per_tick );
	*phase += phaseadd;
    *phase = fmod(*phase, 2 );
	
	float res = (  2*(*phase * 2)+1.0)/2.0;
	return res ;
}

//Frequency in HERTZ 440 * 100 HZ = 440 * FIXPOINT_DECIMAL_PLACES
uint32_t osc_square_wave(uint32_t freq, uint32_t *phase)
{
	uint32_t phaseadd = (freq  * osc_s_per_tick)/100 ;
	*phase += phaseadd ;
	if (*phase >= FIXPOINT_DECIMAL_PLACES) {
		*phase -= FIXPOINT_DECIMAL_PLACES;
	}
	uint32_t res = (*phase < FIXPOINT_DECIMAL_PLACES>>1) ? 0 : FIXPOINT_DECIMAL_PLACES;


	return res ;
}

static uint32_t osc_play_polyphonies(osc *o, inputState *input)
{
	uint64_t res = 0;
	uint8_t activePolyphonies = 0;
	for(uint8_t i = 0; i < NR_INPUTS; i++)
	{
		if(input->keys[i].pressed && activePolyphonies < MAX_POLYPHONIES)
		{
			for(uint8_t v = 0; v <o->nactiveVoices; v ++)
			{
				// uint64_t fr = (uint64_t)((key_assignments[i]*o->polyphonies[i].oscVoices[v].freqOffset) / FIXPOINT_DECIMAL_PLACES);
				uint64_t fr = (key_assignments[i]);
				uint64_t powerad = ((uint64_t)(o->waveform(fr, &(o->polyphonies[i].oscVoices[v].phase))))/(uint64_t)(o->nactiveVoices);
				// powerad = (powerad * o->polyphonies[i].oscVoices[v].volume )/ FIXPOINT_DECIMAL_PLACES;
				res += powerad;
			}
			
		}
	}
	return ((res*o->volume)/FIXPOINT_DECIMAL_PLACES);
}

uint32_t osc_play_osc(osc *o, inputState *input )
{

	switch (o->currentOscState)
	{
	case OSC_STATE_GLIDE:
		/*Not implemented*/
		break;

	case OSC_STATE_POLYPHONY:
		return osc_play_polyphonies(o, input);
		break;
	
	default:
		break;
	}

    return 0;
}

void init_osc(osc *o)
{
	o->waveform = osc_square_wave;
    o->currentOscState = OSC_STATE_POLYPHONY;
	o->nactiveVoices = NR_VOICES;
	o->volume = 200000;
	for(uint16_t i = 0; i < MAX_POLYPHONIES; i++)
	{
		for(uint16_t v = 0; v < NR_VOICES; v++)
		{
			o->polyphonies[i].oscVoices[v].phase = (((v * 1242) % 117)*1554253 )%FIXPOINT_DECIMAL_PLACES;
			o->polyphonies[i].oscVoices[v].phase = 0;
			o->polyphonies[i].oscVoices[v].freqOffset = FIXPOINT_DECIMAL_PLACES - (50000 * (((NR_VOICES/2) )-v));
			// o->polyphonies[i].oscVoices[v].volume = FIXPOINT_DECIMAL_PLACES - (200000 * (c_abs64((NR_VOICES/2) -v)));
		}
	}

   
}
