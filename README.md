# Wireless Data Communication using STM32

## Project Overview
A hardware-based wireless communication system using two STM32 microcontrollers and 433 MHz RF modules. The system uses a 4×4 matrix keypad for data input and PIN-based authentication to provide controlled wireless access.

After successful authentication, the receiver can control an LED and buzzer remotely.

## Components Used
- 2 × STM32F103C8T6 (Blue Pill)
- 433 MHz RF Transmitter and Receiver
- 4×4 Matrix Keypad
- 16×2 I2C LCD
- LED and Buzzer
- Breadboard and Jumper Wires

## Technologies Used
- Embedded C/C++
- Arduino IDE
- STM32 Microcontroller
- RadioHead (RH_ASK) Library
- Keypad and LiquidCrystal_I2C Libraries

## Working Principle
1. User enters a 4-digit PIN using the keypad.
2. The transmitter sends the PIN wirelessly using RF modules.
3. The receiver verifies the PIN.
4. After successful authentication, keys A, B and C control the LED and buzzer.

## Applications
- Wireless Access Control
- Home Automation
- Security Systems
- Industrial Control

## Results
- Successful wireless data transmission.
- PIN-based authentication demonstrated.
- LED and buzzer controlled remotely.
- Indoor communication range tested up to approximately 10–15 metres in an open corridor.

## Author
**Rajat Ram**

Department of Electronics and Communication Engineering

Delhi Technological University (DTU)
