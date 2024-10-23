#include "adc.h"

const float ONE_BY_POTENTIOMETER_MAX_READING = 1.f/POTENTIOMETER_MAX_READING;

void init_adc()
{
    rcc_periph_clock_enable(RCC_ADC1); 
    gpio_mode_setup(GPIOC, GPIO_MODE_ANALOG, GPIO_PUPD_NONE, GPIO1);

    adc_power_off(ADC1);                           // Power off the ADC to configure it
    adc_disable_scan_mode(ADC1);                   // Single channel mode
    adc_set_single_conversion_mode(ADC1);          // Single conversion mode
    adc_set_sample_time_on_all_channels(ADC1, ADC_SMPR_SMP_3CYC);
    adc_power_on(ADC1);
}
