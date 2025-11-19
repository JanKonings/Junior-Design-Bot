# Junior Design Autonomous Robot

## Overview
This project is an autonomous mobile robot developed for the Junior Design engineering course. The bot is designed to navigate a defined course, detect obstacles, and perform tasks using onboard sensors, real-time control algorithms, and an embedded microcontroller. Project has gone beyond specs to include custome made PCBs and websockets.
## Features
- **Autonomous navigation**
- **Real-time sensor processing**
- **Closed-loop motor control**
- **Custom-designed PCBs** for:
  - Collision Detection
  - Color Sensing
- **Robust electronics architecture** with modular subsystems

## Hardware Components
- **Microcontroller:** Arduino Uno
- **Drive system:** DC motors with H-bridge driver
- **Sensors:** ultrasonic sensors, IR sensors
- **Custom PCBs:** designed for sensor breakout, motor driver stages, and stable power supply
- **Chassis:** custom platform with battery mount
- **Power:** LiPo battery with onboard regulation

## Software Architecture
- Main real-time loop
- Navigation logic
- Sensor acquisition and filtering
- PID-based motor speed control
- State machine for task sequencing

## Control Strategy
- Obstacle detection using ultrasonic sensors
- Lane following using dou color sensors

## Usage
1. Power the robot
2. Place on start position on a track
3. Press start button
4. Robot autonomously navigates and performs required tasks
