#include "music.h"
#include "processing/music_processor.h"

float current_Waveform[1024];

osc oscilator[2];


void init_music(float s_per_tick)
{
    init_oscs(s_per_tick);
    init_osc(&(oscilator[0]));
    init_osc(&(oscilator[1]));
    hp_set_cuttoff_freq(10);
    lp_set_cuttoff_freq(5000);
    init_music_processor();
}

uint32_t music_play(uint8_t nosc, inputState *in)
{
    float raw_sample = osc_play_osc(&(oscilator[nosc]), in);
    // float lp_filtered =  mp_lp(raw_sample, in);

	in->eventsConsumed = true;

    return dac_rel_to_abs_voltage(raw_sample);
}

