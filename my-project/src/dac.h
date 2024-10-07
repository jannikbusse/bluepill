#ifndef __DAC__
#define __DAC__


#define SPI1_T_READY (SPI1_SR & (1 << 1))
#define SPI1_BUSY (SPI1_SR & (1 << 7))

#define OUT_BUFFER_EMPTY    (bufferHead == bufferTail)
#define OUT_BUFFER_FULL     ((uint8_t)(bufferHead + 1) == bufferTail)

#include <libopencm3/stm32/rcc.h>
#include <libopencm3/stm32/gpio.h>
#include <libopencm3/stm32/flash.h>
#include <libopencm3/stm32/spi.h>

#include "common.h"

#define SPI_PORT GPIOA

#define SPI1_NSS  GPIO4
#define SPI1_SCK  GPIO5
#define SPI1_MISO GPIO6
#define SPI1_MOSI GPIO7
#define SPI1_LDAC GPIO2
#define SPI1_SLAVE_SELECT_DAC GPIO3

extern volatile uint16_t outputBuffer[256];
extern volatile uint8_t bufferHead;
extern volatile uint8_t bufferTail;

void init_dac(void);
void write_to_data_register(uint16_t word);




static inline __attribute__((always_inline)) void write_voltage_to_dac(uint16_t power)
{
      // while (!SPI1_T_READY) {}
    gpio_clear(GPIOA, SPI1_SLAVE_SELECT_DAC);
    SPI1_DR = power;
    while (!SPI1_T_READY) {}
    for(int i = 0; i < 5 ; i++)
		{
			__asm volatile ("nop");
		}
    gpio_clear(GPIOA, SPI1_LDAC);

    gpio_set(GPIOA, SPI1_LDAC);
    gpio_set(GPIOA, SPI1_SLAVE_SELECT_DAC);
}

// range 0 - FIXPOINT_DECIMAL_PLACES
static inline __attribute__((always_inline)) uint16_t dac_rel_to_abs_voltage(uint32_t voltage)
{
    //pre configure the config bits
    return (((4095 * voltage)/FIXPOINT_DECIMAL_PLACES) & ~(0b1111 << 12) ) | (0b0011 << 12) ;
    
}


#endif