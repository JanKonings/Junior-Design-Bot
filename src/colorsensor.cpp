const int redLED = 2;
const int blueLED = 3;
const int sensorPin = A0;

int ambValue = 0;
int redValue = 0;
int blueValue = 0;

int ydiff = 0;
int xdiff = 0;

int mag = 0;
float rads = 0;
float deg = 0;

enum Color {
  RED,
  BLUE,
  YELLOW,
  BLACK,
  OTHER
};

void setup() {
  pinMode(redLED, OUTPUT);
  pinMode(blueLED, OUTPUT);
  Serial.begin(9600);
}

void loop() {
  Color detected = OTHER;

  // Red ambient light
  digitalWrite(redLED, LOW);
  digitalWrite(blueLED, LOW);
  delay(100); // allow light to stabilize
  ambValue = analogRead(sensorPin);

  // Read red reflectance
  digitalWrite(redLED, HIGH);
  digitalWrite(blueLED, LOW);
  delay(100);
  redValue = analogRead(sensorPin);

  // Read blue reflectance
  digitalWrite(redLED, LOW);
  digitalWrite(blueLED, HIGH);
  delay(100);
  blueValue = analogRead(sensorPin);

  // Turn off both LEDs
  digitalWrite(redLED, LOW);
  digitalWrite(blueLED, LOW);

  // Print results
  // Serial.print("Ambient: ");
  // Serial.print(ambValue);
  // Serial.print(" | Red: ");
  // Serial.print(redValue);
  // Serial.print(" | Blue: ");
  // Serial.println(blueValue);

  ydiff = abs(blueValue - ambValue);
  xdiff = abs(redValue - ambValue);

  mag = sqrt(sq(ydiff) + sq(xdiff));
  rads = atan2(ydiff, xdiff);
  deg = degrees(rads);

  // // Print cords
  // Serial.print("Decision Space: ");
  // Serial.print(xdiff);
  // Serial.print(", ");
  // Serial.println(ydiff);

  if(mag < 100) {
    detected = BLACK;
    Serial.println("BLACK ");
  } else if (deg > 0 && deg <= 35) {
    detected = RED;
    Serial.println("RED ");
  } else if (deg > 35 && deg <= 55) {
    detected = YELLOW;
    Serial.println("YELLOW ");
  } else if (deg > 55) {
    detected = BLUE;
    Serial.println("BLUE ");
  } else {
    detected = OTHER;
    Serial.println("OTHER ");
  }
  
  // Serial.print(mag);
  // Serial.print(", ");
  // Serial.println(deg);

} 