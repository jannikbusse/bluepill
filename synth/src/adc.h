#ifndef ADC_H
#define ADC_H


#include <libopencm3/stm32/rcc.h>
#include <libopencm3/stm32/gpio.h>
#include <libopencm3/stm32/adc.h>

#define POTENTIOMETER_MAX_READING 4000.f
extern const float ONE_BY_POTENTIOMETER_MAX_READING;

void init_adc(void);


inline __attribute__((always_inline)) uint16_t adc_read(void)
{
    uint8_t channels[16]; 
    channels[0] =ADC_CHANNEL11;
    adc_set_regular_sequence(ADC1, 1, channels);
    adc_start_conversion_regular(ADC1);
    while (!adc_eoc(ADC1));
    uint16_t r = adc_read_regular(ADC1);
    return r;
}


#endif // ADC_H