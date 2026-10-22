#include "stm32f4xx.h"
uint8_t data;
int main()
{		// PIN Configuration
    RCC->AHB1ENR |= (1<<0);    // GPIO clock enabled for UART Rx Pin PA3 & Tx Pin PA2 - GPIOA
    GPIOA->MODER |= (1<<5);    // Alternate function mode select for USART2 Tx Pin PA2
    GPIOA->MODER |= (1<<7);    // Alternate function mode select for USART2 Rx Pin PA3
	GPIOA->AFR[0] &= (~(1<<15));   // Alternate function select mode for USART2 Rx Pin PA3
  GPIOA->AFR[0] |= (1<<14) | (1<<13) | (1<<12); // Alternate function select mode to be UART Rx channel
	GPIOA->AFR[0] &= (~(1<<11));   // Alternate function select mode for USART2 Tx Pin PA2
  GPIOA->AFR[0] |= (1<<10) | (1<<9) | (1<<8); // Alternate function select mode to be UART Tx channel
		// USART Configuration
    RCC->APB1ENR |= (1<<17);   // USART2 clock enable
    USART2->BRR = 0x0683; // Baudrate of USART2 to be 9600 @ 16 Mhz
    USART2->CR1 |= (1<<2);     // Rx enable  
	  USART2->CR1 |= (1<<3);     // Tx enable
    USART2->CR1 |= (1<<13);    // USART2 enable
    while(1)
    {
				while(!(USART2->SR & (1U<<5))) {};  // wait for Rx buffer is not emptiness
        data = USART2->DR;                  // Receive byte of data to USART2 Bus        
				while(!(USART2->SR & (1U<<7))) {};  // wait for Tx buffer emptiness
        USART2->DR = data;                   // Write recieved byte to USART2 Bus
    }
}
