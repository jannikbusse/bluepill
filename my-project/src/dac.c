#include "dac.h"

volatile uint16_t outputBuffer[256];
volatile uint8_t bufferHead = 0;
volatile uint8_t bufferTail = 0;


void write_to_data_register(uint16_t word)
{
    // while (!SPI1_T_READY) {}
    gpio_clear(GPIOA, SPI1_SLAVE_SELECT_DAC);
    SPI1_DR = word;
    while (!SPI1_T_READY) {}
    for(int i = 0; i < 5 ; i++)
		{
			__asm volatile ("nop");
		}
    gpio_clear(GPIOA, SPI1_LDAC);

    gpio_set(GPIOA, SPI1_LDAC);
    gpio_set(GPIOA, SPI1_SLAVE_SELECT_DAC);
}

void init_dac()
{

  //(DMA_SxNDTR)
  rcc_periph_clock_enable(RCC_SPI1);

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
  
  SPI1_CR2 |= SPI_CR2_TXDMAEN; //enable dma

  SPI1_CR1 |= 1 << 14; //output only mode. might not be needed
  SPI1_CR1 |= 1 << 15; //output only mode. might not be needed

  spi_enable_software_slave_management(SPI1);
  spi_set_nss_high(SPI1);
  spi_enable(SPI1);

  //config dma
  //enable dma bus. SPI1 transmit uses dma1 channel3
  // RCC_AHBENR |= RCC_AHBENR_DMA1EN;

  // dma_channel_reset(DMA1, DMA_CHANNEL3);
	// dma_set_peripheral_address(DMA1, DMA_CHANNEL3, (uint32_t) &SPI1_DR);
  // dma_set_memory_address(DMA1, DMA_CHANNEL3, (uint32_t)outputBuffer);         // Data buffer address
	// dma_set_number_of_data(DMA1, DMA_CHANNEL3, 1);
	// dma_set_read_from_memory(DMA1, DMA_CHANNEL3); 
  // dma_enable_memory_increment_mode(DMA1, DMA_CHANNEL3);  // Increment buffer pointer after each transfer
  // dma_enable_circular_mode(DMA1, DMA_CHANNEL3);
	// dma_set_peripheral_size(DMA1, DMA_CHANNEL3, DMA_CCR_PSIZE_16BIT);  // Peripheral size: 16-bit (for SPI)
  // dma_set_memory_size(DMA1, DMA_CHANNEL3, DMA_CCR_MSIZE_16BIT);  // Memory size: 16-bit (buffer element size)
  // dma_set_priority(DMA1, DMA_CHANNEL3, DMA_CCR_PL_HIGH);  // Set high priority
	// dma_enable_transfer_complete_interrupt(DMA1, DMA_CHANNEL3);
  // dma_enable_channel(DMA1, DMA_CHANNEL3);
	// nvic_enable_irq(NVIC_DMA1_CHANNEL3_IRQ);


}

