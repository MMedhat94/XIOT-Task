/*
 * Created: 25-June-18 6:07:15 PM
 *  Author: Mohamed Medhat
 */ 
#define F_CPU 1000000UL
#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include "user_input.h"

void Timer0_Init (void);
void Interrupt_Init(void);
void Port_Init(void);

int main(void)
{
	Port_Init();		 // Initialize Port
	Interrupt_Init();	 // Initialize Interrupts
	Timer0_Init();		 // Initialize Timer0
	sei();				 // Enable global interrupt bit
	
	while(1){
		//Main program here
	}	
}

void Timer0_Init (void)
{
	TCCR0 = (1<<CS02); //normal mode, 256 prescaler
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
	TCNT0= 21;				 // for 60 ms delay
	TIMSK |= (1 << TOIE0) ;	 // Enable Timer0 interrupt
	GICR &= ~(1<<INT0);		 //	Disable external interrupt to prevent bouncing
	GIFR |= 1<<INTF0;		 // Clear external interrupt flag
}

ISR(TIMER0_OVF_vect)
{
	TIMSK &= ~(1 << TOIE0) ;					//Clear timer0 overflow flag
	static char State=LED_OFF;					//current state of the LED
	if(PIN_BUTTON & (1<<PIN_NUMBER_BUTTON)){	//if the button is still pressed after bouncing time of the switch
		if(State==LED_OFF)
		{
			PORT_LED |= (1<<PIN_NUMBER_LED);
			State=LED_ON;
		}else if(State==LED_ON)
		{
			PORT_LED &= ~(1<<PIN_NUMBER_LED);
			State=LED_OFF;
		}
	}
	GICR |= 1<<INT0;
}