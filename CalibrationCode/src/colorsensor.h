#include <WiFiNINA.h>
#include <ArduinoHttpClient.h>

// =========================
// Color Sensor Definitions
// =========================
enum Color {
  RED,
  BLUE,
  YELLOW,
  BLACK,
  OTHER
};

// =========================
// Color Sensor Function Prototypes
// =========================
void colorSetup();
void colorLoop(Color &detected, Color &detected2, int &deg, int &deg2, int &mag, int &mag2);