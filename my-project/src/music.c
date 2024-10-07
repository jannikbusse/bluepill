#include "music.h"



const float c_major_scale[] = {
 523.25,
 587.33,
 659.25,
 698.46,
 783.99,
 880.00,
 987.77,
0.f
};

volatile uint16_t notes_to_play[] = {0,7,1,7,1,7,2,7,3,7,3,7,3,7,3,7,3,7};
volatile uint16_t idx = 0;
volatile uint16_t length = sizeof(notes_to_play) / sizeof(notes_to_play[0]);
osc oscilator;



void init_music(uint32_t s_per_tick_fix)
{
    init_oscs(s_per_tick_fix);

    oscilator.phase = 0;
    oscilator.waveform = osc_square_wave;
}

void music_play(uint32_t frequency)
{
    outputBuffer[bufferHead++] = dac_rel_to_abs_voltage((osc_play_osc(frequency, &oscilator))>>8);
}


void music_advance_note()
{
    idx = (idx +1 ) % length;
}
