# Junior Design Project: Autonomous Bot RC Bot Using Arduino UNO WiFi Rev2| EE31 | Fall 2025
### Made by: Jan Konings, Drew Galen, Isaac Medina, and Ryan Chen
### Tufts University
### School of Engineering | Department of Electrical and Computer Engineering
This project uses an Arduino UNO Wifi Rev2 to implement an autonomous bot that follows a specific "arena" style course given by the course instructions. The goal of the project was to design a robust bot that can perform the course as either "bot 1" or "bot 2". The embedded Arduino code was written in C++ using PlatformIO. 

The arena and "bot 1/2" tracks are outlined at the bottom of this README. 

A comprehensive documentation summary can be found here.

**Repository Structure**
- CalibrationCode:
  - This contains the embedded code for calibrating the color sensors
- Duo Demo:
  - BlueFollowing: "Bot 2" code for following the blue lane with the partner team
  - RedFollowing: "Bot 1" code for following the red lane with the partner team
- Solo Demo:
  - BlueFollowing: "Bot 2" code for following the blue lane solo
  - RedFollowing: "Bot 1" code for following the red lane solo
- calibration.py: Calibration websocket Python code to calibrate color sensors 
- websocket.py: A simple websocket to connect to for debugging

## Hardware
![Block Diagram](Block%20Diagram%20v3.png)
**Parts List & Quantity**
- Arduino UNO Wifi Rev2
- TB6612FNG H-Bridge
- LM2596 Buck Converter
- DC Motors x2
- IR Phototransistor  
- Phototransistor (Visible)  x2
- IR LED       
- Red LED   x2
- Blue LED x2
- Resistors (240 Ω) 0805 SMD x2
- Resistors (620 Ω) 0805 SMD x2
- Resistors (330 kΩ) 0805 SMD x2
- Resistor (1.5 MΩ) 0805 SMD 
- Standard resistor (1.0 MΩ) 
- Capacitor (10 μF) 0805 SMD
- Capacitor (1 μF) 0805 SMD 
- Electrolytic Capacitor (10 μF) x6
- 9V Battery x2

## Software Design

