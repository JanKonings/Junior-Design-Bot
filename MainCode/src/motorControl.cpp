#include "motorControl.h"


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
    digitalWrite(BI1, HIGH);
    digitalWrite(BI2, LOW);
    analogWrite(PWMB, speed);

}

void startLeftForward(int speed) {
    digitalWrite(AI1, HIGH);
    digitalWrite(AI2, LOW);
    analogWrite(PWMA, speed);
}

void startRightBackward(int speed) {
    digitalWrite(BI1, LOW);
    digitalWrite(BI2, HIGH);
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

// A is RIGHT motor
// B is LEFT motor
void pivot_clockwise() {
    stopLeft();
    startRightForward(100);
}

void pivot_counter() {
    stopRight();
    startLeftForward(100);
}

void turn_right(int speedLeft) {
    startLeftForward(speedLeft);
    startRightForward(100);
}

void turn_left(int speedRight) {
    startRightForward(speedRight);
    startLeftForward(100);
}


void forward(int speed) {
    startLeftForward(speed);
    startRightForward(speed);
}

void backward(int speed) {
    startRightBackward(speed);
    startLeftBackward(speed);
}
