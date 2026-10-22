#include "stm32f4xx.h"
static void init_port()
{
	//1. Enable GPIOC clock by Writing 1 to bit2 of AHB1ENR
	RCC->AHB1ENR |=(1U<<2);
	//2. Set PC13 to output mode by writing 1 to bit26 of MODER
	GPIOC->MODER |=(1U<<26);
}

int main(void) {
	init_port();
	while (1) {
		//Turn on PC13 by writing 1 to bit13 of ODR
		GPIOC->ODR |= (1U<<13);
		//Delay for some time/
		for(int i =0; i<500000; i++) {}
		//Turn off PC13 by writing 0 to bit13 of ODR
		GPIOC->ODR &= ~(1U<<13);
		//Delay for some time/
		for(int j =0; j<500000; j++) {}
	}
}
