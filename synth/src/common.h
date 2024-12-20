#ifndef COM_H
#define COM_H
#include <stdint.h>


#define M_PII 3.14159f
#define M_2PII 6.28318f

#define MMIN(a, b) ((a) < (b) ? (a) : (b))
#define MMAX(a, b) ((a) > (b) ? (a) : (b))
#define MABS(x) ((x) < 0 ? -(x) : (x))

#define INPUT1      GPIO8 
#define INPUT2      GPIO15 
#define INPUT3      GPIO10 
#define INPUT4      GPIO11 
#define INPUT5      GPIO12 

#define NR_KEYS             4
#define NR_POTENTIOMETERS   1
#define KEY_UNPRESSED       255

//#define CLOCK_SPEED_HZ 			180000000 //in hz
#define SAMPLE_FREQ_HZ			44100.f
#define INPUT_FREQ_HZ           100

extern const float s_PER_TICK;
extern const float uS_PER_TICK;
extern const float INPUT_EACH_N_SAMPLE;
extern float key_assignments[];
extern uint64_t tick_counter;

struct AUDIO_SAMPLE
{
    float left;
    float right;
};
typedef struct AUDIO_SAMPLE audio_sample_t;

enum KEY_EVENT {
    KEY_EVENT_PRESSED,
    KEY_EVENT_RELEASED,
    KEY_EVENT_NOTHING
};
typedef enum KEY_EVENT key_event_t;

#endif