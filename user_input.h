/*
 * user_input.h
 *home branch test
 * Created: 28-Jun-18 10:38:36 PM
 *  Author: Mohamed Medhat
 */ 
// fixed one/* dummy comment */
#ifndef USER_INPUT_H_
#define USER_INPUT_H_

#define USART_BAUDRATE (4800)
#define MAX_TEMP (100)

#define DDR_LED			DDRA	// Choose from DDRA, DDRB, DDRC or DDRD
#define PORT_LED		PORTA	//Choose from PORTA, PORTB, PORTC or PORTD
#define PIN_NUMBER_LED	1		//Choose the desired pin, 0,1,2 ....7

#define DDR_BUTTON		DDRD	//For the program to operate properly using interrupts, leave as it is
#define PIN_BUTTON		PIND	//For the program to operate properly using interrupts, leave as it is
#define PIN_NUMBER_BUTTON 2		//For the program to operate properly using interrupts, leave as it is

#define ADC_CHANNEL_NUM	  0

#endif /* USER_INPUT_H_ */
