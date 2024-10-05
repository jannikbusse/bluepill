#include "dac.h"





void write_to_data_register(uint16_t word)
{
    while (!SPI1_T_READY) {}
    gpio_clear(GPIOA, SPI1_SLAVE_SELECT_DAC);
    SPI1_DR = word;
    while (!SPI1_T_READY) {}
    for(int i = 0; i < 7 ; i++)
		{
			__asm volatile ("nop");
		}
    gpio_clear(GPIOA, SPI1_LDAC);

    gpio_set(GPIOA, SPI1_LDAC);
    gpio_set(GPIOA, SPI1_SLAVE_SELECT_DAC);
}



void init_dac()
{
  rcc_periph_clock_enable(RCC_SPI1);

    /* Configure GPIOs: SS=PA4, SCK=PA5, MISO=PA6 and MOSI=PA7 */
  gpio_set_mode(GPIOA, GPIO_MODE_OUTPUT_50_MHZ, GPIO_CNF_OUTPUT_PUSHPULL, SPI1_SLAVE_SELECT_DAC | SPI1_LDAC);
  
  gpio_set_mode(GPIOA, GPIO_MODE_OUTPUT_50_MHZ,
          GPIO_CNF_OUTPUT_ALTFN_PUSHPULL, GPIO4 |
                                          GPIO5 |
                                          GPIO7
                                          );

  gpio_set_mode(GPIOA, GPIO_MODE_INPUT, GPIO_CNF_INPUT_FLOAT,
          GPIO6);
  gpio_set(GPIOA, SPI1_SLAVE_SELECT_DAC);
  gpio_set(GPIOA, SPI1_LDAC);

  /* Reset SPI, SPI_CR1 register cleared, SPI is disabled */
  rcc_periph_reset_pulse(RST_SPI1);

  
    AFIO_MAPR &= ~(0x1);

    SPI1_CR1 |= 1 << 2; //configure in master mode
    SPI1_CR1 |= 1 << 11; //16 bit format
    // SPI1_CR1 |= 0b101 << 3;
    // SPI1_CR1 |= SPI_CR1_CPHA_CLK_TRANSITION_2;
    SPI1_CR1 |= 1 << 14; //output only mode. might not be needed
    SPI1_CR1 |= 1 << 15; //output only mode. might not be needed


    spi_enable_software_slave_management(SPI1);
    spi_set_nss_high(SPI1);

    spi_enable(SPI1);

}

