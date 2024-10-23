#include "modmatrix.h"

mod_connection mp_mod_lpEnvelope;


void mod_init_mod_connections()
{
    mod_init_mod_connection(&mp_mod_lpEnvelope,             &(envelopes[1].current_scalar),             MOD_CONNECTION_SIDE_RIGHT);
    mod_init_mod_connection(&oscilator[0].mp_mod_pitchLfo,  &(lfos[0].current_scalar),                  MOD_CONNECTION_SIDE_CENTER);


    for(int i = 0; i < MAX_POLYPHONIES; i++)
    {
        mod_init_mod_connection(&(oscilator[0].polyphonies[i].pitchModConnection), &(envelopes[0].current_scalar), MOD_CONNECTION_SIDE_RIGHT);

    }

}

void mod_init_poti_connections(inputState_t *in)
{
    in->pot[POT_ID_VOLUME].target = &(oscilator[0].mp_mod_pitchLfo.amount);
    in->pot[POT_ID_VOLUME].target = &(lfos[0].freq);
    in->pot[POT_ID_VOLUME].outputOffset = 0.01f;
    in->pot[POT_ID_VOLUME].outputRange = 50.f;
}

void mod_write_to_mod_target(mod_connection *mod, float value) 
{
    *(mod->target_scalar) = value;
    
}