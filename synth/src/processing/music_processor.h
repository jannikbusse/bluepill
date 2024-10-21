#ifndef MPR_H
#define MPR_H

#include "folp.h"
#include <stdint.h>
#include "../common.h"
#include "../input.h"
#include "envelope.h"
#include "modulator.h"

extern uint8_t lpActive;


extern mod_connection lpEnvelope;

void init_lp(void);
void init_music_processor(void);


static inline float mp_lp(float raw_sample)
{
    if(!lpActive)
    {
        return raw_sample;
    } 
    lp_set_cuttoff_freq(mod_modulate(&lpEnvelope, 0, 2000));
    return lp_filter_sample_fo(raw_sample);

}

static inline void __attribute__((always_inline)) mp_update_envelopes(inputState *in)
{
    for(int i = 0; i < NR_ENVELOPES;i++)
    {
        env_update_envelope(&(envelopes[i]), in->activeKeyEvent);
    }
}

#endif