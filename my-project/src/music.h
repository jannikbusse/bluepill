#ifndef MUSIC_H
#define MUSIC_H

#include <stdint.h>
#include "osc.h"
#include "dac.h"
#include "common.h"



extern float current_Waveform[];


void init_music(float s_per_tick);
uint16_t music_play(uint8_t nosc, inputState *in);

static inline void music_write_to_buffer(uint16_t voltage)
{
    outputBuffer[bufferHead++] = (voltage & ~(0b1111 << 12) ) | (0b0011 << 12);
}


#endif