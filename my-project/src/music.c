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
volatile float curFreq = 300;
float volume = 0;
osc oscilator;


void set_volume(float vol)
{
    volume = vol;
}

void init_music(float us_per_tick)
{
    init_oscs(us_per_tick);

    oscilator.phase = 0;
    oscilator.waveform = osc_sine_wave;
}

void music_play(float frequency)
{
 
    write_relative_voltage((osc_play_osc(frequency, &oscilator)*volume));
}

void music_advance_note()
{
    idx = (idx +1 ) % length;
}
