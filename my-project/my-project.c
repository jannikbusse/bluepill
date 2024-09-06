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
void wait(void);
void shortwait(void);

static inline void write_bit(uint8_t val)
{
	if(val)
	{
		gpio_set(GPIOB, DATA_DATA);
	}else {
		gpio_clear(GPIOB, DATA_DATA);
	}
	shortwait();
	shortwait();
	shortwait();
	gpio_toggle(GPIOB, DATA_CLK);
	shortwait();
	shortwait();
	shortwait();

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
	shortwait();
	shortwait();
	shortwait();

	shortwait();
	shortwait();
	shortwait();
}

void print_str(char* str, int strlen)
{
	uint16_t outPos = 0;
	for(uint16_t outPos = 0; outPos < strlen; outPos ++)
	{
		write_byte(str[outPos]);

	}
	shortwait();
	shortwait();

	shortwait();
	shortwait();
	shortwait();
	write_byte(0);
}

void wait()
{
	for(int i = 0; i < 1000000; i ++)
	{
		__asm__("nop");
	}
}
void shortwait()
{
	for(int i = 0; i < 50; i ++)
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
	for(int i = 0; i < 10000090; i ++)
	{
		__asm__("nop");
	}

	while(1)
	{
		gpio_toggle(GPIOC,GPIO13);
		// gpio_toggle(GPIOB,DATA_CLK);
		print_str("AFAFF", 3);
		wait();
		wait();
		wait();
		wait();
		wait();
		wait();
		wait();
		wait();
		wait();
		wait();
		wait();
		wait();
		wait();
		
	}

}
