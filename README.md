*This project was developed as an assignment for the ssy class at BUT FEEC University. (https://www.fekt.vut.cz/en/home)*

# SSY-HX711_LWM

Creation of school project, with HX711 sensor, ATmega256RFR2 in C library which allows user to measure weight and send it via uart and lwm network. Task was to use controller, connect HX711 and load cells (used 4 load cells). I took first inspiration for using HX711 from its datasheet and from [5].

### Assignment:

-Study datasheet of HX 711 https://cdn.sparkfun.com/datasheets/Sensors/ForceFlex/hx711_english.pdf

-Implement function for reading weights in C code by datasheet (example)

-Design connection of load cells and use 1 or 2 axis. 

## Solution:

## 1. Improving existing LWM code from lecture
First step in my project was to take and improve existing code from lecture, for LightWeight Mesh network. I let older files like i2c librabry, ad converter library and also at30tse758 in code for future improvements or just for testing. Main file for LWM is config.h, which consists lwm network code. In this file you **need to set up values of APP_ADDR and APP_PANID** APP_ADDR is choosing it the code is working as sender or receiver, 1 is receiver and app_panid is unique ID in lwm network, you need to set up same for sender and receiver. **Also make sure to set up value of My_interval value for the time you want to send data** in sender code. 

Part of code you can see briefly in picture below.

![](Images/1.png "Setup")



# References:
[1][Datasheet](https://ww1.microchip.com/downloads/en/DeviceDoc/Atmel-8393-MCU_Wireless-ATmega256RFR2-ATmega128RFR2-ATmega64RFR2_Datasheet.pdf)

[2][HX711sensor](https://cdn.sparkfun.com/datasheets/Sensors/ForceFlex/hx711_english.pdf)

[3][University](https://www.fekt.vut.cz/en/home)

[4][LWM guide](https://ww1.microchip.com/downloads/en/Appnotes/Atmel-42028-Lightweight-Mesh-Developer-Guide_Application-Note_AVR2130.pdf)

[5][Inspiration](https://github.com/bogde/HX711)
