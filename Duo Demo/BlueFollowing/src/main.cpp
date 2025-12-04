#include "motorControl.h"
#include "obstacleDetecting.h"
#include "colorsensor.h"
#include <WiFiNINA.h>

// =========================
// WiFi / WebSocket Setup
// =========================
char ssid[] = "tufts_eecs";
char pass[] = "foundedin1883";

class WEBSOCKET

char serverAddress[] = "34.28.153.91";  // server address
int port = 80;
WiFiClient wifi;
WebSocketClient client = WebSocketClient(wifi, serverAddress, port);
String clientID = "89C87865077A"; // Insert your Client ID Here!
int status = WL_IDLE_STATUS;
String msg;

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
constexpr int THRESHOLD = 620;     // stop/avoid when sensor > 580


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

// void wifiConnect() {
//   while (status != WL_CONNECTED) {
//     status = WiFi.begin(ssid, pass);
//     delay(1000);
//   }

//   IPAddress ip = WiFi.localIP();
// }

// =========================
// Setup
// =========================
void setup() {
  colorSetup();

  Serial.begin(9600);

  motorSetup();

  obstacleDetectingSetup();

  // Connect to WiFi
  // wifiConnect();
}

// =========================
// Main Loop
// =========================
void loop() {
  delay(1000); // wait for 1 seconds before starting
  changeState(state1_crossing); // start by going to red

  while (true)
  {
    colorLoop(Left, Right); // Read color sensor values
    int sensorValue = analogRead(dividerIn);
    
    switch (currentState) {
      case state0_idle: {
        // Do nothing
        stop();

        int msgSize = client.parseMessage();
        if (msgSize > 0) {
          msg = client.readString();
        }

        if (msg == "red lane found") changeState(state1_crossing);

        // --- Handle incoming WebSocket messages ---
        // int msgSize = client.parseMessage();
        // if (msgSize > 0) {
        //   String msg = client.readString();
        //   int pos = msg.indexOf('.');
        //   if (pos != -1) {
        //     String stateStr = msg.substring(pos + 1);
        //     if (stateStr.startsWith("RIDJ")) {
        //       stateStr = stateStr.substring(5); 
        //       int stateNum = stateStr.toInt();
        //       changeState(stateNum);
        //     }
        //   }
        // }
        break;
      }

      case state1_crossing: {
        forward(75);
        if (sensorValue > THRESHOLD) {
          stop();
          backward(100);
          delay(500);
          pivot_counter();
          delay(3250);
          stop();
          changeState(state2_goRed);
        }
        break;
      }

      case state2_goRed: {
        forward(50);
        if (Left == BLUE && Right == BLUE) {
          stop();
          pivot_clockwise();
          delay(1750);
          stop();

          client.beginMessage(TYPE_TEXT);
          client.print("blue lane found");
          client.endMessage();

          changeState(state3_followRed);
        } 
        break;
      }

      case state3_followRed: {
        if (sensorValue > THRESHOLD) {
          stop();
          backward(200);
          delay(500);
          pivot_clockwise();
          delay(2000);
          stop();
          changeState(state4_goYellow);
          break;
        }

        if(Left == BLUE && Right == BLUE) {
          forward(75);
        } else if (Left == BLUE && Right == BLACK) {
          turn_left(17);
        } else if (Left == BLACK && Right == BLUE) {
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
          pivot_clockwise();
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
          pivot_clockwise();
          delay(1500);
          stop();
          changeState(state6_go_home);
          break;
        }

        if(Left == YELLOW && Right == YELLOW) {
          forward(50);
        } else if (Left == YELLOW && Right == BLACK) {
          turn_left(17);
        } else if (Left == BLACK && Right == YELLOW) {
          turn_right(17);
        } else {
          stop();
        }
        break;
      }

      case state6_go_home: {
        forward(75);
        if (sensorValue > 600) {
          stop();
          client.beginMessage(TYPE_TEXT);
          client.print("returned");
          client.endMessage();
          changeState(state0_idle);
        }
        break;
      }
    }
  }
}

