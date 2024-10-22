#ifndef LFO_H
#define LFO_H

#include <stdint.h>
#include "../common.h"
#include "../wavetable.h"
#include "../osc.h"

#define NR_LFOS (3)


struct LFO
{
    wavetable wt;
    float phase;
    float freq;
    float current_scalar;
};
typedef struct LFO lfo;

extern lfo lfos[];

void lfo_init_lfo(lfo *lfo);
void lfo_init_lfos(void);

inline __attribute__((always_inline)) void lfo_update_lfo(lfo *_lfo)
{
    _lfo->phase += _lfo->freq * s_PER_TICK;
    if (_lfo->phase >= 1.f)
    {
        _lfo->phase -= 1.f;
    }
    _lfo->current_scalar = wt_lookup(_lfo->phase, &(_lfo->wt));
}

#endif