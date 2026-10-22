#include "stm32f4xx.h"

int main(void)
{
    // 1- Enable GPIOA, GPIOB, and GPIOC clocks
    RCC->AHB1ENR |= (1U << 0);  // Enable clock for GPIOA (1U << 0 corresponds to GPIOAEN)
    RCC->AHB1ENR |= (1U << 1);  // Enable clock for GPIOB (1U << 1 corresponds to GPIOBEN)
    RCC->AHB1ENR |= (1U << 2);  // Enable clock for GPIOC (1U << 2 corresponds to GPIOCEN)

    // 2- Set PC14 as output
    GPIOC->MODER &= ~(0x3 << 28); // Clear mode bits for PC14 (28 is the bit position)
    GPIOC->MODER |= (0x1 << 28);  // Set PC14 to output mode (01)

    // 3- Set PA6 and PB10 as input
    GPIOA->MODER &= ~(0x3 << 12); // Set PA6 to input mode (12 is the bit position for PA6)
    GPIOB->MODER &= ~(0x3 << 20); // Set PB10 to input mode (20 is the bit position for PB10)

    // Enable internal pull-up resistors for PA6 and PB10
    GPIOA->PUPDR &= ~(0x3 << 12); // Clear pull-up/pull-down bits for PA6
    GPIOA->PUPDR |= (0x1 << 12);  // Set pull-up for PA6 (01)

    GPIOB->PUPDR &= ~(0x3 << 20); // Clear pull-up/pull-down bits for PB10
    GPIOB->PUPDR |= (0x1 << 20);  // Set pull-up for PB10 (01)

    // 4- Enable SYSCFG clock
    RCC->APB2ENR |= (1U << 14); // Enable SYSCFG clock (1U << 14 corresponds to SYSCFGEN)

    // 5- Configure EXTI for PA6 and PB10
    SYSCFG->EXTICR[1] &= ~(0xF << 8); // Clear EXTI6 configuration (PA6)
    SYSCFG->EXTICR[1] |= (0x0 << 8);  // Set PA6 as the source input for EXTI6

    SYSCFG->EXTICR[2] &= ~(0xF << 8); // Clear EXTI10 configuration (PB10)
    SYSCFG->EXTICR[2] |= (0x1 << 8);  // Set PB10 as the source input for EXTI10

    // 6- Enable the Interrupt Mask register
    EXTI->IMR |= (1 << 6);   // Unmask EXTI line 6 (PA6)
    EXTI->IMR |= (1 << 10);  // Unmask EXTI line 10 (PB10)

    // 7- Select the Interrupt Trigger
    EXTI->FTSR |= (1 << 6);  // PA6 as Falling trigger
    EXTI->FTSR |= (1 << 10); // PB10 as Falling trigger

    // 8- NVIC Enable
    NVIC_EnableIRQ(EXTI9_5_IRQn);  // Enable EXTI9_5 interrupt (PA6 covers line 5 to 9)
    NVIC_EnableIRQ(EXTI15_10_IRQn); // Enable EXTI15_10 interrupt (PB10 covers line 10 to 15)

    while (1)
    {
        // Main loop - do nothing, waiting for interrupts
    }
}

// EXTI9_5 interrupt handler (PA6 is on EXTI line 6)
void EXTI9_5_IRQHandler(void)
{
    if (EXTI->PR & (1 << 6)) // Check if interrupt is pending on EXTI6
    {
        EXTI->PR |= (1 << 6); // Clear pending interrupt on EXTI6

        // Turn on LED on PC14
        GPIOC->ODR |= (1 << 14);
    }
}

// EXTI15_10 interrupt handler (PB10 is on EXTI line 10)
void EXTI15_10_IRQHandler(void)
{
    if (EXTI->PR & (1 << 10)) // Check if interrupt is pending on EXTI10
    {
        EXTI->PR |= (1 << 10); // Clear pending interrupt on EXTI10

        // Turn off LED on PC14
        GPIOC->ODR &= ~(1 << 14);
    }
}


#include "stm32f4xx.h"

int main(void)
{
    // Enable GPIOA, GPIOB, GPIOC clocks
    RCC->AHB1ENR |= (1 << 0);
    RCC->AHB1ENR |= (1 << 1);
    RCC->AHB1ENR |= (1 << 2);

    // PC14 = OUTPUT (LED)
    GPIOC->MODER &= ~(3 << 28);
    GPIOC->MODER |=  (1 << 28);

    // PA6 = INPUT
    GPIOA->MODER &= ~(3 << 12);

    // PB10 = INPUT
    GPIOB->MODER &= ~(3 << 20);

    // PA6 pull-up
    GPIOA->PUPDR &= ~(3 << 12);
    GPIOA->PUPDR |=  (1 << 12);

    // PB10 pull-up
    GPIOB->PUPDR &= ~(3 << 20);
    GPIOB->PUPDR |=  (1 << 20);

    // Enable SYSCFG clock
    RCC->APB2ENR |= (1 << 14);

    // PA6 -> EXTI6
    SYSCFG->EXTICR[1] &= ~(0xF << 8);

    // PB10 -> EXTI10
    SYSCFG->EXTICR[2] &= ~(0xF << 8);
    SYSCFG->EXTICR[2] |=  (1 << 8);

    // Enable EXTI6 and EXTI10
    EXTI->IMR |= (1 << 6);
    EXTI->IMR |= (1 << 10);

    // Falling edge trigger
    EXTI->FTSR |= (1 << 6);
    EXTI->FTSR |= (1 << 10);

    // Enable NVIC interrupts
    NVIC_EnableIRQ(EXTI9_5_IRQn);
    NVIC_EnableIRQ(EXTI15_10_IRQn);

    // Main loop
    while (1)
    {
    }
}


// PA6 button → LED ON
void EXTI9_5_IRQHandler(void)
{
    if (EXTI->PR & (1 << 6))
    {
        EXTI->PR |= (1 << 6);

        GPIOC->ODR |= (1 << 14);
    }
}


// PB10 button → LED OFF
void EXTI15_10_IRQHandler(void)
{
    if (EXTI->PR & (1 << 10))
    {
        EXTI->PR |= (1 << 10);

        GPIOC->ODR &= ~(1 << 14);
    }
}
