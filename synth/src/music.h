#ifndef MUSIC_H
#define MUSIC_H

#include "osc.h"
#include "dac.h"
#include "common.h"
#include "processing/music_processor.h"


extern osc oscilator[];

void init_music(float s_per_tick);


void music_play(uint8_t nosc, inputState_t *in, audio_sample_t *out);





#endif