#include "dac.h"


volatile uint32_t outputBuffer[BUFF_SIZE];
volatile uint8_t bufferHead = 0;
volatile uint8_t bufferTail = 0;



void dma2_stream4_isr(void) {

    if (dma_get_interrupt_flag(DMA2, DMA_STREAM4, DMA_TCIF)) {
        // Clear the interrupt flag
        dma_clear_interrupt_flags(DMA2, DMA_STREAM4, DMA_TCIF);
        bufferTail = BUFF_SIZE/2;

        

    }
    if (dma_get_interrupt_flag(DMA2, DMA_STREAM4, DMA_HTIF)) {
        // Clear the interrupt flag
        dma_clear_interrupt_flags(DMA2, DMA_STREAM4, DMA_HTIF);
        gpio_toggle(GPIOB, GPIO2);

         bufferTail = 0;
    }
}

void init_dac()
{


    rcc_periph_clock_enable(RCC_SPI2);
    rcc_periph_clock_enable(RCC_DMA1); //DMA
    rcc_osc_on(RCC_PLLI2S);
	rcc_plli2s_config(79, 7);


    

    gpio_mode_setup(GPIOB, GPIO_MODE_AF, GPIO_PUPD_NONE, I2S2_SCK | I2S2_SLSEL | I2S2_MOSI);
    gpio_set_af(GPIOB, GPIO_AF5, I2S2_SCK | I2S2_SLSEL | I2S2_MOSI); // Set alternate function for I2S (AF5 for SPI2/I2S2)


    SPI_I2SCFGR(SPI2) = SPI_I2SCFGR_I2SMOD |    // Enable I2S mode
                    (0b10)<<8 | // Set to master transmit mode
                    0|  // Standard Philips I2S format | 
                    SPI_I2SCFGR_DATLEN_32BIT |
                    1;  

    SPI_I2SPR(SPI2) =  1; // Example prescaler value (change based on your frequency)
    SPI_CR2(SPI2) |= SPI_CR2_TXDMAEN;
    while (!(RCC_CR & RCC_CR_PLLI2SRDY));  // Wait for PLLI2S to be ready


      //dma setup
    dma_stream_reset(DMA1, DMA_STREAM4);
    nvic_enable_irq(NVIC_DMA1_STREAM4_IRQ);
    nvic_set_priority(NVIC_DMA1_STREAM4_IRQ, 1); // Set interrupt priority 
    dma_enable_transfer_complete_interrupt(DMA1, DMA_STREAM4);
    dma_enable_half_transfer_interrupt(DMA1, DMA_STREAM4);
    dma_set_transfer_mode(DMA1, DMA_STREAM4, DMA_SxCR_DIR_MEM_TO_PERIPHERAL);

    dma_set_peripheral_address(DMA1, DMA_STREAM4, (uint32_t)&SPI_DR(SPI2));  // I2S data register address
    dma_set_memory_address(DMA1, DMA_STREAM4, (uint32_t)outputBuffer); 
    dma_set_number_of_data(DMA1, DMA_STREAM4, BUFF_SIZE);
    // Enable circular mode
    dma_enable_circular_mode(DMA1, DMA_STREAM4);
     // Set DMA priority
    dma_set_priority(DMA1, DMA_STREAM4, DMA_SxCR_PL_HIGH);
    
    dma_set_memory_size(DMA1, DMA_STREAM4, DMA_SxCR_MSIZE_32BIT);
    dma_set_peripheral_size(DMA1, DMA_STREAM4, DMA_SxCR_PSIZE_16BIT);
    dma_enable_memory_increment_mode(DMA1, DMA_STREAM4);
    dma_enable_stream(DMA1, DMA_STREAM4);

    //end dma setup

  
    SPI_I2SCFGR(SPI2) |= SPI_I2SCFGR_I2SE;

}

