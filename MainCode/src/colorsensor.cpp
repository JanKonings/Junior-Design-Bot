#include "colorsensor.h"


const int redLED = 13;
const int blueLED = 12;
const int sensorPinL = A0;
const int sensorPinR = A5;

int ambValue = 0;
int redValue = 0;
int blueValue = 0;

int ydiff = 0;
int xdiff = 0;

// int mag = 0;
float rads = 0;
// float deg = 0;

int ambValue2 = 0;
int redValue2 = 0;
int blueValue2 = 0;

int ydiff2 = 0;
int xdiff2 = 0;

// int mag2 = 0;
float rads2 = 0;
// float deg2 = 0;

void colorSetup() {
  pinMode(redLED, OUTPUT);
  pinMode(blueLED, OUTPUT);
  Serial.begin(9600);
}

void colorLoop(Color &detected, Color &detected2, int &deg, int &deg2, int &mag, int &mag2) {
  detected = OTHER;
  detected2 = OTHER;

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

  if(mag <= 35) {
    detected = BLACK;
    Serial.print("BLACK       ");
  } else if (deg > 20 && deg <= 60) {
    detected = RED; 
    Serial.print("RED     ");
  } else if (deg > 0 && deg <= 20) {
    detected = YELLOW;
    Serial.print("YELLOW        ");
  } else if (deg > 60) {
    detected = BLUE;
    Serial.print("BLUE        ");
  } else {
    detected = OTHER;
    Serial.print("OTHER       ");
  }

  if(mag2 <= 35) {
    detected2 = BLACK;
    Serial.println("BLACK");
  } else if (deg2 > 20 && deg2 <= 60) {
    detected2 = RED; 
    Serial.println("RED");
  } else if (deg2 > 0 && deg2 <= 20) {
    detected2 = YELLOW;
    Serial.println("YELLOW");
  } else if (deg2 > 60) {
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