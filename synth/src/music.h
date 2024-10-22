#ifndef MUSIC_H
#define MUSIC_H

#include "osc.h"
#include "dac.h"
#include "common.h"
#include "processing/music_processor.h"


extern osc oscilator[];

void init_music(float s_per_tick);


void music_play(uint8_t nosc, inputState *in, audio_sample_t *out);



inline __attribute__((always_inline)) void music_write_to_buffer(audio_sample_t *out)
{
    
    outputBuffer[bufferHead++] = dac_rel_to_abs_voltage(out->left);
    outputBuffer[bufferHead++] = dac_rel_to_abs_voltage(out->right);
}   


#endif