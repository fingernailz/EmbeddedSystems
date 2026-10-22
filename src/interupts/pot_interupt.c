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
    ADC1->SQR3 = 0;             // Channel 0 pa-0-7 (0-7) pb0-1 (8-9) pc 1-5(10-15)
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

//eg pb1 ->pot
#include "stm32f4xx.h"

volatile int adcData;
const int threshold = 1024;

int main(void)
{
    // 1. Enable GPIOB, GPIOC, and ADC1 clocks
    RCC->AHB1ENR |= (1U << 1);   // Enable GPIOB clock
    RCC->AHB1ENR |= (1U << 2);   // Enable GPIOC clock
    RCC->APB2ENR |= (1U << 8);   // Enable ADC1 clock

    // 2. Configure PB1 as analog input
    GPIOB->MODER |= (3U << 2);   // PB1 analog mode (11)

    // 3. Configure PC14 as output (LED)
    GPIOC->MODER &= ~(3U << 28); // Clear PC14 mode bits
    GPIOC->MODER |=  (1U << 28); // Set PC14 as output (01)

    // 4. Configure ADC1
    ADC1->SMPR2 = 0;
    ADC->CCR = 0;

    ADC1->CR1 = 0;
    ADC1->CR2 = 0;

    ADC1->CR2 |= (1U << 1);      // Continuous conversion
    ADC1->SQR3 = 9;              // ADC channel 9 = PB1
    ADC1->SQR1 = 0;              // One conversion in the sequence

    // 5. Enable end-of-conversion interrupt
    ADC1->CR1 |= (1U << 5);      // EOC interrupt enable
    NVIC_EnableIRQ(ADC_IRQn);    // Enable ADC interrupt in NVIC

    // 6. Enable ADC1 and start conversion
    ADC1->CR2 |= (1U << 0);      // ADC enable
    ADC1->CR2 |= (1U << 30);     // Start conversion

    while (1)
    {
    }
}

void ADC_IRQHandler(void)
{
    if (ADC1->SR & (1U << 1))    // Check EOC flag
    {
        adcData = ADC1->DR;      // Read ADC result (clears EOC)

        if (adcData > threshold)
        {
            GPIOC->ODR |= (1U << 14);    // LED ON
        }
        else
        {
            GPIOC->ODR &= ~(1U << 14);   // LED OFF
        }
    }
}