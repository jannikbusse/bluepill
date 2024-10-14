#include "music.h"

float current_Waveform[1024];

osc oscilator[2];


void init_music(float s_per_tick_fix)
{
    init_oscs(s_per_tick_fix);
    init_osc(&(oscilator[0]));
    init_osc(&(oscilator[1]));
    hp_set_cuttoff_freq(10);
    lp_set_cuttoff_freq(5000);
}

uint16_t music_play(uint8_t nosc, inputState *in)
{
    float raw_sample = osc_play_osc(&(oscilator[nosc]), in);

    // if(in->activeKey != KEY_UNPRESSED)
    // {
    //     float cuttoffFreq = 30;
    //     #define DURATION 10500
    //     if(tick_counter < in->keys[in->activeKey].pressedLastAtSample + DURATION )
    //     {
    //         cuttoffFreq = MMAX ((DURATION  +  in->keys[in->activeKey].pressedLastAtSample - tick_counter), cuttoffFreq);
    //     }
    //     lp_set_cuttoff_freq(cuttoffFreq);

    // }
    float lp_sample = lp_filter_sample_fo(raw_sample);
    return dac_rel_to_abs_voltage(raw_sample);
}

