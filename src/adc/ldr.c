#include "stm32f4xx.h"

int adcData;
const int threshold = 1024;

int main(void) {
    RCC->AHB1ENR |= (1U << 0);
    RCC->AHB1ENR |= (1U << 2);
    RCC->APB2ENR |= (1 << 8);

    GPIOA->MODER |= (1U << 2);
    GPIOA->MODER |= (1U << 3);

    GPIOC->MODER &= ~(1U << 29);
    GPIOC->MODER |= (1U << 28);

    ADC1->SMPR2 = 0;
    ADC->CCR = 0;
    ADC1->CR1 = 0;
    ADC1->CR2 = 0;

    ADC1->CR2 |= (1U << 1);
    ADC1->SQR3 = 1;
    ADC1->SQR1 = 0;

    ADC1->CR2 |= 1;

    while ((ADC1->CR2 & 1) == 0) {}

    ADC1->CR2 |= (1 << 30);

    while (1) {
        while (!(ADC1->SR & (1 << 1))) {}

        adcData = ADC1->DR;

        if (adcData > threshold) {
            GPIOC->ODR |= (1U << 14);
        } else {
            GPIOC->ODR &= ~(1U << 14);
        }
    }
}
