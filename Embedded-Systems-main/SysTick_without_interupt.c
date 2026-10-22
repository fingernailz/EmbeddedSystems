#include "stm32f4xx.h"
int main(void) {
    // Enable GPIOC clock
   RCC->AHB1ENR |= (1 << 2);  // Bit 2 enables the clock for GPIOC
    
    // Clear pin mode for PC14 (GPIOC pin 14)
    GPIOC->MODER &= ~0x30000000;
    
    // Set PC14 to output mode
    GPIOC->MODER |= 0x10000000;
    
    // Configure SysTick to generate 1Hz toggle
    SysTick->LOAD = 16000000 - 1;  // Reload with number of clocks per second (assuming 16 MHz clock)
    SysTick->VAL = 0;              // Clear current value register
    SysTick->CTRL = 5;             // Enable SysTick, no interrupt, use processor clock
    
    while (1) {
        // Wait for COUNT flag to be set
        if (SysTick->CTRL & 0x00010000) {
            // Toggle PC14
            GPIOC->ODR ^= (1 << 14);  // Toggle PC14
        }
    }
}
