/*
 * Created: 25-June-18 6:07:15 PM
 *  Author: Mohamed Medhat
 */ 
#define F_CPU 1000000UL
#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include "user_input.h"

uint8_t *ON_string	=(uint8_t *)"PRESSED: ON\n";
uint8_t *OFF_string =(uint8_t *)"PRESSED: OFF\n";
uint8_t *Sensor_text=(uint8_t *)"Current temperature is: ";

#define LED_OFF (0)
#define LED_ON  (1)
#define BAUD_PRESCALE (	(	(F_CPU / (USART_BAUDRATE * 16UL)	)	) - 1	)
#define ADC_VREF (5)
#define ADC_Step (ADC_VREF	/	1024.0)
#define SENSOR_SENSTIVITY (MAX_TEMP	/	ADC_VREF)

void Timer0_Init (void);
void Timer1_Init(void);
void Interrupt_Init(void);
void Port_Init(void);
void ADC_Init(void);
void USART_Init(void);
void USART_SendByte(uint8_t u8Data);
void USART_SendText(uint8_t* word);
void USART_send_uint16(uint16_t );

int main(void)
{
	Port_Init();		 // Initialize Port
	USART_Init();		 // Initialize USART
	Timer1_Init();		 // Initialize Timer1
	Timer0_Init();		 // Initialize Timer0
	Interrupt_Init();	 // Initialize Interrupts
	ADC_Init();			 // Initialize ADC
	sei();
	while(1){
		//Main program here
	}	
}

void Port_Init(void)
{
	DDR_BUTTON  &= ~(1<<PIN_NUMBER_BUTTON);	//Set user button pin as input
	DDR_LED		|=  (1<<PIN_NUMBER_LED);	//Set user led pin as output
}

void Interrupt_Init(void)
{
	MCUCR = 1<<ISC01 | 1<<ISC00; //rising edge interrupt
	GICR |= 1<<INT0;			// enable INT0
}

void ADC_Init(void)
{
	ADMUX = (1<<REFS0) | (ADC_CHANNEL_NUM<<MUX0); //VCC is Vref, Right Adjust results, choose ADC0 with 1 gain
	SFIOR = (1<<ADTS0) | (1<<ADTS2); // Auto trigger with Timer1 Compare Match B
	ADCSRA= (1<<ADEN) | (1<<ADSC) | (1<<ADATE) | (1<<ADIE) | (1<<ADPS2); //Enable ADC, Start conversion, Auto trigger Enable, 16 prescaler
}

void Timer0_Init (void)
{
	TCCR0 = (1<<CS02); //normal mode, 256 prescaler
}

void Timer1_Init(void)
{
	OCR1A=46877;											// 3 seconds, cant use OCR1B as mentioned in AVR datasheet
	TCCR1A=0x00;											// Compare match mode with OCR1A as compare value
	TCCR1B |= (1 << WGM12) | (1 << CS11) | (1 << CS10);		// 64 prescaler, Compare match mode with OCR1A as compare value
	TCNT1 = 0;												//
}

void USART_Init(void){
    // Enable transmitter 
   UCSRB =  1<<TXEN;								// Enable transmitter
   UCSRC = (1<<URSEL) | (0<<UMSEL) | (0<<UPM0) | (0<<UPM1) | (0<<USBS) | (1<<UCSZ0) | (1<<UCSZ1);		//8 data bits, 1 STOP bit , no parity bit
    // Set baud rate
   UBRRL = (uint8_t)BAUD_PRESCALE;					// lower 8 bits to the low byte of UBRR
   UBRRH = (uint8_t)(BAUD_PRESCALE >> 8);			// upper 8 bits to the high byte of UBRR
}

uint8_t uint8_to_ascii(uint8_t num)
{
	return num+48;									// add 48 to convert each number to it's corresponding ascii value
}

void USART_send_uint16(uint16_t number)
{
	uint8_t hundred,ten,unit;
	unit	=	(uint8_t)(number % 10);							// Find the units in the number
	ten		=	(uint8_t)(	(number % 100)/10 );				// Find the tenth in the number
	hundred =	(uint8_t)(	(uint16_t)(number % 1000)/100);		// Find the hundreds in the number
	
	USART_SendText(Sensor_text);								// Send Text "Current temperature is: "
	USART_SendByte(uint8_to_ascii(hundred));					// Send the hundreds digit
	USART_SendByte(uint8_to_ascii(ten));						// Send the tens digit
	USART_SendByte(uint8_to_ascii(unit));						// Send the units digit
	USART_SendByte('\n');										// Send newline
}

void USART_SendByte(uint8_t u8Data){
	
	while((UCSRA &(1<<UDRE)) == 0);		// Wait until last byte is transmitted
	// Transmit data
	UDR = u8Data;						// put the desired byte in the transmit buffer
}

void USART_SendText(uint8_t* word)
{
	uint16_t index=0;
	while(word[index])						// loop through the characters of the text and proceed if not equal to 0
	{
		USART_SendByte(word[index++]);		// send each char
	}
}

 ISR(ADC_vect)
{
	uint16_t sensor_temp=0;
	float sensor_volt=0;
	sensor_volt = ADC *ADC_Step;								// multiply ADC value by ADC step to get the Voltage value of the sensor
	sensor_temp = (uint16_t)(sensor_volt*SENSOR_SENSTIVITY);	// multiply by the sensor sensetivity to get the actual temperature of the room
	USART_send_uint16(sensor_temp);								// send the temperature through the UART
	TIFR=(1<<OCF1B);
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