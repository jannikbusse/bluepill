#ifndef OSC_H
#define OSC_H

#include <math.h>
#include <stdint.h>
#include "common.h"

//only needed for debugging 
#include <libopencm3/stm32/rcc.h>
#include <libopencm3/stm32/spi.h>
#include <libopencm3/stm32/gpio.h>

struct OSC
{
    uint32_t phase;
    uint32_t (*waveform)(uint32_t freq, uint32_t *phase);
};

typedef struct OSC osc ;
void init_oscs(uint32_t s_per_tick_fix);
uint32_t osc_square_wave(uint32_t freq, uint32_t *phase);
uint32_t osc_sine_wave(uint32_t freq, uint32_t *phase);

static inline uint32_t osc_play_osc(uint32_t freq, osc *o)
{
    return o->waveform(freq, &(o->phase));
}

#endif