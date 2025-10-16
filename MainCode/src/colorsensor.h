#include <WiFiNINA.h>
#include <ArduinoHttpClient.h>

const int redLED = 2;
const int blueLED = 3;
const int sensorPinL = A2;
const int sensorPinR = A3;

int ambValue = 0;
int redValue = 0;
int blueValue = 0;

int ydiff = 0;
int xdiff = 0;

int mag = 0;
float rads = 0;
float deg = 0;

int ambValue2 = 0;
int redValue2 = 0;
int blueValue2 = 0;

int ydiff2 = 0;
int xdiff2 = 0;

int mag2 = 0;
float rads2 = 0;
float deg2 = 0;

enum Color {
  RED,
  BLUE,
  YELLOW,
  BLACK,
  OTHER
};

void colorSetup();
void colorLoop();