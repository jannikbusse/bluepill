#ifndef MPR_H
#define MPR_H

#include "folp.h"
#include "../common.h"
#include "../input.h"
#include "envelope.h"
#include "modulator.h"
#include "modmatrix.h"
#include "lfo.h"

extern uint8_t lpActive;

extern mod_connection mp_mod_lpEnvelope;

void init_lp(void);
void init_music_processor(void);


inline __attribute__((always_inline)) void mp_lp(audio_sample_t *out)
{
    if(!lpActive)
    {
        return;
    } 
    lp_set_cuttoff_freq(mod_modulate(&mp_mod_lpEnvelope, 0, 500));
    out->left = lp_filter_sample_fo(out->left);
    out->right = lp_filter_sample_fo(out->right);
}

inline void __attribute__((always_inline)) mp_update(inputState_t *in)
{
    //update envelopes
    for(int i = 0; i < NR_ENVELOPES;i++)
    {
        env_update_envelope(&(envelopes[i]), in->activeKeyEvent);
    }

    //update lfos
    for(int i = 0; i < NR_LFOS; i++)
    {
        lfo_update_lfo(&(lfos[i]));
    }

}

#endif