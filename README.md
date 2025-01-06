Arduino RFID Access Control System

Project Overview

This project implements an RFID-based access control system using an Arduino board. The system validates RFID cards, controls access through a servo-monitor, provides visual feedback through LEDs, a buzzer and an OLED display, and logs attempts to a web server.

Features

RFID card reading and validation
Servo-controlled mechanism
Visual feedback through:
-OLED display showing access status
-RGB LED indicators (Blue for standby, Green for access granted, Red for access denied)
Audio feedback through a buzzer
Web server integration for access logging
Wi-Fi connectivity for remote monitoring

Hardware Requirements

Arduino board (compatible with WiFi)
MFRC522 RFID Reader
SSD1306 OLED Display (128x64)
Servo Motor
LEDs:

1x Blue LED (Pin 2)
1x Green LED (Pin 3)
1x Red LED (Pin 4)


Buzzer (Pin 5)
Connecting wires
Power supply

Required Libraries
#include <SPI.h>
#include <MFRC522.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <Servo.h>
#include <WiFi.h>

Pin Configuration

RST_PIN: 9 (RFID RST)
SS_PIN: 10 (RFID SS/SDA)
Blue LED: 2
Green LED: 3
Red LED: 4
Buzzer: 5
Servo: 6

Setup Instructions

Install all required libraries through the Arduino IDE Library Manager
Create a secrets.h file with your Wi-Fi credentials:
const char* ssid = "your_wifi_ssid";
const char* password = "your_wifi_password";

Connect the hardware components according to the pin configuration
Upload the code to your Arduino board
Configure your master RFID card by updating the MasterTag variable with your card's UID

Operation

The system starts in standby mode (Blue LED on)
When an RFID card is presented:

If it matches the master tag:

-Green LED lights up
-Buzzer beeps twice
-Servo unlocks for 3 seconds
-Access is logged as "Granted"


If it doesn't match:

-Red LED flashes
-Buzzer sounds
-Access is logged as "Denied"


All access attempts are logged to the web server

Web Server Integration
The system sends access data to a web server using HTTP POST requests. The server endpoint should be configured in the code:
const char* server = "www.ysjcs.net";
const int port = 80;

Troubleshooting

If the OLED display fails to initialize, check the I2C connections and address
If Wi-Fi connection fails, verify your credentials in secrets.h
If RFID reading is inconsistent, check the wiring and power supply

Security Notes

Keep your secrets.h file secure 
Regularly update the master RFID tag for security

Emily Long
220022392
York St John University