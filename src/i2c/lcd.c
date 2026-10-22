#include "stm32f4xx.h"
#include <stdio.h>

// I2C address of PCF8574 (8-bit write address: 0x27 << 1 = 0x4E)
#define PCF8574_ADDR 0x4E

// Global ADC data variable
volatile int adcData;

// --------------------------------------------------------------------
// Simple delay (software busy-wait)
// --------------------------------------------------------------------
void delay_ms(uint32_t ms) {
    for(uint32_t i = 0; i < ms * 8000; i++) {
        __NOP();
    }
}

// --------------------------------------------------------------------
// I2C1 Initialization (PB6 = SCL, PB7 = SDA, AF4, open-drain, 100 kHz)
// PCLK1 = 16 MHz (default HSI clock, no PLL configured)
// --------------------------------------------------------------------
void I2C1_Init(void) {
    RCC->AHB1ENR |= (1<<1);   // Enable GPIOB clock
    RCC->APB1ENR |= (1<<21);  // Enable I2C1 clock

    // Configure PB6, PB7 as AF4 (I2C1), open-drain, high speed, pull-up
    GPIOB->MODER &= ~((3<<12)|(3<<14));
    GPIOB->MODER |=  ((2<<12)|(2<<14));
    GPIOB->OTYPER |= (1<<6)|(1<<7);
    GPIOB->OSPEEDR |= (3<<12)|(3<<14);
    GPIOB->PUPDR   |= (1<<12)|(1<<14);
    GPIOB->AFR[0]  |= (4<<24) | (4<<28);

    // Reset I2C1
    I2C1->CR1 |= (1<<15);
    I2C1->CR1 &= ~(1<<15);

    // Configure I2C timing for PCLK1 = 16 MHz, Standard Mode (100 kHz)
    I2C1->CR2   = 16;   // APB1 clock = 16 MHz
    I2C1->CCR   = 80;   // 100 kHz SCL
    I2C1->TRISE = 17;   // (1000 ns / 62.5 ns) + 1 = 17

    I2C1->CR1 |= (1<<0); // Enable I2C1
}

// --------------------------------------------------------------------
// I2C Write: send one data byte to given 8-bit device address
// --------------------------------------------------------------------
void I2C1_Write(uint8_t addr, uint8_t data) {
    while(I2C1->SR2 & (1<<1));     // Wait until bus not busy
    I2C1->CR1 |= (1<<8);           // Generate START
    while(!(I2C1->SR1 & (1<<0)));  // Wait for SB

    I2C1->DR = addr;               // Send address
    while(!(I2C1->SR1 & (1<<1)));  // Wait for ADDR
    (void)I2C1->SR2;               // Clear ADDR flag

    while(!(I2C1->SR1 & (1<<7)));  // Wait TXE
    I2C1->DR = data;               // Send data

    while(!(I2C1->SR1 & (1<<2)));  // Wait BTF
    I2C1->CR1 |= (1<<9);           // Generate STOP
}

// --------------------------------------------------------------------
// LCD driver functions (HD44780 over PCF8574, 4-bit mode)
// --------------------------------------------------------------------
void LCD_SendCmd(uint8_t cmd) {
    uint8_t high = (cmd & 0xF0);
    uint8_t low  = ((cmd << 4) & 0xF0);

    I2C1_Write(PCF8574_ADDR, high | 0x0C);
    delay_ms(1);
    I2C1_Write(PCF8574_ADDR, high | 0x08);

    I2C1_Write(PCF8574_ADDR, low | 0x0C);
    delay_ms(1);
    I2C1_Write(PCF8574_ADDR, low | 0x08);
}

void LCD_SendData(uint8_t data) {
    uint8_t high = (data & 0xF0);
    uint8_t low  = ((data << 4) & 0xF0);

    I2C1_Write(PCF8574_ADDR, high | 0x0D);
    delay_ms(1);
    I2C1_Write(PCF8574_ADDR, high | 0x09);

    I2C1_Write(PCF8574_ADDR, low | 0x0D);
    delay_ms(1);
    I2C1_Write(PCF8574_ADDR, low | 0x09);
}

void LCD_Init(void) {
    delay_ms(50);       // LCD power-up delay
    LCD_SendCmd(0x33);  // init
    LCD_SendCmd(0x32);  // 4-bit mode
    LCD_SendCmd(0x28);  // 2-line, 5x7 font
    LCD_SendCmd(0x0C);  // display on, cursor off
    LCD_SendCmd(0x06);  // entry mode
    LCD_SendCmd(0x01);  // clear display
    delay_ms(5);
}

void LCD_SetCursor(uint8_t row, uint8_t col) {
    uint8_t addr = (row == 0 ? 0x80 : 0xC0) + col;
    LCD_SendCmd(addr);
}

void LCD_Print(char *str) {
    while(*str) {
        LCD_SendData(*str++);
    }
}

// --------------------------------------------------------------------
// ADC1 Initialization (PA0, continuous conversion mode)
// --------------------------------------------------------------------
void ADC1_Init(void) {
    RCC->AHB1ENR |= (1U << 0);   // Enable GPIOA
    RCC->APB2ENR |= (1 << 8);    // Enable ADC1

    GPIOA->MODER |= (3U << 0);   // PA0 analog mode

    ADC1->SMPR2 = 0;             // Sampling time = 3 cycles
    ADC->CCR = 0;                // Prescaler = PCLK2/2
    ADC1->CR1 = 0;               // Reset CR1
    ADC1->CR2 = 0;               // Reset CR2

    ADC1->CR2 |= (1U << 1);      // Continuous mode
    ADC1->SQR3 = 0;              // Channel 0
    ADC1->SQR1 = 0;              // Sequence length = 1

    ADC1->CR2 |= 1;              // Enable ADC1
    while ((ADC1->CR2 & 1) == 0); // Wait ready

    ADC1->CR2 |= (1 << 30);      // Start conversion
}

// --------------------------------------------------------------------
// LED Initialization (PC13 as output)
// --------------------------------------------------------------------
void LED_Init(void) {
    RCC->AHB1ENR |= (1<<2);           // Enable GPIOC
    GPIOC->MODER &= ~(3 << (13 * 2));
    GPIOC->MODER |=  (1 << (13 * 2)); // PC13 output
}

// --------------------------------------------------------------------
// MAIN PROGRAM
// --------------------------------------------------------------------
int main(void) {
    char buf[16];

    I2C1_Init();
    LCD_Init();
    ADC1_Init();
    LED_Init();

    while (1) {
        // Wait for ADC conversion complete
        while (!(ADC1->SR & (1 << 1)));
        adcData = ADC1->DR;

        // LED ON if ADC > 3000, else OFF
        if (adcData > 3000)
            GPIOC->ODR |= (1 << 13);
        else
            GPIOC->ODR &= ~(1 << 13);

        // Display ADC value on LCD
        LCD_SendCmd(0x01);  // Clear
        LCD_SetCursor(0, 0);
        LCD_Print("POT Value:");
        LCD_SetCursor(1, 0);
        sprintf(buf, "%u", adcData);
        LCD_Print(buf);

        delay_ms(500); // Update every 0.5s
    }
}
