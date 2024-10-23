#include "input.h"
#include <libopencm3/stm32/rcc.h>
#include <libopencm3/stm32/spi.h>
#include <libopencm3/stm32/gpio.h>

const uint16_t inputMap[] = {INPUT1, INPUT2, INPUT3, INPUT4}; //make sure this matches the nr of inputs in common.h

void init_input(inputState_t *in)
{
    for(int i = 0; i < NR_KEYS; i ++)
    {
        gpio_mode_setup(GPIOA, GPIO_MODE_INPUT, GPIO_PUPD_PULLDOWN, inputMap[i]);
        gpio_clear(GPIOA, inputMap[i]);
    }

    for(int i = 0; i < NR_KEYS; i++)
    {
        in->keys[i].pressed = false;
        in->keys[i].event = KEY_EVENT_NOTHING;
        in->keys[i].pressedLastAtSample = 0;
    }
    in->activeKey = KEY_UNPRESSED;
    in->eventsConsumed = false;

    for(int i = 0; i < NR_POTENTIOMETERS; i ++)
    {
        in->pot[i].target = 0;
        in->pot[i].outputOffset = 0;
        in->pot[i].outputRange = 1;
    }

}

static void update_potentiometer(potState_t *pot)
{
    float r  = ((float)adc_read() * ONE_BY_POTENTIOMETER_MAX_READING) ;
    if(r > 1)
        r = 1;
    r *= pot->outputRange;
    r += pot->outputOffset;
    *(pot->target) = (*(pot->target) * POT_SMOOTHING) +  ((r ) * (1.0f - POT_SMOOTHING));
    
    
}

static uint8_t find_last_unpressed_key(inputState_t *in)
{
    uint8_t res = KEY_UNPRESSED;
    uint64_t mostRecentKeyStamp = 0;
    
    for(int i = 0; i < NR_KEYS; i ++)
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

void input_update(inputState_t *in)
{
    static uint8_t lastCheckedPoti = 0;
    update_potentiometer(&in->pot[lastCheckedPoti++]);
    if(lastCheckedPoti >= NR_POTENTIOMETERS)
    {
        lastCheckedPoti = 0;
    }

    if(!in->eventsConsumed)
    {
        return;
    }
    in->eventsConsumed = false;
    in->activeKeyEvent = KEY_EVENT_NOTHING;

    for(int i = 0; i < NR_KEYS; i ++)
    {
        in->keys[i].event = KEY_EVENT_NOTHING;
        if(inputPressed(inputMap[i]))
        {   
            if(in->activeKey == KEY_UNPRESSED)
            {
                in->activeKeyEvent = KEY_EVENT_PRESSED;

            }
            if(!in->keys[i].pressed)
            {
                //newly pressed key
                in->keys[i].pressed = true;
                in->keys[i].event = KEY_EVENT_PRESSED;
                in->keys[i].pressedLastAtSample = tick_counter;
                in->activeKey = i;
            }
        }  
        else{
            if(in->keys[i].pressed)
            {
                //key just released
                in->keys[i].event = KEY_EVENT_RELEASED;
                in->keys[i].pressed = false;
            }
            if(in->activeKey == i)
            {   
                in->activeKey = find_last_unpressed_key(in);
                if(in->activeKey == KEY_UNPRESSED)
                {
                    in->activeKeyEvent = KEY_EVENT_RELEASED;
                }
            }
        }
    }
}