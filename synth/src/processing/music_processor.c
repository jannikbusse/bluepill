#include "music_processor.h"

uint8_t lpActive = 1;
float mp_lp_baseCutOffFreq = 10000;


void init_lp()
{
    lp_set_cuttoff_freq(2000);
}


void init_music_processor()
{
    init_lp();
    env_init_envelopes();
    lfo_init_lfos();
    mod_init_mod_connections();


}