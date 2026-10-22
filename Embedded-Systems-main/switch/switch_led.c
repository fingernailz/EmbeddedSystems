#include "stm32f4xx.h"

static void init_port(void)
{
    /* 1. Enable GPIOA clock */
    RCC->AHB1ENR |= (1U << 0);

    /* 2. Enable GPIOC clock */
    RCC->AHB1ENR |= (1U << 2);

    /* 3. Configure PA0 as input */
    GPIOA->MODER &= ~(3U << 0);

    /* 4. Configure PA0 with pull-up */
    GPIOA->PUPDR &= ~(3U << 0);  // Clear bits 1 and 0
    GPIOA->PUPDR |=  (1U << 0);  // Set pull-up

    /* 5. Configure PC13 as output */
    GPIOC->MODER &= ~(3U << 26); // Clear bits 27 and 26
    GPIOC->MODER |=  (1U << 26); // Set output mode
}

int main(void)
{
    init_port();

    while (1)
    {
        /* 6. Check whether PA0 is HIGH */
        if (GPIOA->IDR & (1U << 0))
        {
            /* 7. Turn ON PC13 (active-low LED) */
            GPIOC->ODR &= ~(1U << 13);
        }
        else
        {
            /* 8. Turn OFF PC13 */
            GPIOC->ODR |= (1U << 13);
        }
    }
}
