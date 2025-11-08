#include "motorControl.h"
#include "obstacleDetecting.h"
#include "colorsensor.h"
#include <WiFiNINA.h>

// =========================
// WiFi / WebSocket Setup
// =========================
char ssid[] = "tufts_eecs";
char pass[] = "foundedin1883";


// our own WEBSOCKET

char serverAddress[] = "10.5.12.247";  // server address
int port = 8080;
WiFiClient wifi;
WebSocketClient client = WebSocketClient(wifi, serverAddress, port);
String clientID = "89C87865077A"; // Insert your Client ID Here!
int status = WL_IDLE_STATUS;

// class WEBSOCKET

// char serverAddress[] = "34.28.153.91";  // server address
// int port = 80;
// WiFiClient wifi;
// WebSocketClient client = WebSocketClient(wifi, serverAddress, port);
// String clientID = "89C87865077A"; // Insert your Client ID Here!
// int status = WL_IDLE_STATUS;

// =========================
// State Machine
// =========================
enum { 
  state0_idle,              // idle / done
  state1_cross_to_top,      // cross to opposite side until top wall
  state2_return_find_red,   // drive back until red lane
  state3_follow_red_to_wall,// follow red until right wall
  state4_find_yellow,       // after left turn, search for yellow lane
  state5_follow_yellow_to_wall,// follow yellow until left wall
  state6_go_home            // turn left + go back to start
 };

unsigned char currentState = state0_idle;  

// =========================
// Obstacle Detection
// =========================
constexpr int THRESHOLD = 360;    

bool atWall() {
  int sensorValue = analogRead(dividerIn);
  return sensorValue > THRESHOLD;
}

// =========================
// Color Sensor
// =========================
Color detectedColor = OTHER;  // color detected by sensor
Color detectedColor2 = OTHER; // color detected by second sensor
int deg = 0;   // angle in degrees
int deg2 = 0;  // angle in degrees for second sensor
int mag = 0;   // magnitude
int mag2 = 0;  // magnitude for second sensor


bool seeRed() {
  return (detectedColor == RED || detectedColor2 == RED);
}

bool seeYellow() {
  return (detectedColor == YELLOW || detectedColor2 == YELLOW);
}

// left sensor = detectedColor, right sensor = detectedColor2
void followColor(Color target, int baseSpeed = 100, int turnSpeed = 90) {
  bool leftOn  = (detectedColor  == target);
  bool rightOn = (detectedColor2 == target);

  if (leftOn && rightOn) {
    forward(baseSpeed);
  } else if (leftOn && !rightOn) {
    turn_left(turnSpeed);
  } else if (!leftOn && rightOn) {
    turn_right(turnSpeed);
  } else {
    forward(baseSpeed / 2);
  }
}


// =========================
// Helpers
// =========================
void changeState(unsigned char newState) {
  if (newState > state6_go_home) return;

  currentState = newState;
}

void wifiConnect() {
  while (status != WL_CONNECTED) {
    status = WiFi.begin(ssid, pass);
    delay(1000);
  }

  IPAddress ip = WiFi.localIP();
}

// =========================
// Setup
// =========================
void setup() {
  colorSetup(); // Initialize color sensor

  Serial.begin(9600);

  motorSetup();

  // LED + obstacle detection hardware
  obstacleDetectingSetup();

  // Connect to WiFi
  wifiConnect();
}

// =========================
// Main Loop
// =========================
void loop() {
  // connect
  if (!client.connected()) {
    client.begin();
  }

  changeState(1); 

  while (1) {
    // Update sensors
    colorLoop(detectedColor, detectedColor2, deg, deg2, mag, mag2);
    int sensorValue = analogRead(dividerIn);

    switch (currentState) {

      // ======================
      // 0: Idle (end state)
      // ======================
      case state0_idle:
        stop();
        break;

      // ======================
      // 1: Cross to the other side
      // Stop at top wall, turn around
      // ======================
      case state1_cross_to_top:
        forward(100);

        if (sensorValue < THRESHOLD) {      // top wall
          stop();
          delay(300);

          // 180° turn (tune delay for your bot)
          pivot_clockwise();
          delay(3500);
          stop();
          delay(200);

          changeState(state2_return_find_red);
        }
        break;

      // ======================
      // 2: Cross back to find red lane
      // ======================
      case state2_return_find_red:
        forward(110);

        if (seeRed()) {
          stop();
          delay(200);
          changeState(state3_follow_red_to_wall);
        }
        break;

      // ======================
      // 3: Follow red lane until wall at the right
      // (We assume navigating forward; wall trigger ends this phase)
      // ======================
      case state3_follow_red_to_wall:
        followColor(RED, 105, 95);

        if (sensorValue > THRESHOLD) {    // hit right-side wall area
          stop();
          delay(300);

          // Turn left to face toward yellow-lane region
          pivot_counter();
          delay(350);   // ~90° left (tune)
          stop();
          delay(200);

          changeState(state4_find_yellow);
        }
        break;

      // ======================
      // 4: Find yellow lane
      // ======================
      case state4_find_yellow:
        forward(100);

        if (seeYellow()) {
          stop();
          delay(200);
          changeState(state5_follow_yellow_to_wall);
        }
        break;

      // ======================
      // 5: Follow yellow lane until wall at the left
      // ======================
      case state5_follow_yellow_to_wall:
        followColor(YELLOW, 105, 95);

        if (sensorValue > THRESHOLD) {  // left-side wall
          stop();
          delay(300);

          // Turn left again to face "home" direction
          pivot_counter();
          delay(350);  // ~90° (tune)
          stop();
          delay(200);

          changeState(state6_go_home);
        }
        break;

      // ======================
      // 6: Return to starting position
      // Here I’m assuming you drive straight until bottom wall.
      // ======================
      case state6_go_home:
        forward(110);

        if (sensorValue > THRESHOLD) {  // back at starting wall
          stop();
          delay(300);
          changeState(state0_idle);
        }
        break;
    }

    // small delay for stability
    delay(10);
  }
}

