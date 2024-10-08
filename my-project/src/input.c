#include "input.h"

const uint16_t inputMap[] = {INPUT1, INPUT2, INPUT3, INPUT4}; //make sure this matches the nr of inputs in common.h

void init_input(inputState *in)
{
    for(int i = 0; i < NR_INPUTS; i ++)
    {
        gpio_set_mode(GPIOA, GPIO_MODE_INPUT, GPIO_CNF_INPUT_PULL_UPDOWN, inputMap[i]);
        gpio_clear(GPIOA, inputMap[i]);
    }

    for(int i = 0; i < NR_INPUTS; i++)
    {
        in->keys[i].pressed = false;
    }

}

bool inputPressed(uint32_t btn)
{
    return gpio_get(GPIOA, btn);
}

void input_update(inputState *in)
{

    for(int i = 0; i < NR_INPUTS; i ++)
    {
        in->keys[i].pressed = inputPressed(inputMap[i]);
    }


}