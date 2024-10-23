#include "dac.h"


volatile uint32_t outputBuffer[BUFF_SIZE];
volatile uint8_t bufferHead = 0;
volatile uint8_t bufferTail = 0;



void dma2_stream4_isr(void) {

    if (dma_get_interrupt_flag(DMA2, DMA_STREAM4, DMA_TCIF)) {
        // Clear the interrupt flag
        dma_clear_interrupt_flags(DMA2, DMA_STREAM4, DMA_TCIF);
        bufferTail = 0;

        

    }
    if (dma_get_interrupt_flag(DMA2, DMA_STREAM4, DMA_HTIF)) {
        // Clear the interrupt flag
        dma_clear_interrupt_flags(DMA2, DMA_STREAM4, DMA_HTIF);

        bufferTail =  BUFF_SIZE/2;
    }
}

void init_dac()
{


   	rcc_periph_clock_enable(RCC_SPI2);
    rcc_periph_clock_enable(RCC_DMA2); //DMA
	

	rcc_osc_on(RCC_PLLSAI);

	rcc_pllsai_config(317, 0, 14, 4);


    gpio_mode_setup(GPIOB, GPIO_MODE_AF, GPIO_PUPD_NONE, SAI1_FS_B_BLOCK_B | SAI1_SCK_B_BLOCK_B );
    gpio_mode_setup(GPIOA, GPIO_MODE_AF, GPIO_PUPD_NONE, SAI1_SD_B_BLOCK_A );
	
    gpio_set_af(GPIOB, GPIO_AF6, SAI1_FS_B_BLOCK_B | SAI1_SCK_B_BLOCK_B); 
    gpio_set_af(GPIOA, GPIO_AF6, SAI1_SD_B_BLOCK_A); 

	//dma setup

    dma_stream_reset(DMA2, DMA_STREAM4);
    nvic_enable_irq(NVIC_DMA2_STREAM4_IRQ);
    nvic_set_priority(NVIC_DMA2_STREAM4_IRQ, 1); // Set interrupt priority 
    dma_enable_transfer_complete_interrupt(DMA2, DMA_STREAM4);
    dma_enable_half_transfer_interrupt(DMA2, DMA_STREAM4);
    dma_set_transfer_mode(DMA2, DMA_STREAM4, DMA_SxCR_DIR_MEM_TO_PERIPHERAL);
    dma_set_peripheral_address(DMA2, DMA_STREAM4, (uint32_t)&SAI_BDR);  // I2S data register address
    dma_set_memory_address(DMA2, DMA_STREAM4, (uint32_t)outputBuffer); 
    dma_set_number_of_data(DMA2, DMA_STREAM4, BUFF_SIZE);
	dma_enable_fifo_mode(DMA2, DMA_STREAM4);
    // Enable circular mode
    dma_enable_circular_mode(DMA2, DMA_STREAM4);
    dma_set_priority(DMA2, DMA_STREAM4, DMA_SxCR_PL_HIGH);
	dma_channel_select(DMA2, DMA_STREAM4, DMA_SxCR_CHSEL_1);
    dma_set_memory_size(DMA2, DMA_STREAM4, DMA_SxCR_MSIZE_32BIT);
    dma_set_peripheral_size(DMA2, DMA_STREAM4, DMA_SxCR_PSIZE_32BIT);
    dma_enable_memory_increment_mode(DMA2, DMA_STREAM4);

    //SAI configuration
	// RCC_APB2RSTR |= RCC_APB2RSTR_SAI1RST; //reset sai1
	RCC_APB2ENR |= RCC_APB2ENR_SAI1EN;	//enable sai1

	//RCC_PLLI2SCFGR = 0b0010 <<24; div factor. page 164 in rm
	//RCC_PLLSAICFGR = 0b0010 <<24; div factor. page 164 in rm

	SAI1_BCR1 = 0b111 << 5 //DS data size  111=32, 110=24
				| 1 << 9 //CKSTR: rising edge
				| 1 << 17 //DMA enable (0 = off)
				| 0 << 19 //No divider used in the clock generator (in this case Master Clock Divider bit has no effect)
				| 0 << 20 //Master clock divider
				;


	//frame config register
	SAI_BFRCR = 63 << 0// frame length for 64 bits. the number of bits in the frame is equal to FRL[7:0] + 1
				|31	<< 8 //FSALL[6:0]: Frame synchronization active level length. These bits are set and cleared by software. They specify the length in number of bit clock
						 //(SCK) + 1 (FSALL[6:0] + 1) of the active level of the FS signal in the audio frame
				|1 << 16 //FSDEF: Frame synchronization def - this is correct
				|1 << 17 //FSPOL: //this is correct
				|1 << 18 //FSOFF:	//probably correct
				;

	SAI_BSLOTR 	= 0b11 << 16 //SLOTENenable two slots
			 	| 0b1 << 8	//NBSLOT The value set in this bitfield represents the number of slots + 1 in the audio frame (including the
							//number of inactive slots). The maximum number of slots is 16.
				| 0b10 << 6	//10: 32-bit slot size 
				| 0b0 << 0 //first bit offset
				;

	dma_enable_stream(DMA2, DMA_STREAM4);
	SAI1_BCR1 |= 1 << 16; //enable sai1	


}

