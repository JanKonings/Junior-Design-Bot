#include "obstacleDetecting.h"

// =========================
// Obstacle Detection Functions
// =========================
void obstacleDetectingSetup() {
  pinMode(dividerIn, INPUT);
  pinMode(IRled, OUTPUT);
  digitalWrite(IRled, HIGH); // Turn on the IR LED
}

// Function to check if an obstacle is detected (for debugging-- unused in main loop)
void isObstacleDetected() {
  int sensorValue = analogRead(dividerIn);
  Serial.print("IR Sensor Value: ");
  Serial.println(sensorValue);
}
