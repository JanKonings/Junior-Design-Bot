#include "motorControl.h"
#include "obstacleDetecting.h"

// =========================
// WiFi / WebSocket Setup
// =========================
char ssid[] = "tufts_eecs";
char pass[] = "foundedin1883";

char serverAddress[] = "34.28.153.91";  // server address
int port = 80;
WiFiClient wifi;
WebSocketClient client = WebSocketClient(wifi, serverAddress, port);
String clientID = "89C87865077A"; // Insert your Client ID Here!
int status = WL_IDLE_STATUS;

// =========================
// State Machine
// =========================
enum { state0, state1, state2, state3, state4, state5, state6 };
unsigned char currentState = state0;  

// =========================
// Obstacle Detection
// =========================
constexpr int THRESHOLD = 600;     // stop/avoid when sensor > 400


// =========================
// Helpers
// =========================
void changeState(unsigned char newState) {
  if (newState > state6) {
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
  Serial.begin(9600);
  while (!Serial) {;}

  motorSetup();

  // LED + obstacle detection hardware
  obstacleDetectingSetup();

  Serial.println("Setup complete. Starting in state0.");

  // Connect to WiFi
  wifiConnect();
}

// =========================
// Main Loop
// =========================
void loop() {
  // Start (or restart) WebSocket session
  Serial.println("starting WebSocket client");
  client.begin();
  client.beginMessage(TYPE_TEXT);
  client.print(clientID);
  client.endMessage();

  while (client.connected()) {
    // --- Read IR sensor and print ---
    int sensorValue = analogRead(dividerIn);
    Serial.print("IR Sensor Value: ");
    Serial.println(sensorValue);

    // --- Obstacle avoidance logic ---
    if (currentState == state1 && sensorValue > THRESHOLD) {
      Serial.println("Obstacle detected! Stopping.");
      stop(); //stop 
      delay(1000); // wait 1 second
      backward(100); // back up
      delay(2000); // back up for 2 seconds
      pivot_clockwise(); // pivot clockwise to avoid
      delay(1500); // pivot for 1.5 seconds
      forward(100); // resume forward
    }









    // --- Handle incoming WebSocket messages ---
    int msgSize = client.parseMessage();
    if (msgSize > 0) {
      String msg = client.readString();
      int pos = msg.indexOf('.');
      if (pos != -1) {
        String stateStr = msg.substring(pos + 1);
        if (stateStr.startsWith("RIDJ")) {
          stateStr = stateStr.substring(5); 
          int stateNum = stateStr.toInt();
          changeState(stateNum);
        }
      }
    }

    // isObstacleDetected();

    // --- Drive motors based on state ---
    switch (currentState) {
      case state0:  stop();                 break; // idle
      case state1:  forward(100);           break;
      case state2:  backward(100);          break;
      case state3:  pivot_clockwise();      break;
      case state4:  pivot_counter();        break;
      case state5:  turn_right(200);        break;
      case state6:  turn_left(200);         break;
    }
  }

  Serial.println("disconnected");
}
