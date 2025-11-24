#include "motorControl.h"
#include "obstacleDetecting.h"
#include "colorsensor.h"
#include <WiFiNINA.h>

// =========================
// WiFi / WebSocket Setup
// =========================

// Replace with your network credentials
char ssid[] = "tufts_eecs";
char pass[] = "foundedin1883";

// Find your computers IP address (the computer running the calibration python script)
char serverAddress[] = "10.5.12.247";  // server  (ISAACS COMPUTER)
// char serverAddress[] = "10.243.65.242";  // server (JANS COMPUTER)

int port = 8080;
WiFiClient wifi;
WebSocketClient client = WebSocketClient(wifi, serverAddress, port);
int status = WL_IDLE_STATUS;

// =========================
// State Machine
// =========================
enum { 
  state0_idle,
  state1_crossing,
  state2_goRed,
  state3_followRed,
  state4_goYellow,
  state5_followYellow,
  state6_go_home
 };
unsigned char currentState = state0_idle;  

// =========================
// Obstacle Detection
// =========================
constexpr int THRESHOLD = 620;    // stop/avoid when sensor > 620

// =========================
// Color Sensor
// =========================
Color Left = OTHER;  // color detected by sensor
Color Right = OTHER; // color detected by second sensor
int deg = 0;   // angle in degrees
int deg2 = 0;  // angle in degrees for second sensor
int mag = 0;   // magnitude
int mag2 = 0;  // magnitude for second sensor

// =========================
// Helpers
// =========================
void changeState(unsigned char newState) {
  if (newState > state6_go_home) {
    Serial.println("Invalid state. State unchanged.");
    return;
  }
  currentState = newState;
  Serial.print("State changed to ");
  Serial.println(currentState);
}

void wifiConnect() {
  while (status != WL_CONNECTED) {
    Serial.print("Attempting to connect to: ");
    Serial.println(ssid);
    status = WiFi.begin(ssid, pass);
    delay(1000);
  }

  Serial.print("SSID: ");
  Serial.println(WiFi.SSID());
  IPAddress ip = WiFi.localIP();
  Serial.print("IP Address: ");
  Serial.println(ip);
}

// =========================
// Setup
// =========================
void setup() {
  colorSetup(); // Initialize color sensor

  Serial.begin(9600); // Begin serial communication

  motorSetup(); // Setup everything for motor control

  obstacleDetectingSetup(); // Obstacle detection setup

  wifiConnect(); // Connect to WiFi
}

// =========================
// Main Loop
// =========================
void loop() {
  client.begin(); // Connect to WebSocket server

  while (client.connected()) {
    colorLoop(Left, Right, deg, deg2, mag, mag2); // Read color sensor values

    int sensorValue = analogRead(dividerIn); // Read sesnsor value for obstacle detection
    // we read this value and dont use it because we want the conditions for calibration 
    // to be as similar as possible to a real demo, where the obstacle detection sensor is being read frequently
  
    // For calibration, send all the data to the websocket, which then collects and analyzes it to find 
    // min/max values for deg and mag for each sensor
    client.beginMessage(TYPE_TEXT);
    client.print(mag);
    client.print(',');
    client.print(deg);
    client.print(',');
    client.print(mag2);
    client.print(',');
    client.println(deg2);
    client.endMessage();
  }
}
