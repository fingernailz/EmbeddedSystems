/*The program receives characters from the serial console and sends them back, which can be seen on the serial terminal.
Any serial console, like PuTTY, Arduino serial monitor, may be used with this program.
*/


/***************************************************
 USART2 TX on STM32F401 is at pin PA2, RX is at pin
 PA3.
 :AKS:Sept 17, 2025
========= STM32Black Pill === FTDI 232 ==========
           GND------------GND pin
   pin A2  TX-------------RX  pin
   pin A3  RX-------------TX  pin
	 
 Select 3.3V jumper position. 
 Use any serial monitor program to view the output
***************************************************/

#include "stm32f4xx.h"
#include "stdint.h"
#include <stdio.h>
#include "stdarg.h"
#include "string.h"

int len = 0;
char buff[256];
void usart_2_enable(void);
void printMsg(char *msg);
int readMsg(void);

void usart_2_enable(void)
{
  //Enabling clock on pins PA3, PA3 for alternating funct. for uart/usart
  RCC->AHB1ENR |= (1U<<0); //clock to GPIO A enabled.
	RCC->APB1ENR |= (1U<<17); //USART2 clock enabled
	GPIOA->MODER |= (2U<<4)|(2U<<6); //Select alternate function for PA2 and PA3
	
	GPIOA->AFR[0] &= ~(1U<<11);
	GPIOA->AFR[0] |= (1U<<10)|(1U<<9)|(1U<<8); // Chose USART TX function for PA2 
	
	GPIOA->AFR[0] &= ~(1<<15);
	GPIOA->AFR[0] |= (1<<14)|(1<<13)|(1<<12); // Chose USART RX function for PA3 
	USART2->BRR = (0x0683); //   set up the divisor for 9600 Baud, with deafult fraame settings
	
	USART2->CR1 |= (1<<2)|(1<<3)|(1<<13); // Enable USART2 for RX and TX
	
}

void printMsg(char buff[])
{
	for(uint32_t i=0;i<strlen(buff);i++)
	{
		USART2->DR = buff[i];
		while(!(USART2->SR & USART_SR_TXE));
	   //wait for TXE, 1 = data transferred
  }
}

int readMsg(void)
{ 
  
	for( len =0;len<256;len++)
	{
    while(!(USART2->SR & USART_SR_RXNE));
	  buff[len]= USART2->DR;
    if (buff[len]==10 || buff[len]==13) break; //if enter is pressed, providing that it is 10 or 0xA (line feed), or 13 (0xD) (carriage return) 
	}
	return len; //Returning number of entered characters, not including line feed nor carriage return (!)
}

int main(void)
{
	usart_2_enable();
	printMsg("UART serial Communication \n");		
	while(1)
	{
	
    readMsg(); // Receive characters from serial coomunicatio port of the PC			
    // Send the received characters back to the Serial consolde - the same characters as received		
		for (int i=0;i<len+1;i++)
	  {
	    USART2->DR = buff[i];
	    while(!(USART2->SR & USART_SR_TXE)); //TXE becomes true (1) when the content of DR (data register) goes into shift register
	  }
	}
}
