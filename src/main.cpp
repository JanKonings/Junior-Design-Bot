/*
  Simple WebSocket client for ArduinoHttpClient library
  created 28 Jun 2016
  by Sandeep Mistry
  modified 22 Jan 2019
  by Tom Igoe
  Modified by Gabriel Sessions and Emily Carlson
  for EE 31
  this example is in the public domain
*/
#include <WiFiNINA.h>
#include <ArduinoHttpClient.h>

/////// you can enter your sensitive data in the Secret tab/arduino_secrets.h
/////// WiFi Settings ///////
char ssid[] = "tufts_eecs";
char pass[] = "foundedin1883";

char serverAddress[] = "34.28.153.91";  // server address
int port = 80;

WiFiClient wifi;
WebSocketClient client = WebSocketClient(wifi, serverAddress, port);
String clientID = "89C87865077A"; //Insert your Client ID Here!
int status = WL_IDLE_STATUS;
int count = 0;

void setup() {
  Serial.begin(9600);
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

void loop() {

  // start Websocket Client
  Serial.println("starting WebSocket client");
  client.begin();
  client.beginMessage(TYPE_TEXT);
  client.print(clientID);
  client.endMessage();

  while (client.connected()) {
    client.beginMessage(TYPE_TEXT);
    client.print("Ferda");
    client.endMessage();
  }

  Serial.println("disconnected");
}