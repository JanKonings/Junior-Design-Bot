#include <WiFiNINA.h>
#include <ArduinoHttpClient.h>


enum Color {
  RED,
  BLUE,
  YELLOW,
  BLACK,
  OTHER
};

void colorSetup();
void colorLoop(Color &detected, Color &detected2, int &deg, int &deg2, int &mag, int &mag2);