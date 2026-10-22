#include "stm32f4xx.h"

int main(void)
{
    // Enable GPIOC clock
    RCC->AHB1ENR |= (1U << 2);

    // Clear pin mode for PC14
    GPIOC->MODER &= ~(3U << 28);

    // Set PC14 to output mode
    GPIOC->MODER |= (1U << 28);

    // Configure SysTick for a 1-second interval
    SysTick->LOAD = 16000000 - 1;
    SysTick->VAL = 0;

    // Enable SysTick, interrupt, and processor clock
    SysTick->CTRL = 7;

    while (1)
    {
        // Toggle handled by SysTick interrupt
    }
}

// SysTick interrupt handler
void SysTick_Handler(void)
{
    // Toggle PC14
    GPIOC->ODR ^= (1U << 14);
}
