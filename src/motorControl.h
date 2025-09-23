#include <WiFiNINA.h>
#include <ArduinoHttpClient.h>

// pins for motor control
int PWMA = 9;
int PWMB = 10;
int AI1 = 4;
int AI2 = 5;
int BI1 = 6;
int BI2 = 7;
int STBY = 8;


void forward(int speed);
void backward(int speed);

void pivot_clockwise();
void pivot_counter();

void turn_right();
void turn_left();

void stop();

void stopRight();
void stopLeft();

void startLeftForward(int speed);
void startRightForward(int speed);

void startLeftBackward(int speed);
void startRightBackward(int speed);
