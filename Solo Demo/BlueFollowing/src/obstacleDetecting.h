#include <WiFiNINA.h>
#include <ArduinoHttpClient.h>

// =========================
// Obstacle Detection Pins Configuratiosn
// =========================
constexpr int dividerIn = A2;
constexpr int IRled = 13;

// =========================
// Obstacle Detection Function Prototypes
// =========================
void obstacleDetectingSetup();