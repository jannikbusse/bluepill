#ifndef __DAC__
#define __DAC__

#include <libopencm3/stm32/rcc.h>
#include <libopencm3/stm32/gpio.h>
#include <libopencm3/stm32/flash.h>
#include <libopencm3/stm32/spi.h>
#include <libopencm3/cm3/nvic.h>

#include <libopencm3/stm32/dma.h>
#include "common.h"

#define BUFF_SIZE  256
#define OUT_BUFFER_EMPTY    (bufferHead == bufferTail)
#define OUT_BUFFER_FULL     (((bufferHead + 2) & (BUFF_SIZE-1)) == bufferTail)


#define SPI_PORT GPIOA

#define I2S2_SCK  GPIO13
#define I2S2_SLSEL GPIO12
#define I2S2_MOSI GPIO15

#define SAI1_SCK_B_BLOCK_B  GPIO12 //B
#define SAI1_SD_B_BLOCK_A   GPIO9 //A
#define SAI1_FS_B_BLOCK_B   GPIO9 //B


#define SAI1_GCR  	MMIO32(SAI1_BASE + 0x0)
#define SAI1_BCR1  	MMIO32(SAI1_BASE + 0x024)
#define SAI1_BCR2  	MMIO32(SAI1_BASE + 0x028)
#define SAI_BFRCR  	MMIO32(SAI1_BASE + 0x02C) //Reset value: 0x0000 0007
#define SAI_BSLOTR	MMIO32(SAI1_BASE + 0x030) //Reset value: 0
#define SAI_BDR  	MMIO32(SAI1_BASE + 0x040) //data register
#define SAI_STATUS  MMIO32(SAI1_BASE + 0x038) 

extern volatile uint32_t outputBuffer[];
extern volatile uint16_t bufferHead;
extern volatile uint16_t bufferTail;

void init_dac(void);



inline __attribute__((always_inline)) uint32_t dac_rel_to_abs_voltage(float voltage)
{
    return ((uint32_t) (voltage *  3777215.f ) )<<8;
}

inline __attribute__((always_inline)) void music_write_to_buffer(audio_sample_t *out)
{
    
    outputBuffer[bufferHead++] = dac_rel_to_abs_voltage(out->left);
    bufferHead &= (BUFF_SIZE - 1);
    outputBuffer[bufferHead++] = dac_rel_to_abs_voltage(out->right);
    bufferHead &= (BUFF_SIZE - 1);
}   

#endif