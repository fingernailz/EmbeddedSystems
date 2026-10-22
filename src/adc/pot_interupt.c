#include "stm32f4xx.h"

volatile int adcData;
const int threshold = 1024;

int main(void)
{
    // Enable GPIOA, GPIOC and ADC1 clocks
    RCC->AHB1ENR |= (1U << 0);
    RCC->AHB1ENR |= (1U << 2);
    RCC->APB2ENR |= (1U << 8);

    // PA0 as analog input
    GPIOA->MODER |= (3U << 0);

    // PC14 as output
    GPIOC->MODER &= ~(1U << 29);
    GPIOC->MODER |= (1U << 28);

    // ADC configuration
    ADC1->SMPR2 = 0;
    ADC->CCR = 0;

    ADC1->CR1 = 0;
    ADC1->CR2 = 0;

    ADC1->CR2 |= (1U << 1);     // Continuous conversion
    ADC1->SQR3 = 0;             // Channel 0
    ADC1->SQR1 = 0;  // conversion length

    // Enable EOC interrupt
    ADC1->CR1 |= (1U << 5);

    // Enable ADC interrupt in NVIC
    NVIC_EnableIRQ(ADC_IRQn);

    // Enable ADC
    ADC1->CR2 |= (1U << 0);

    // Start conversion
    ADC1->CR2 |= (1U << 30);

    while (1)
    {
    }
}


void ADC_IRQHandler(void)
{
    if (ADC1->SR & (1U << 1))
    {
        adcData = ADC1->DR;

        if (adcData > threshold)
        {
            GPIOC->ODR |= (1U << 14);
        }
        else
        {
            GPIOC->ODR &= ~(1U << 14);
        }
    }
}
