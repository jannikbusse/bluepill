#ifndef WT_H
#define WT_H

#define WAVETABLE_ENTRIES 4096 //2**12
#define WAVETALBE_LEN_2_POTENCY 12


extern float wavetable[];
extern volatile float sine_table[];


void wt_populate_wavetable(float (*waveform)(float , float *), float* table);


#endif