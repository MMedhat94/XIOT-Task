# XIOT-Task



## Installation

- You can use another MCUs. All you need is datasheet. Don't forget to change the pins.
1. Create a project in Atmel Studio choosing 8-bit MCU ATmega16A.
2. Copy code from XIOT_Task.c into your main.c file, include user_input.h in your project.
3. Open user_input.h and choose your desired settings as follow: 
- LED port and pin number.
- Push button port and pin number.
- ADC channel number.
- UART baudrate.
- Max temperature that the sensor can measure.
4. Build project.

## Hardware setup

- Atmega16A
- LED and 330 resistor connected in series to the output pin.
- push button connected with a pull down resistor to the input pin.
![rsz_pulldown](https://user-images.githubusercontent.com/19417067/42128477-db8c942c-7cac-11e8-9bf4-845a814b4837.jpg)

- Temperature sensor connected to the ADC pin.
- UART TTL to USB module is used in order to send the serial data from the AVR to the PC.
e.g. CP2102 Chip USB to TTL (Serial) Converter.

![webp net-resizeimage](https://user-images.githubusercontent.com/19417067/42128476-d8b6fc7e-7cac-11e8-896c-87ed7799bea5.jpg)




## Assumptions
- The clock of your Microcontroller should be set to **1 Mhz** for the timers to operate properly.
- The temperature sensor used is assumed to be **linear** and it's sensetivity is changed according to it's maximum temperature.

e.g. if the sensor has maximum temperature of 100 degree celsius, then it's senstivity is Max temp/Max voltage= 100/5= 20 degree/volt.
The user shall write the max temp in user_input.h and according to it the senstivity is calculated and used to find the correct temperature.

- The UART data frame is set to 8 data bits, 1 stops bit and no parity bit is used.
- The bounce time is set to **60 ms** as the worst bouncing time in all push buttons can't exceed 60 ms.
- When choosing the push button PIN in user_input.h make sure to choose a pin used as external interrupt in your microncontroller.

e.g. the default pin is user_input.h is Port D pin 2 which is Interrupt 0 for ATmega16A.

## Issues

- Care should be taken when setting the baud rate in user_input.h according to this table to avoid error frames.
![screen shot 2011-09-27 at 9 40 45 pm 1](https://user-images.githubusercontent.com/19417067/42127834-c5da75ac-7c9f-11e8-8bd1-dc94871b7474.png)

The default baud rate in user_input.h is set to **4800** for 0.2% error.
