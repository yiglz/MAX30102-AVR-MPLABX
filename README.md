# MAX30102-ATMEGA328P-DRIVER
Bare-metal C driver for MAX30102 on ATmega328P. Developed in MPLAB X IDE with XC8 compiler. Features a custom, I2C driver and table-based documentation for configuration.

Warning: This driver is pre-configured for an 8 MHz clock frequency. If your hardware uses a different oscillator (e.g. 16 MHz on an Arduino Uno) you must update the following:
1-Frequency Definition: Change #define _XTAL_FREQ 8000000UL in max30102.h to match your crystal speed.
2-I2C Bit Rate: Check the i2c_driver.c file. The I2C communication speed depends on the CPU frequency. If you change the clock speed, you need to adjust the Bit Rate Generator register (TWBR) to maintain a stable bus speed.

This project is designed for the Microchip XC8 compiler. If you switch to AVR-GCC (Atmel Studio or Arduino), you must replace the __delay_ms() with the standard _delay_ms() function from the <util/delay.h> library and make sure your clock frequency is defined as F_CPU instead of _XTAL_FREQ.
