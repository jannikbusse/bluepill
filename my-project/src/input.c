#include "input.h"

bool pressed1 = false, pressed2 = false;

void init_input()
{
    gpio_set_mode(GPIOA, GPIO_MODE_INPUT, GPIO_CNF_INPUT_PULL_UPDOWN, INPUT1);
    gpio_set_mode(GPIOA, GPIO_MODE_INPUT, GPIO_CNF_INPUT_PULL_UPDOWN, INPUT2);
    gpio_clear(GPIOA, INPUT1);
    gpio_clear(GPIOA, INPUT2);

}

bool inputPressed(uint32_t btn)
{
    return gpio_get(GPIOA, btn);
}

void input_update()
{
    pressed1 = (inputPressed(INPUT1));
    pressed2 = (inputPressed(INPUT2));

}