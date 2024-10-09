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
        in->keys[i].pressedLastAtSample = 0;
    }

    in->activeKey = KEY_UNPRESSED;

}

static uint8_t find_last_unpressed_key(inputState *in)
{
    uint8_t res = KEY_UNPRESSED;
    uint64_t mostRecentKeyStamp = 0;
    
    for(int i = 0; i < NR_INPUTS; i ++)
    {
        if(in->keys[i].pressed && in->keys[i].pressedLastAtSample > mostRecentKeyStamp)
        {
            res = i;
            mostRecentKeyStamp = in->keys[i].pressedLastAtSample;
        }
    }
    return res;
}

bool inputPressed(uint32_t btn)
{
    return gpio_get(GPIOA, btn);
}

void input_update(inputState *in)
{

    for(int i = 0; i < NR_INPUTS; i ++)
    {
        if(inputPressed(inputMap[i]))
        {   
            if(!in->keys[i].pressed)
            {
                //newly pressed key
                in->keys[i].pressed = true;
                in->keys[i].pressedLastAtSample = tick_counter;
                in->activeKey = i;
            }
            
        }  
        else{
            in->keys[i].pressed = false;
            if(in->activeKey == i)
            {   

                in->activeKey = find_last_unpressed_key(in);
            }
        }
        
    }


}