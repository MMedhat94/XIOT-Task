# XIOT-Task



## Installation

- You can use another MCUs. All you need is datasheet. Don't forget to change the pins.
1. Create a project in Atmel Studio choosing 8-bit MCU ATmega16A.
2. Copy code from XIOT_Task.c into your main.c file, include user_input.h in your project.
3. Open user_input.h and choose your desired settings for the application.
4. Build project.

## Assumptions
- The clock of your Microcontroller should be set to 1 Mhz for the timers to operate properly.
-Care should be taken when setting the baud rate in user-input.h according to this table to avoid error frames.
