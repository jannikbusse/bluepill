#ifndef MPR_H
#define MPR_H

#include "folp.h"
#include <stdint.h>
#include "../common.h"
#include "../input.h"
#include "envelope.h"

extern uint8_t lpActive;
static envelope lpEnvelope;

static inline void init_lp(void)
{
    env_init_env(&lpEnvelope);
    env_set_attack(&lpEnvelope, 0.001f);
    env_set_decay(&lpEnvelope, 0.1f);
    env_set_sustain(&lpEnvelope, 0.9f);
    env_set_release(&lpEnvelope, 1.f);
}

static inline void init_music_processor(void)
{
    init_lp();

}

static inline float mp_lp(float raw_sample, inputState *in)
{
    if(!lpActive)
    {
        return raw_sample;
    } 
    env_update_envelope(&lpEnvelope, in->activeKeyEvent);
    lp_set_cuttoff_freq(lpEnvelope.current_scalar * 3000);
    return lp_filter_sample_fo(raw_sample);

}

#endif