#ifndef INP_H
#define INP_H

#include <libopencm3/stm32/rcc.h>
#include <libopencm3/stm32/gpio.h>
#include "common.h"

#define INPUT1 GPIO8 
#define INPUT2 GPIO9 
#define INPUT3 GPIO10 
#define INPUT4 GPIO11 
#define INPUT5 GPIO12 
extern bool pressed1, pressed2;

void init_input(void);
void input_update(void);
bool inputPressed(uint32_t btn);


#endif