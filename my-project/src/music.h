#ifndef MUSIC_H
#define MUSIC_H

#include <stdint.h>
#include "osc.h"
#include "dac.h"
#include "common.h"

#define OUT_BUFFER_EMPTY (bufferHead == bufferTail)
#define OUT_BUFFER_FULL (bufferHead + 1 == bufferTail)

extern const float c_major_scale[];
extern volatile uint16_t notes_to_play[];
extern volatile uint16_t idx;
extern volatile uint16_t length;
extern volatile float curFreq ;
extern volatile uint16_t outputBuffer[256];
extern volatile uint8_t bufferHead;
extern volatile uint8_t bufferTail;


void music_advance_note(void);
void init_music(uint32_t us_per_tick);
void music_play(uint32_t frequency);
void set_volume(uint32_t vol);


#endif