#ifndef __DAC__
#define __DAC__

#define SPI1_T_READY (SPI1_SR & (1 << 1))
#define SPI1_BUSY (SPI1_SR & (1 << 7))

#include <libopencm3/stm32/rcc.h>
#include <libopencm3/stm32/gpio.h>
#include <libopencm3/stm32/flash.h>
#include <libopencm3/stm32/spi.h>

//page 181 in manual. SPI1_REMAP = 0 for SPI functionality (?)

#define SPI_PORT GPIOA

#define SPI1_NSS  GPIO4
#define SPI1_SCK  GPIO5
#define SPI1_MISO GPIO6
#define SPI1_MOSI GPIO7
#define SPI1_LDAC GPIO2
#define SPI1_SLAVE_SELECT_DAC GPIO3

void init_dac(void);
void write_to_data_register(uint16_t word);


/// @brief Writes power to dac. 0 = 0 V, 4095=Vref
/// @param power 
void write_voltage_to_dac(uint16_t power);

void write_relative_voltage(float f);

#endif