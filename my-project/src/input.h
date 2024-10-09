#ifndef INP_H
#define INP_H

#include <libopencm3/stm32/rcc.h>
#include <libopencm3/stm32/gpio.h>
#include "common.h"


struct KEY_STATE
{
    bool pressed;
    uint64_t pressedLastAtSample;
};
typedef struct KEY_STATE keyState;


struct INPUT_STATE
{
    keyState keys[NR_INPUTS];
    uint8_t activeKey;
    uint64_t activeKeyPressed;

};
typedef struct INPUT_STATE inputState;

extern const uint16_t inputMap[];
extern inputState inpState;

void init_input(inputState *in);
void input_update(inputState *in);
bool inputPressed(uint32_t btn);


#endif