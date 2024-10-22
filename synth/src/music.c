#include "music.h"


osc oscilator[1];


void init_music(float s_per_tick)
{
    init_oscs(s_per_tick);
    init_osc(&(oscilator[0]));
    // init_osc(&(oscilator[1]));
    hp_set_cuttoff_freq(10);
    lp_set_cuttoff_freq(5000);
    init_music_processor();
}

void music_play(uint8_t nosc, inputState *in, audio_sample_t *out)
{
    
    osc_play_osc(&(oscilator[nosc]), in, out);

    mp_update(in);
    //mp_lp(out);
	in->eventsConsumed = true;
    tick_counter ++;
}
