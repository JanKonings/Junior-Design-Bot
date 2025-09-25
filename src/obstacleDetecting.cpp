#include <WiFiNINA.h>
#include <ArduinoHttpClient.h>

constexpr char dividerIn = 'A1';
constexpr int IRled = 11;

void obstacleDetectingSetup() {
  pinMode(dividerIn, INPUT);
  pinMode(IRled, OUTPUT);
  digitalWrite(IRled, HIGH); // Turn on the IR LED
}

void isObstacleDetected() {
  int sensorValue = analogRead(dividerIn);
  Serial.print("IR Sensor Value: ");
  Serial.println(sensorValue);
}
