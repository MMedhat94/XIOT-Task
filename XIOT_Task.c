/*
 * Created: 25-June-18 6:07:15 PM
 *  Author: Mohamed Medhat
 */ 
#define F_CPU 1000000UL
#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include "user_input.h"

uint8_t *ON_string	=(uint8_t *)"ON\n";
uint8_t *OFF_string =(uint8_t *)"OFF\n";

#define LED_OFF (0)
#define LED_ON  (1)
#define BAUD_PRESCALE (	(	(F_CPU / (USART_BAUDRATE * 16UL)	)	) - 1	)

void Timer0_Init (void);
void Interrupt_Init(void);
void Port_Init(void);
void USART_Init(void);
void USART_SendByte(uint8_t u8Data);
void USART_SendText(uint8_t* word);
void USART_send_uint16(uint16_t );

int main(void)
{
	Port_Init();		 // Initialize Port
	Interrupt_Init();	 // Initialize Interrupts
	USART_Init();		 // Initialize USART
	Timer0_Init();		 // Initialize Timer0
	sei();				 // Enable global interrupt bit
	
	while(1){
		//Main program here
	}	
}

void USART_Init(void){
    // Enable transmitter 
   UCSRB =  1<<TXEN;	
   UCSRC = (1<<URSEL) | (0<<UMSEL) | (0<<UPM0) | (0<<UPM1) | (0<<USBS) | (1<<UCSZ0) | (1<<UCSZ1);		//8 data bits, 1 STOP bit , no parity bit
    // Set baud rate
   UBRRL = (uint8_t)BAUD_PRESCALE;					// lower 8 bits to the low byte of UBRR
   UBRRH = (uint8_t)(BAUD_PRESCALE >> 8);			// upper 8 bits to the high byte of UBRR
}

void USART_SendByte(uint8_t u8Data){

	// Wait until last byte is transmitted
	while((UCSRA &(1<<UDRE)) == 0);

	// Transmit data
	UDR = u8Data;	// put the desired byte in the transmit buffer
}

void USART_SendText(uint8_t* word)
{
	uint16_t index=0;
	while(word[index])						// loop through the characters of the text and proceed if not equal to 0
	{
		USART_SendByte(word[index++]);		// send each char
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
			PORT_LED |= (1<<PIN_NUMBER_LED);	//Turn on the LED
			State=LED_ON;
			USART_SendText(ON_string);			//Send "ON" through the UART
		}else if(State==LED_ON)
		{
			PORT_LED &= ~(1<<PIN_NUMBER_LED);	//Turn off the LED
			State=LED_OFF;
			USART_SendText(OFF_string);			//Send "OFF" through the UART
		}	
	}
	GICR |= 1<<INT0;							//Enable External interrupt after the bouncing time is over
}