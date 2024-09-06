#define STM32F1 1
#include <libopencm3/stm32/rcc.h>
#include <libopencm3/stm32/gpio.h>

#define DATA_CLK 	GPIO10
#define DATA_DATA 	GPIO11

char outstr1[] = "IHUUHU";
char outstr2[] = "hehehe";
char outstr3[] = "hhuhu";

void print_str(char* str, int strlen);
static inline void write_byte(uint8_t b); 
static inline void write_bit(uint8_t val);


static inline void write_bit(uint8_t val)
{
	if(val)
	{
		gpio_set(GPIOB, DATA_DATA);
	}else {
		gpio_clear(GPIOB, DATA_DATA);
	}
	wait();

	gpio_toggle(GPIOB, DATA_CLK);
	wait();

}

static inline void write_byte(uint8_t b)
{
	write_bit(b & (1 << 0));
	write_bit(b & (1 << 1));
	write_bit(b & (1 << 2));
	write_bit(b & (1 << 3));
	write_bit(b & (1 << 4));
	write_bit(b & (1 << 5));
	write_bit(b & (1 << 6));
	write_bit(b & (1 << 7));
}

void print_str(char* str, int strlen)
{
	uint16_t outPos = 0;
	for(outPos = 0; outPos < strlen; outPos ++)
	{
		write_byte(str[outPos]);
		wait();

	}
	write_byte('^');
}

void wait()
{
	for(int i = 0; i < 1000000; i ++)
	{
		__asm__("nop");
	}
}

int main(void) {
	rcc_periph_clock_enable(RCC_GPIOB);
	rcc_periph_clock_enable(RCC_GPIOC);

	//set mode to output
	gpio_set_mode(GPIOC, GPIO_MODE_OUTPUT_2_MHZ, GPIO_CNF_OUTPUT_PUSHPULL, GPIO13);
	gpio_set_mode(GPIOB, GPIO_MODE_OUTPUT_2_MHZ, GPIO_CNF_OUTPUT_PUSHPULL, DATA_CLK);
	gpio_set_mode(GPIOB, GPIO_MODE_OUTPUT_2_MHZ, GPIO_CNF_OUTPUT_PUSHPULL, DATA_DATA);
	
	gpio_set(GPIOC,GPIO13);
	// gpio_toggle(GPIOB,GPIO1);
	for(int i = 0; i < 10000090; i ++)
	{
		__asm__("nop");
	}
	gpio_clear(GPIOC,GPIO13);

	while(1)
	{
		// gpio_toggle(GPIOC,GPIO13);
		print_str(outstr1, 5);
		for(int i = 0; i < 10000000; i ++)
		{
			__asm__("nop");
		}
		
	}

}
