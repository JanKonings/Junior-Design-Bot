#include "motorControl.h"
#include "obstacleDetecting.h"
#include "colorsensor.h"



#include <WiFiNINA.h>

// =========================
// WiFi / WebSocket Setup
// =========================
char ssid[] = "tufts_eecs";
char pass[] = "foundedin1883";

// char serverAddress[] = "10.5.12.247";  // server  (ISAACS COMPUTER)
char serverAddress[] = "10.243.65.242";  // server (JANS COMPUTER)

int port = 8080;
WiFiClient wifi;
WebSocketClient client = WebSocketClient(wifi, serverAddress, port);
String clientID = "89C87865077A"; // Insert your Client ID Here!
int status = WL_IDLE_STATUS;

// char serverAddress[] = "34.28.153.91";  // server address
// int port = 80;
// WiFiClient wifi;
// WebSocketClient client = WebSocketClient(wifi, serverAddress, port);
// String clientID = "89C87865077A"; // Insert your Client ID Here!
// int status = WL_IDLE_STATUS;

// =========================
// State Machine
// =========================
enum { state0, state1, state2, state3, state4, state5, state6 };
unsigned char currentState = state0;  

// =========================
// Obstacle Detection
// =========================
constexpr int THRESHOLD = 660;     // stop/avoid when sensor > 400

// =========================
// Color Sensor
// =========================
Color Left = OTHER;  // color detected by sensor
Color Right = OTHER; // color detected by second sensor
int deg = 0;   // angle in degrees
int deg2 = 0;  // angle in degrees for second sensor
int mag = 0;   // magnitude
int mag2 = 0;  // magnitude for second sensor

int timer = 0; // timer for color reading intervals


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
  colorSetup(); // Initialize color sensor


  Serial.begin(9600);
  // Serial.begin(115200);

   
  // while (!Serial) {;}

  motorSetup();

  // LED + obstacle detection hardware
  obstacleDetectingSetup();

  // Serial.println("Setup complete. Starting in state0.");

  // Connect to WiFi
  wifiConnect();
}

// =========================
// Main Loop
// =========================
void loop() {
  // Start (or restart) WebSocket session
  // Serial.println("starting WebSocket client");
  client.begin();
  // client.beginMessage(TYPE_TEXT);
  // client.print(clientID);
  // client.endMessage();

  // changeState(1); 

  // int sensorValue = analogRead(dividerIn);
  //  Serial.print("IR Sensor Value: ");
  //   Serial.println(sensorValue);

// client.connected()
  while (1) {
    colorLoop(Left, Right, deg, deg2, mag, mag2); // Read color sensor values

    // --- Read IR sensor and print ---
    int sensorValue = analogRead(dividerIn);
    // Serial.print("IR Sensor Value: ");
    // Serial.println(sensorValue);


    if(sensorValue < THRESHOLD) {
      if(Left == 0 && Right == 0) {
        changeState(1);
      } else if (Left == 0 && Right == 3) {
        changeState(6);
      } else if (Left == 3 && Right == 0) {
        changeState(5);
      } else {
        changeState(0);
      }
    } else {
      changeState(0);
    }




    // if (timer == 10) { // send every 1 second
    //   timer = 0;
      // client.beginMessage(TYPE_TEXT);
      // client.print("Left: ");
      // client.println(Left);
      // client.print(mag);
      // client.print(", ");
      // client.print(deg);
      // client.print("      ");
      // client.print("\n");
      // client.print("Right: ");
      // client.println(Right);
      // client.print(mag2);
      // client.print(", ");
      // client.println(deg2);


      // client.beginMessage(TYPE_TEXT);
      // client.print(sensorValue);
      // client.endMessage();


      // for calibration:

      // client.beginMessage(TYPE_TEXT);
      // client.print(mag);
      // client.print(',');
      // client.print(deg);
      // client.print(',');
      // client.print(mag2);
      // client.print(',');
      // client.println(deg2);
      // client.endMessage();



      // Serial.print("Detected Color1: ");
      // Serial.println(detectedColor);
      // Serial.print("Detected Color2: ");
      // Serial.println(detectedColor2);
      // Serial.print(mag);
      // Serial.print(", ");
      // Serial.print(deg);
      // Serial.print("      ");

      // Serial.print(mag2);
      // Serial.print(", ");
      // Serial.println(deg2);
      // Serial.print(sensorValue);
    // }
    // timer++;
    
    // delay(1000); // send every 100ms
    // Serial.print("IR Sensor Value: ");
    // Serial.println(sensorValue);

    // --- Obstacle avoidance logic ---
    // if (currentState == state1 && sensorValue < THRESHOLD) {
    //   // Serial.println("Obstacle detected! Stopping.");
    //   stop(); //stop 
    //   delay(1000); // wait 1 second
    //   backward(100); // back up
    //   delay(2000); // back up for 2 seconds
    //   pivot_clockwise(); // pivot clockwise to avoid
    //   delay(1750); // pivot for 1.5 seconds
    //   forward(100); // resume forward
    // }

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

    // isObstacleDetected();

    // --- Drive motors based on state ---
    switch (currentState) {
      case state0:  stop();                 break; // idle
      case state1:  forward(75);           break;
      case state2:  backward(75);          break;
      case state3:  pivot_clockwise();      break;
      case state4:  pivot_counter();        break;
      case state5:  turn_right(17);        break;
      case state6:  turn_left(17);         break;
    }
  }

  Serial.println("disconnected");
}
