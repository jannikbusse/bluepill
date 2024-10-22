#include "modmatrix.h"

mod_connection mp_mod_lpEnvelope;


void mod_init_mod_connections()
{
    mod_init_mod_connection(&mp_mod_lpEnvelope,             &(envelopes[1].current_scalar), MOD_CONNECTION_SIDE_RIGHT);
    mod_init_mod_connection(&oscilator[0].mp_mod_pitchLfo,  &(lfos[0].current_scalar),      MOD_CONNECTION_SIDE_CENTER);

    for(int i = 0; i < MAX_POLYPHONIES; i++)
    {
        mod_init_mod_connection(&(oscilator[0].polyphonies[i].pitchModConnection), &(envelopes[0].current_scalar), MOD_CONNECTION_SIDE_RIGHT);

    }

}