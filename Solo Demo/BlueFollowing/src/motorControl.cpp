#include "motorControl.h"

// =========================
// Motor Control Functions
// =========================
void motorSetup() {
    pinMode(AI1, OUTPUT);
    pinMode(AI2, OUTPUT);
    pinMode(BI1, OUTPUT);
    pinMode(BI2, OUTPUT);

    pinMode(PWMA, OUTPUT);
    pinMode(PWMB, OUTPUT);
    pinMode(STBY, OUTPUT);
    digitalWrite(STBY, HIGH);
}

void stopRight() {
    digitalWrite(BI1, LOW);
    digitalWrite(BI2, LOW);
}

void stopLeft() {
    digitalWrite(AI1, LOW);
    digitalWrite(AI2, LOW);
}

void startRightForward(int speed) {
    digitalWrite(BI1, LOW);
    digitalWrite(BI2, HIGH);
    analogWrite(PWMB, speed);

}

void startLeftForward(int speed) {
    digitalWrite(AI1, HIGH);
    digitalWrite(AI2, LOW);
    analogWrite(PWMA, speed);
}

void startRightBackward(int speed) {
    digitalWrite(BI1, HIGH);
    digitalWrite(BI2, LOW);
    analogWrite(PWMB, speed);
}

void startLeftBackward(int speed) {
    digitalWrite(AI1, LOW);
    digitalWrite(AI2, HIGH);
    analogWrite(PWMA, speed);
}

void stop() {
    stopLeft();
    stopRight();
}

void pivot_clockwise() {
    stopLeft();
    startRightBackward(100);
}

void pivot_counter() {
    stopRight();
    startLeftBackward(100);
}

void turn_right(int speedLeft) {
    startLeftBackward(speedLeft);
    startRightBackward(64);
}

void turn_left(int speedRight) {
    startRightBackward(speedRight);
    startLeftBackward(64);
}

void backward(int speed) {
    startLeftForward(speed);
    startRightForward(speed);
}

void forward(int speed) {
    startRightBackward(speed);
    startLeftBackward(speed);
}
