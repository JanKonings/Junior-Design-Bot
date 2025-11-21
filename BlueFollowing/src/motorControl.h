#include <WiFiNINA.h>
#include <ArduinoHttpClient.h>

// pins for motor control

//OLD CONFIG

// constexpr int PWMA = 9;
// constexpr int PWMB = 10;
// constexpr int AI1 = 4;
// constexpr int AI2 = 5;
// constexpr int BI1 = 6;
// constexpr int BI2 = 7;
// constexpr int STBY = 8;

constexpr int PWMA = 9;
constexpr int PWMB = 6;
constexpr int AI1 = 4;
constexpr int AI2 = 3;
constexpr int BI1 = 8;
constexpr int BI2 = 7;
constexpr int STBY = 5;

void motorSetup();

void forward(int speed);
void backward(int speed);

void pivot_clockwise();
void pivot_counter();

void turn_right(int speedLeft);
void turn_left(int speedRight);

void turn_right_backward(int speedLeft);
void turn_left_backward(int speedRight);

void stop();

void stopRight();
void stopLeft();

void startLeftForward(int speed);
void startRightForward(int speed);

void startLeftBackward(int speed);
void startRightBackward(int speed);
