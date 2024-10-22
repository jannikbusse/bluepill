#ifndef MUSIC_H
#define MUSIC_H

#include <stdint.h>
#include "osc.h"
#include "dac.h"
#include "common.h"
#include "processing/music_processor.h"


extern osc oscilator[];

void init_music(float s_per_tick);
uint32_t music_play(uint8_t nosc, inputState *in);

static inline void music_write_to_buffer(uint32_t voltage)
{
    outputBuffer[bufferHead++] = voltage;
    outputBuffer[bufferHead++] = voltage;
}


#endif