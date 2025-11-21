#include <WiFiNINA.h>
#include <ArduinoHttpClient.h>


enum Color {
  RED,
  BLUE,
  YELLOW,
  BLACK,
  OTHER
};
// Black < 45, degree 45-50
// Blue > 65
// 

void colorSetup();
void colorLoop(Color &detected, Color &detected2);