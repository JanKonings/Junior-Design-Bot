#include "motorControl.h"


void motorSetup() {
    pinMode(AI1, OUTPUT);
    pinMode(AI2, OUTPUT);
    pinMode(BI1, OUTPUT);
    pinMode(BI2, OUTPUT);

    pinMode(PWMA, OUTPUT);
    pinMode(PWMB, OUTPUT);
    pinMode(STBY, OUTPUT);
}

void forward(int speed) {
    digitalWrite(AI1, HIGH);
    digitalWrite(AI2, LOW);
    analogWrite(PWMA, speed);


    digitalWrite(BI1, HIGH);
    digitalWrite(BI2, LOW);
    analogWrite(PWMB, speed);

}

void backward(int speed) {
    digitalWrite(AI1, LOW);
    digitalWrite(AI2, HIGH);
    analogWrite(PWMA, speed);

    digitalWrite(BI1, LOW);
    digitalWrite(BI2, HIGH);
    analogWrite(PWMB, speed);

}

void pivot_clockwise() {
  
}

void pivot_counter() {
  
}

void turn_right() {
  
}

void turn_left() {
  
}
