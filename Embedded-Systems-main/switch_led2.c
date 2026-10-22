#include "stm32f4xx.h"

void init_port() {
	RCC->AHB1ENR |= (1U<<0);
	RCC->AHB1ENR |= (1U<<1);
	RCC->AHB1ENR |= (1U<<2);
	
	GPIOA->MODER |= (0U<<0);//pa0
	GPIOA->MODER |= (0U<<12);//pa6
	GPIOB->MODER |= (1U<<18);//pb9
	GPIOC->MODER |= (1U<<28);//pc14
	
	GPIOA->PUPDR &= ~(3U<<0);
    GPIOA->PUPDR |= (1U<<0);
	GPIOA->PUPDR &= ~(3U<<12);
    GPIOA->PUPDR |= (1U<<12);
}	

int main(void) {
	init_port();
	while(1) {
		if(GPIOA->IDR & (1U<<0)) {
			GPIOC->ODR |= (1U<<14);
		}
		else if (GPIOA->IDR & (1U<<6)) {
			GPIOC->ODR &= ~(1U<<14);
			GPIOB->ODR |= (1U<<9);
			for(int i =0; i<500000; i++){}
			GPIOB->ODR &= ~(1U<<9);
		}
	}
	return 0;
}
