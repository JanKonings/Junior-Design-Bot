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

// char serverAddress[] = "10.5.12.247";  // server  (ISAACS COMPUTER)
// // char serverAddress[] = "10.243.65.242";  // server (JANS COMPUTER)
// // char serverAddress[] = "10.5.8.205";  // server (DREWS COMPUTER)


// int port = 8080;
// WiFiClient wifi;
// WebSocketClient client = WebSocketClient(wifi, serverAddress, port);
// String clientID = "89C87865077A"; // Insert your Client ID Here!
// int status = WL_IDLE_STATUS;

// class WEBSOCKET

// char serverAddress[] = "34.28.153.91";  // server address
char serverAddress[] = "35.239.140.61";  // server address

int port = 8080;
WiFiClient wifi;
WebSocketClient client = WebSocketClient(wifi, serverAddress, port);
String clientID = "89C87865077A"; // Insert your Client ID Here!
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
//LIGHT
// constexpr int THRESHOLD = 620;     // stop/avoid when sensor > 580
//DARK
constexpr int THRESHOLD = 300;     // stop/avoid when sensor > 580



// =========================
// Color Sensor
// =========================
Color Left = OTHER;  // color detected by sensor
Color Right = OTHER; // color detected by second sensor
int sensorValue = 0;  // value from color sensor


// =========================
// Helpers
// =========================
void changeState(unsigned char newState) {
  if (newState > state6_go_home) return;

  currentState = newState;
}

void wifiConnect() {
  Serial.begin(9600);
  while ( status != WL_CONNECTED) {
    Serial.print("Attempting to connect to Network named: ");
    Serial.println(ssid); // print the network name (SSID);

    // Connect to WPA/WPA2 network:
    status = WiFi.begin(ssid, pass);
  }

  // print the SSID of the network you're attached to:
  Serial.print("SSID: ");
  Serial.println(WiFi.SSID());

  // print your WiFi shield's IP address:
  IPAddress ip = WiFi.localIP();
  Serial.print("IP Address: ");
  Serial.println(ip);
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
  delay(1000); // wait for 1 seconds before starting
  Serial.println("starting WebSocket client");
  client.begin();
  client.beginMessage(TYPE_TEXT);
  client.print(clientID);
  client.endMessage();
  // changeState(state1_crossing); // start by going to red
  Serial.print("got here");

  while (client.connected())
  {
    colorLoop(Left, Right); // Read color sensor values
    int sensorValue = analogRead(dividerIn); // Read obstacle detection sensor
    // client.beginMessage(TYPE_TEXT);
    // client.print(sensorValue);
    // client.endMessage();
    
    switch (currentState) {
      case state0_idle: {
        stop();

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
        break;
      }

      case state1_crossing: {
        forward(75);
        if (sensorValue > THRESHOLD) {
          stop();
          backward(100);
          delay(500);
          pivot_clockwise();
          delay(3250);
          stop();
          changeState(state2_goRed);
        }
        break;
      }

      case state2_goRed: {
        forward(50);
        if (Left == RED && Right == RED) {
          stop();
          pivot_counter();
          delay(1750);
          stop();
          changeState(state3_followRed);
        } 
        break;
      }

      case state3_followRed: {
        if (sensorValue > THRESHOLD) {
          stop();
          backward(200);
          delay(500);
          pivot_counter();
          delay(2000);
          stop();
          changeState(state4_goYellow);
          break;
        }

        if(Left == RED && Right == RED) {
          forward(75);
        } else if (Left == RED && Right == 3) {
          turn_left(17);
        } else if (Left == 3 && Right == RED) {
          turn_right(17);
        } else {
          stop();
        }
        break;
      }

      case state4_goYellow: {
        forward(50);
        if (Left == YELLOW || Right == YELLOW) {
          stop();
          pivot_counter();
          delay(1250);
          stop();
          changeState(state5_followYellow);
        }
        break;
      }

      case state5_followYellow: {
        if (sensorValue > THRESHOLD) {
          stop();
          backward(100);
          delay(500);
          pivot_counter();
          delay(1500);
          stop();
          changeState(state6_go_home);
          break;
        }

        if(Left == 2 && Right == 2) {
          forward(50);
        } else if (Left == 2 && Right == 3) {
          turn_left(17);
        } else if (Left == 3 && Right == 2) {
          turn_right(17);
        } else {
          stop();
        }
        break;
      }

      case state6_go_home: {
        forward(75);
        if (sensorValue > THRESHOLD) {
          stop();
          changeState(state0_idle);
        }
        break;
      }
    }
  }
}

