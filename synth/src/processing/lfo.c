#include "lfo.h"

lfo lfos[NR_LFOS];

void lfo_init_lfo(lfo *_lfo)
{
    _lfo->freq = 16.f;
    _lfo->phase = 0;
    _lfo->current_scalar = 0;
    wt_populate_wavetable(osc_sine_wave, &(_lfo->wt));
}

void lfo_init_lfos()
{
    for(int i = 0; i < NR_LFOS; i++)
    {
        lfo_init_lfo(&(lfos[i]));
    }
}