#include "obstacleDetecting.h"

// =========================
// Obstacle Detection Functions
// =========================
void obstacleDetectingSetup() {
  pinMode(dividerIn, INPUT);
  pinMode(IRled, OUTPUT);
  digitalWrite(IRled, HIGH); // Turn on the IR LED
}