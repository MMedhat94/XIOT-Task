## Goal:

Create an application  to control one input and output (IO Pins), where the input pin is a push button as external interrupt to the AVR,
once the push button is pressed it should toggle the led connected to output pin.

If the push button is pressed it must send “pressed” to the serial monitor via UART only once also send the LED state ”ON” or “OFF”.

The whole function could be used many times in the program without interfering each-other.

The program can send continuous serial date coming from temperature sensor with 3 second rate at the same time.

## Project phases:
1. Create a simple push button that toggles a led using any two pins and set them as input and output.
The LED should be connected in series with a resistor and the push button should be connected with a pull down resistor.

2. Put the push button to a pin that can be used as external interrupt and toggle the pin inside the ISR instead of using polling method in 
your main loop.

3. To eliminate the bouncing effect start a timer for 60 ms that starts counting from inside the ecternal interrupt ISR, After the timer overflows
check if the button is still pressed or not inside the timer ISR, if still pressed Toggle the LED after making sure that this button press was 
not because of the bouncing effect of the push button.

4. Initialize UART module and choose the right baud rate according to your AVR clock speed in order to avoid error frames.

5. Connect a UART TTL to USB module through your UART pins in the AVR in order to be able to send serial data to your PC and view the data on serial console.

6. Using the UART send "ON" and "OFF" each time the button is pressed to toggle the LED.

7. Connect a temperature sensor on any ADC channel of your AVR and initialize your ADC so that it is triggered every 3 seconds
depending on timer module that overflows every 3 seconds.

8. Inside the ADC ISR read the sensor's voltage and convert it to actual temperature assuming the temperature sensor is linear.

9. Using the UART send the temperature to the PC.

## Gained experience:
Using timer trigerred ADC.

## Previous experience:
Using AVR and C language for embedded systems projects for 3 years.

Examples for my projects:
- **Virtual Reality Car**: A car controlled using a PS2 controller connected to AVR using SPI and commands are sent to
the car. the car has a 360 degree camera mounted on it.A mobile phone is put into the Virtual Reality kit on the head of
the user and the video recorded by the camera is streamed on the phone's screen.The controller signals are sent to the
AVR, using SPI, then sent to the car using WiFi module ESP-8266 which has STM32F407 microcontroller to be able to
fully control the car.

- **Camera Pan/Tilt Stabilizer**: A system to stabilize camera to keep tracking a certain position without disturbance.
Two servos in the pan and tilt kit are controlled using STM32F407microcontroller (ARM based processor) which reads
gyroscope sensor readings from IMU6050and magnetometer sensor readings from GY-271 using I2C protocol.
