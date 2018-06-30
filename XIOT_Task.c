/*
 * Created: 25-June-18 6:07:15 PM
 *  Author: Mohamed Medhat
 */ 
#define F_CPU 1000000UL
#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include "user_input.h"

void Interrupt_Init(void);
void Port_Init(void);

int main(void)
{
	Port_Init();		 // Initialize Port
	Interrupt_Init();	 // Initialize Interrupts
	sei();				 // Enable global interrupt bit
	
	while(1){
		//Main program here
	}	
}

void Interrupt_Init(void)
{
	MCUCR = 1<<ISC01 | 1<<ISC00; //rising edge interrupt
	GICR |= 1<<INT0;			// enable INT0
}

void Port_Init(void)
{
	DDR_BUTTON  &= ~(1<<PIN_NUMBER_BUTTON);	//Set user button pin as input
	DDR_LED		|=  (1<<PIN_NUMBER_LED);	//Set user led pin as output
}

ISR(INT0_vect)
{
	PORT_LED ^=(1<<PIN_NUMBER_LED);		//Toggle the led for every push button interrupt
}