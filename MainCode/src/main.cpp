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
char serverAddress[] = "10.243.65.242";  // server (JANS COMPUTER)
// char serverAddress[] = "10.5.8.205";  // server (DREWS COMPUTER)


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
constexpr int THRESHOLD = 660;     // stop/avoid when sensor > 580


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
  // if (!client.connected()) {
  //   client.begin();
  // }
  client.begin();
  client.beginMessage(TYPE_TEXT);
  client.print("Here");
  client.endMessage();

  delay(3000); // wait for 10 seconds before starting
  changeState(state2_goRed); // start by going to red

  // while (client.connected())
  while (true)
  {
    client.beginMessage(TYPE_TEXT);
    client.println(currentState);
    client.endMessage();
    // Serial.print("currentState = ");
    // Serial.println(currentState);
    // Serial.print("Wanted State = ");
    // Serial.println(state1_crossing);
    
    switch (currentState) {
      case state0_idle: {
        // Do nothing
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
        // Cross intersection

        sensorValue = analogRead(dividerIn);
        // client.beginMessage(TYPE_TEXT);
        // client.println(sensorValue);
        // client.endMessage();
        forward(75);
        if (sensorValue > THRESHOLD) {
          stop();
          backward(100);
          delay(500);
          pivot_clockwise();
          delay(2000);
          stop();
          changeState(state2_goRed);
        }

        client.beginMessage(TYPE_TEXT);
        client.println(sensorValue);
        client.endMessage();
        break;
      }

      case state2_goRed: {
        // Move towards red
        forward(50);
        colorLoop(Left, Right);
        if (Left == RED && Right == RED) {
          stop();
          pivot_clockwise();
          delay(1000);
          stop();
          changeState(state3_followRed);
        } 
        break;
      }

      case state3_followRed: {
        // Follow red line
        // sensorValue = analogRead(dividerIn);

        // if (sensorValue > THRESHOLD) {
        //   stop();
        //   backward(100);
        //   delay(500);
        //   pivot_counter();
        //   delay(500);
        //   stop();
        //   changeState(state4_goYellow);
        //   break;
        // }

        colorLoop(Left, Right);
        if(Left == 0 && Right == 0) {
          forward(75);
        } else if (Left == 0 && Right == 3) {
          turn_left(17);
        } else if (Left == 3 && Right == 0) {
          turn_right(17);
        } else {
          stop();
        }
        break;
      }

      case state4_goYellow: {
        // Move towards yellow
        if (Left == YELLOW || Right == YELLOW) {
          stop();
          pivot_counter();
          delay(500);
          stop();
          changeState(state5_followYellow);
        }
        break;
      }

      case state5_followYellow: {
        // Follow yellow line
        sensorValue = analogRead(dividerIn);

        if (sensorValue > THRESHOLD) {
          stop();
          backward(100);
          delay(500);
          pivot_counter();
          delay(500);
          stop();
          changeState(state6_go_home);
          break;
        }

        colorLoop(Left, Right);
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
        // Return to home base (assumed to be marked by both colors)
        sensorValue = analogRead(dividerIn);
        forward(100);
        if (sensorValue > THRESHOLD) {
          stop();
          changeState(state0_idle);
        }
        break;
      }
    }
  }
}

