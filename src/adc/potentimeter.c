#include "stm32f4xx.h"

int adcData;

const int threshold = 1024;

int main(void)
{
    RCC->AHB1ENR |= (1U << 0);

    RCC->AHB1ENR |= (1U << 2);

    RCC->APB2ENR |= (1 << 8);

    GPIOA->MODER |= (3U << 0);

    GPIOC->MODER &= ~(3U << 28);
    GPIOC->MODER |= (1U << 28);

    ADC1->SMPR2 = 0;

    ADC->CCR = 0;

    ADC1->CR1 = 0;
    ADC1->CR2 = 0;

    ADC1->CR2 |= (1U << 1);
    
    ADC1->SQR3=0;//ADC1->SQR3 = 1;//ADC1->SQR3 = (0U << 0) | (1U << 5);
    ADC1->SQR1=0;//for comb//ADC1->SQR1 = (1U << 20);


    ADC1->CR2 |= 1;

    while(!((ADC1->CR2 & 1)))
    {
    }

    ADC1->CR2 |= (1 << 30);

    while(1)
    {
        while(!(ADC1->SR & (1 << 1)))
        {
        }

        adcData = ADC1->DR;

        if(adcData > threshold)
        {
            GPIOC->ODR |= (1U << 14);
        }
        else
        {
            GPIOC->ODR &= ~(1U << 14);
        }
    }
}
