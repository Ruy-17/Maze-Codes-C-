#ifndef ENCODERS_L_H
#define ENCODERS_L_H

#include <Arduino.h>

class Encoders_L {
public:
    volatile long leftEncoderCount = 0;
    volatile long rightEncoderCount = 0;

    Encoders_L(int leftEncPinA, int leftEncPinB, int rightEncPinA, int rightEncPinB);

    static void updateLeftEncoder();
    static void updateRightEncoder();

    void resetCounts();  

private:
    static Encoders_L* instance;  
    int leftEncPinA, leftEncPinB;
    int rightEncPinA, rightEncPinB;

    static int lastEncodedLeft; 
    static int lastEncodedRight;
};

#endif

