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
- Arduino UNO Wifi Rev2      1
- IR Phototransistor         1
- Phototransistor (Visible)  2
- IR LED                       

## Software Design

