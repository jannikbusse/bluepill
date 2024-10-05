#ifndef MUSIC_H
#define MUSIC_H

#include <stdint.h>
#include "osc.h"
#include "dac.h"



extern const float c_major_scale[];
extern volatile uint16_t notes_to_play[];
extern volatile uint16_t idx;
extern volatile uint16_t length;
extern volatile float curFreq ;
void music_advance_note(void);
void init_music(float us_per_tick);
void music_play(float frequency);
void set_volume(float vol);

#endif