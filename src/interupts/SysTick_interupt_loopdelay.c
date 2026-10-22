#include "stm32f4xx.h"

volatile int count = 0;

int main(void)
{
    // Enable GPIOC clock
    RCC->AHB1ENR |= (1U << 2);

    // Configure PC14 as output
    GPIOC->MODER &= ~(3U << 28);
    GPIOC->MODER |= (1U << 28);

    // Configure SysTick for 1 second
    SysTick->LOAD = 16000000 - 1;
    SysTick->VAL = 0;
    SysTick->CTRL = 7;

    while (1)
    {
        // Wait for the interrupt handler
    }
}

void SysTick_Handler(void)
{
    count++;

    if (count >= 3)
    {
        GPIOC->ODR ^= (1U << 14);
        count = 0;
    }
}