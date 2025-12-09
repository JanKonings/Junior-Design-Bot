#include "motorControl.h"
#include "obstacleDetecting.h"
#include "colorsensor.h"
#include <WiFiNINA.h>

// =========================
// WiFi / WebSocket Setup
// =========================
char ssid[] = "tufts_eecs";
char pass[] = "foundedin1883";

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
// constexpr int THRESHOLD = 555;     // stop/avoid when sensor > 555 LIGHT
constexpr int THRESHOLD = 290;     // stop/avoid when sensor > 290 DARK



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
  colorSetup();

  motorSetup();

  obstacleDetectingSetup();

  wifiConnect();
  client.beginMessage(TYPE_TEXT);
  client.print(clientID);
  client.endMessage();
}

// =========================
// Main Loop
// =========================
void loop() {
  delay(1000); 

  while (client.connected())
  {
    colorLoop(Left, Right); 
    int sensorValue = analogRead(dividerIn); 
    
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
        } else if (Left == RED && Right != RED) {
          turn_left(17);
        } else if (Left != RED && Right == RED) {
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

        if(Left == YELLOW && Right == YELLOW) {
          forward(50);
        } else if (Left == YELLOW && Right != YELLOW) {
          turn_left(17);
        } else if (Left != YELLOW && Right == YELLOW) {
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