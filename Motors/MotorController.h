#ifndef MOTORCONTROLLER_H
#define MOTORCONTROLLER_H

#include <Arduino.h>

class MotorController {
  public:
    MotorController(int pwmA, int pwmB, int in1, int in2);
    void setSpeed(int speed);
    
  private:
    int _pwmA, _pwmB, _in1, _in2;
    int _speed;
    int mapSpeed(int speed);
};

#endif
