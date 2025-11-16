#include "MotorController.h"

MotorController::MotorController(int pwmA, int pwmB, int in1, int in2) {
  _pwmA = pwmA;
  _pwmB = pwmB;
  _in1 = in1;
  _in2 = in2;
  
  pinMode(_pwmA, OUTPUT);
  pinMode(_pwmB, OUTPUT);
  pinMode(_in1, OUTPUT);
  pinMode(_in2, OUTPUT);
}

void MotorController::setSpeed(int speed) {
  _speed = constrain(speed, -100, 100);
  int pwmValue = mapSpeed(_speed);

  if (_speed > 0) {
    digitalWrite(_in1, HIGH);
    digitalWrite(_in2, LOW);
  } else if (_speed < 0) {
    digitalWrite(_in1, LOW);
    digitalWrite(_in2, HIGH);
  } else {
    digitalWrite(_in1, LOW);
    digitalWrite(_in2, LOW);
  }
  
  analogWrite(_pwmA, pwmValue);
  analogWrite(_pwmB, pwmValue);
}

int MotorController::mapSpeed(int speed) {
  return map(abs(speed), 0, 100, 0, 255);
}
