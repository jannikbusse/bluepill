#ifndef INP_H
#define INP_H

#include <libopencm3/stm32/rcc.h>
#include <libopencm3/stm32/gpio.h>
#include "common.h"
#include "adc.h"

#define POT_ID_VOLUME 0
#define POT_SMOOTHING 0.95f

struct KEY_STATE
{
    bool pressed;
    key_event_t event;
    uint64_t pressedLastAtSample;
};
typedef struct KEY_STATE keyState_t;

struct POTENTIOMETER_STATE
{
    float *target;
    float outputRange;
    float outputOffset;

};
typedef struct POTENTIOMETER_STATE potState_t;


struct INPUT_STATE
{
    keyState_t keys[NR_KEYS];
    uint8_t activeKey;
    uint64_t activeKeyPressed;
    key_event_t activeKeyEvent;
    bool eventsConsumed;

    potState_t pot[NR_POTENTIOMETERS];


};

typedef struct INPUT_STATE inputState_t;



extern const uint16_t inputMap[];
extern inputState_t inpState;

void init_input(inputState_t *in);
void input_update(inputState_t *in);

bool inputPressed(uint32_t btn);


#endif