#include "music.h"

float current_Waveform[1024];

osc oscilator[2];


void init_music(float s_per_tick_fix)
{
    init_oscs(s_per_tick_fix);
    init_osc(&(oscilator[0]));
    init_osc(&(oscilator[1]));

}

uint16_t music_play(uint8_t nosc, inputState *in)
{
    return dac_rel_to_abs_voltage((osc_play_osc(&(oscilator[nosc]), in)));
}

