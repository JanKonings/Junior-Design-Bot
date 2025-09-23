#include "motorControl.h"
/////// you can enter your sensitive data in the Secret tab/arduino_secrets.h
/////// WiFi Settings ///////
char ssid[] = "tufts_eecs";
char pass[] = "foundedin1883";

//websocket connection code
char serverAddress[] = "34.28.153.91";  // server address
int port = 80;
WiFiClient wifi;
WebSocketClient client = WebSocketClient(wifi, serverAddress, port);
String clientID = "89C87865077A"; //Insert your Client ID Here!
int status = WL_IDLE_STATUS;
int count = 0;


// code for the state machine
const unsigned char buttonInput = 2;
enum {state0, state1, state2, state3, state4, state5, state6};
volatile unsigned char currentState = state0;  // volatile: used inside ISR

//for LED interrupt
unsigned long lastInterruptTime = 0;  // for debouncing

//Setup for Motor Control
int PWMA = 9;
int PWMB = 10;
int AI1 = 4;
int AI2 = 5;
int BI1 = 6;
int BI2 = 7;
int STBY = 8;


void forward() {

}

void backward() {
  
}

void pivot_clockwise() {
  
}

void pivot_counter() {
  
}

void turn_right() {
  
}

void turn_left() {
  
}



// Interrupt Service Routine (ISR) for button press
void buttonISR() {
  unsigned long currentTime = millis();
  if (currentTime - lastInterruptTime > 200) { // debounce (200 ms)
    // Advance to the next state
    switch (currentState) {
      case state0: currentState = state1; break;
      case state1: currentState = state2; break;
      case state2: currentState = state3; break;
      case state3: currentState = state4; break;
      case state4: currentState = state5; break;
      case state5: currentState = state6; break;
      case state6: currentState = state0; break;
    }
    Serial.print("Interrupt: Button pressed. New state = ");
    Serial.println(currentState);
    lastInterruptTime = currentTime;
  }
}

void setup() {

  pinMode(LED_BUILTIN, OUTPUT);
  pinMode(buttonInput, INPUT_PULLUP);

  Serial.begin(9600);
  while (!Serial) {;}
  Serial.println("Setup complete. Starting in state0.");

  // Attach interrupt on falling edge (button press)
  attachInterrupt(digitalPinToInterrupt(buttonInput), buttonISR, FALLING);

  //Serial.begin(9600);
  while ( status != WL_CONNECTED) {
    Serial.print("Attempting to connect to Network named: ");
    Serial.println(ssid);                   // print the network name (SSID);

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



void changeState(unsigned char newState) {
  if (newState > state6 || newState < state0) {
    Serial.println("Invalid state. State unchanged.");
    return;
  }

  currentState = newState;
  Serial.print("State changed to ");
  Serial.println(currentState);
}

//LED blinking function
void handleBlink(int times) {
  for (int i = 0; i < times; i++) {
    digitalWrite(LED_BUILTIN, HIGH);
    delay(100);
    digitalWrite(LED_BUILTIN, LOW);
    delay(100);
  }
}


void loop() {

  // start Websocket Client
  Serial.println("starting WebSocket client");
  client.begin();
  client.beginMessage(TYPE_TEXT);
  client.print(clientID);
  client.endMessage();

  while (client.connected()) {
    int blinkTimes = currentState; // state0=1 blink, state6=7 blinks
    handleBlink(blinkTimes);
    delay(1000);

    int msgSize = client.parseMessage();
    if (msgSize > 0) {
      String msg = client.readString();
      int pos = msg.indexOf('.');
      int stateNum = 0;

      if(pos != -1) {
        String stateStr = msg.substring(pos + 1);
        if(stateStr.startsWith("RIDJ")) {
          stateStr = stateStr.substring(5); // Extract the number after "RIDJ "
          stateNum = stateStr.toInt();
          changeState(stateNum);
        }
      }
    }
  }

  Serial.println("disconnected");
}