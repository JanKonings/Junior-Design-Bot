#include "colorsensor.h"

void colorSetup() {
  pinMode(redLED, OUTPUT);
  pinMode(blueLED, OUTPUT);
  Serial.begin(9600);
}

void colorLoop() {
  Color detected = OTHER;
  Color detected2 = OTHER;

  // Red ambient light
  digitalWrite(redLED, LOW);
  digitalWrite(blueLED, LOW);
  delay(100); // allow light to stabilize
  ambValue = analogRead(sensorPinL);
  ambValue2 = analogRead(sensorPinR);

  // Read red reflectance
  digitalWrite(redLED, HIGH);
  digitalWrite(blueLED, LOW);
  delay(100);
  redValue = analogRead(sensorPinL);
  redValue2 = analogRead(sensorPinR);

  // Read blue reflectance
  digitalWrite(redLED, LOW);
  digitalWrite(blueLED, HIGH);
  delay(100);
  blueValue = analogRead(sensorPinL);
  blueValue2 = analogRead(sensorPinR);

  // Turn off both LEDs
  digitalWrite(redLED, LOW);
  digitalWrite(blueLED, LOW);

  // Print results
  // Serial.print("Ambient: ");
  // Serial.print(ambValue);
  // Serial.print(" | Red: ");
  // Serial.print(redValue);
  // Serial.print(" | Blue: ");
  // Serial.println(blueValue);

  ydiff = abs(blueValue - ambValue);
  xdiff = abs(redValue - ambValue);
  ydiff2 = abs(blueValue2 - ambValue2);
  xdiff2 = abs(redValue2 - ambValue2);

  mag = sqrt(sq(ydiff) + sq(xdiff));
  rads = atan2(ydiff, xdiff);
  deg = degrees(rads);
  mag2 = sqrt(sq(ydiff2) + sq(xdiff2));
  rads2 = atan2(ydiff2, xdiff2);
  deg2 = degrees(rads2);

  // // Print cords
  // Serial.print("Decision Space: ");
  // Serial.print(xdiff);
  // Serial.print(", ");
  // Serial.println(ydiff);

  if(mag < 80) {
    detected = BLACK;
    Serial.print("BLACK       ");
  } else if (deg > 0 && deg <= 20) {
    detected = RED; 
    Serial.print("RED     ");
  } else if (deg > 20 && deg <= 45) {
    detected = YELLOW;
    Serial.print("YELLOW        ");
  } else if (deg > 45) {
    detected = BLUE;
    Serial.print("BLUE        ");
  } else {
    detected = OTHER;
    Serial.print("OTHER       ");
  }

    if(mag2 < 80) {
    detected2 = BLACK;
    Serial.println("BLACK");
  } else if (deg2 > 0 && deg2 <= 15) {
    detected2 = RED; 
    Serial.println("RED");
  } else if (deg2 > 15 && deg2 <= 35) {
    detected2 = YELLOW;
    Serial.println("YELLOW");
  } else if (deg2 > 35) {
    detected2 = BLUE;
    Serial.println("BLUE");
  } else {
    detected2 = OTHER;
    Serial.println("OTHER");
  }
  
  Serial.print(mag);
  Serial.print(", ");
  Serial.print(deg);
  Serial.print("      ");

  Serial.print(mag2);
  Serial.print(", ");
  Serial.println(deg2);

} 