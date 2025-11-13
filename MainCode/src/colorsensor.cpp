#include "colorsensor.h"


const int redLED = 12;
const int blueLED = 11;
const int sensorPinL = A3;
const int sensorPinR = A0;

int mag = 0;
int mag2 = 0;
int deg = 0;
int deg2 = 0;

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

static Color stableL = OTHER;
static Color stableR = OTHER;
static Color prevMeasuredL = OTHER;
static Color prevMeasuredR = OTHER;

void colorSetup() {
  pinMode(redLED, OUTPUT);
  pinMode(blueLED, OUTPUT);
  // Serial.begin(9600);
  // Serial.begin(115200);
}

void colorLoop(Color &detected, Color &detected2) {
  const int NUM_SAMPLES = 25;

  float totalDeg  = 0, totalDeg2 = 0;
  float totalMag  = 0, totalMag2 = 0;

  for (int i = 0; i < NUM_SAMPLES; i++) {
    int ambL, redL, blueL;
    int ambR, redR, blueR;

    // Ambient
    digitalWrite(redLED, LOW);
    digitalWrite(blueLED, LOW);
    delay(5);
    ambL = analogRead(sensorPinL);
    ambR = analogRead(sensorPinR);

    // Red reflectance
    digitalWrite(redLED, HIGH);
    digitalWrite(blueLED, LOW);
    delay(5);
    redL = analogRead(sensorPinL);
    redR = analogRead(sensorPinR);

    // Blue reflectance
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

    float magL = sqrt(sq(ydiffL) + sq(xdiffL));
    float magR = sqrt(sq(ydiffR) + sq(xdiffR));
    float degL = degrees(atan2(ydiffL, xdiffL));
    float degR = degrees(atan2(ydiffR, xdiffR));

    totalMag  += magL;
    totalDeg  += degL;
    totalMag2 += magR;
    totalDeg2 += degR;
  }

  // Averages
  mag  = totalMag  / NUM_SAMPLES;
  mag2 = totalMag2 / NUM_SAMPLES;
  deg  = totalDeg  / NUM_SAMPLES;
  deg2 = totalDeg2 / NUM_SAMPLES;

  // ----- RAW COLOR DECISIONS (measured) -----
  Color measuredL, measuredR;

  // LEFT sensor
   if (mag <= 18) {
    measuredL = BLACK;
  } else if (deg >= 30 && deg < 73) {
    measuredL = RED;
  } else if (deg >= 73) {
    measuredL = YELLOW;
  } else if (deg > 0 && deg < 30) {
    measuredL = BLUE;
  } else {
    measuredL = OTHER;
  }

  // RIGHT sensor
  if (mag2 <= 13) {
    measuredR = BLACK;
  } else if (deg2 > 30 && deg2 < 65) {
    measuredR = RED;
  } else if (deg2 >= 65) {
    measuredR = YELLOW;
  } else if (deg2 > 0 && deg2 <= 30) {
    measuredR = BLUE;
  } else {
    measuredR = OTHER;
  }

  // ----- 2-READ CONFIRMATION (branch predictor style) -----
  // Only change stable color if we see the same new color twice in a row

  // if (measuredL == prevMeasuredL && measuredL != stableL) {
  //   stableL = measuredL;
  // }
  // if (measuredR == prevMeasuredR && measuredR != stableR) {
  //   stableR = measuredR;
  // }

  // prevMeasuredL = measuredL;
  // prevMeasuredR = measuredR;

  // Output the STABLE decision
  // detected  = stableL;
  // detected2 = stableR;

  detected  = measuredL;
  detected2 = measuredR;
}