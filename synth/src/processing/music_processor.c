#include "music_processor.h"

uint8_t lpActive = 1;
mod_connection lpEnvelope;

void init_lp()
{
    lp_set_cuttoff_freq(2000);
    mod_init_mod_connection(&lpEnvelope, &(envelopes[0].current_scalar), MOD_CONNECTION_SIDE_RIGHT);
}


void init_music_processor()
{
    init_lp();
    env_init_envelopes();


}