#include "colorsensor.h"

// =========================
// Setting pin mappings from PCB to Arduino
// =========================
const int redLED = 12;
const int blueLED = 11;
const int sensorPinL = A3;
const int sensorPinR = A0;

// =========================
// Color Sensor Variables Instantiation
// =========================

// Left sensor
int mag = 0;
int mag2 = 0;
int deg = 0;
int deg2 = 0;

int ambValue = 0;
int redValue = 0;
int blueValue = 0;

int ydiff = 0;
int xdiff = 0;

float rads = 0;

// Right sensor
int ambValue2 = 0;
int redValue2 = 0;
int blueValue2 = 0;

int ydiff2 = 0;
int xdiff2 = 0;

float rads2 = 0;

// For 2-read confirmation
static Color stableL = OTHER;
static Color stableR = OTHER;
static Color prevMeasuredL = OTHER;
static Color prevMeasuredR = OTHER;

// =========================
// Color Sensor Setup 
// =========================
void colorSetup() {
  pinMode(redLED, OUTPUT);
  pinMode(blueLED, OUTPUT);
}

// =========================
// Color Sensor Loop
// =========================
void colorLoop(Color &detected, Color &detected2) {
  const int NUM_SAMPLES = 25; // Number of samples to average

  float totalDeg  = 0, totalDeg2 = 0;
  float totalMag  = 0, totalMag2 = 0;

  for (int i = 0; i < NUM_SAMPLES; i++) {
    int ambL, redL, blueL;
    int ambR, redR, blueR;

    // Ambient values (LEDs off)
    digitalWrite(redLED, LOW);
    digitalWrite(blueLED, LOW);
    delay(5);
    ambL = analogRead(sensorPinL);
    ambR = analogRead(sensorPinR);

    // Red reflectance (red LED on)
    digitalWrite(redLED, HIGH);
    digitalWrite(blueLED, LOW);
    delay(5);
    redL = analogRead(sensorPinL);
    redR = analogRead(sensorPinR);

    // Blue reflectance (blue LED on)
    digitalWrite(redLED, LOW);
    digitalWrite(blueLED, HIGH);
    delay(5);
    blueL = analogRead(sensorPinL);
    blueR = analogRead(sensorPinR);

    // Turn off LEDs
    digitalWrite(redLED, LOW);
    digitalWrite(blueLED, LOW);

    // Differences
    int ydiffL = abs(blueL - ambL);
    int xdiffL = abs(redL - ambL);
    int ydiffR = abs(blueR - ambR);
    int xdiffR = abs(redR - ambR);

    // Magnitudes and angles
    float magL = sqrt(sq(ydiffL) + sq(xdiffL));
    float magR = sqrt(sq(ydiffR) + sq(xdiffR));
    float degL = degrees(atan2(ydiffL, xdiffL));
    float degR = degrees(atan2(ydiffR, xdiffR));

    // Accumulate totals
    totalMag  += magL;
    totalDeg  += degL;
    totalMag2 += magR;
    totalDeg2 += degR;
  }

  // Averages of totals
  mag  = totalMag  / NUM_SAMPLES;
  mag2 = totalMag2 / NUM_SAMPLES;
  deg  = totalDeg  / NUM_SAMPLES;
  deg2 = totalDeg2 / NUM_SAMPLES;

  // ----- RAW COLOR DECISIONS (measured) -----
  Color measuredL, measuredR;

  // ----- 1-READ DECISION -----
  // LEFT sensor
   if (mag <= 22) {
    measuredL = BLACK;
  } else if (deg >= 30 && deg <= 76) {
    measuredL = RED;
  } else if (deg > 76) {
    measuredL = YELLOW;
  } else if (deg > 0 && deg < 30) {
    measuredL = BLUE;
  } else {
    measuredL = OTHER;
  }

  // RIGHT sensor
  if (mag2 <= 10) {
    measuredR = BLACK;
  } else if (deg2 > 30 && deg2 <= 64) {
    measuredR = RED;
  } else if (deg2 > 64) {
    measuredR = YELLOW;
  } else if (deg2 > 0 && deg2 <= 30) {
    measuredR = BLUE;
  } else {
    measuredR = OTHER;
  }

  // Output the IMMEDIATE decision (1-read)
  detected  = measuredL;
  detected2 = measuredR;
}