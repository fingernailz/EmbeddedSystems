/*
	Program for stm32f401 microcontroller
	Used for the blinking of on board led located at GPIO pin 13 of port C
	This is a special case as writing 0 to the led's output register sets the led on and vice versa
*/

#include "stm32f4xx.h"

static void init_port() {
	/* 
	 *	To enable the clock of port C we need to write 1 to bit 2 of AHB1ENR register, 
	 *	AHB1ENR's bit 0 corresponds to port A
	 *	AHB1ENR's bit 1 corresponds to port B
	 *	AHB1ENR'S bit 2 corresponds to port C, etc.
	*/
	RCC -> AHB1ENR |= (1U << 2);
	
	/*
	 * 	2 bits are used to represent the working state of a GPIO pin, using 2 bits we can represent 4 states.
	 * 	The 4 states are:
	 * 		1. 00 -> input mode
	 * 		2. 01 -> general purpose output mode
	 * 		3. 10 -> alternate function mode
	 * 		4. 11 -> analog mode
	*/
	GPIOC -> MODER |= (1U << 26);
}

int main(void) {

	// call fnc for config
	init_port();

	// .mainloop
	while (1) {
		/*		
		 *	To turn off the led, we write 1 to the OUTPUT DATA REGISTER (ODR) from GPIOC. (Special case)
		 *	We write exactly 1 bit to the pin_number bit, here 13 bit as pin number is 13.
		 *	
		 *	Taking unsigned interger 1 and shifting it 13 times left using left shift (<<) gives us 1000000000000,
		 *	by using bitwise OR (|) we are just changing the 13th bit.
		 *
		 *	XXXXXXXXXXXXX | 1000000000000 = 1XXXXXXXXXXXX
		 *
		 *	below is the shortform of 
		 *		`GPIOC -> ODR = GPIOC -> ODR | (1U << 13);`	
		*/
		GPIOC -> ODR |= (1U << 13);

		// just to pass some time
		for(int i = 0; i < 500000; i ++) {}

		/*	
		 *	To turn on the led we reset the value back to 0, same as setting the value to 1 but different bitwise operation
		 *
		 *	Take unsigned interger 1 and shift it left 13 times that gives 1000000000000,
		 *	now we negate this value using tilda (~) to get 0111111111111.
		 *
		 *	We use bitwise AND (&) with the existing register value.
		 *
		 *	1XXXXXXXXXXXX & 0111111111111 = 0XXXXXXXXXXXX
		*/
		GPIOC -> ODR &= ~(1U << 13);

		// for blinking purposes
		for(int j = 0; j < 500000; j ++) {}
	}

	return 1; // should work forever, idk if it exits. 
}
