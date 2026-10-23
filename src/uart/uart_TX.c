#include "stm32f446xx.h"
int main()
{		// PIN Configuration
    RCC->AHB1ENR |= (1<<0);    // GPIO clock enabled for UART Tx Pin PA2 - GPIOA
    GPIOA->MODER |= (1<<5);    // Alternate function mode select for USART2 Tx Pin PA2
    GPIOA->AFR[0] &= (~(1<<11));   // Alternate function select mode for USART2 Tx Pin PA2
    GPIOA->AFR[0] |= (1<<10) | (1<<9) | (1<<8); // Alternate function select mode to be UART Tx channel
		// USART Configuration
    RCC->APB1ENR |= (1<<17);   // USART2 clock enable
    USART2->BRR = 0x008B; // Baudrate of USART2 to be 115200 @ 16 Mhz
    USART2->CR1 |= (1<<3);     // Tx enable
    USART2->CR1 |= (1<<13);    // USART2 enable
    while(1)
    {
        while(!(USART2->SR & (1U<<7))) {};  // wait for Tx buffer emptiness
        USART2->DR = 'V';                   // Write byte to USART2 Bus
    }
}
