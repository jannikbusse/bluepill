#ifndef COM_H
#define COM_H
#include <stdint.h>


#define M_PII 3.14159265358979323846

#define MMIN(a, b) ((a) < (b) ? (a) : (b))
#define MMAX(a, b) ((a) > (b) ? (a) : (b))
#define MABS(x) ((x) < 0 ? -(x) : (x))

#define INPUT1      GPIO8 
#define INPUT2      GPIO9 
#define INPUT3      GPIO10 
#define INPUT4      GPIO11 
#define INPUT5      GPIO12 

#define NR_INPUTS        4
#define KEY_UNPRESSED    255


#define SAMPLE_FREQ_HZ			44100.f
#define s_PER_TICK			(1.f/44100.f)
#define uS_PER_TICK 			22.7f


extern const float sin_table[];
extern float key_assignments[];
extern uint64_t tick_counter;


#endif