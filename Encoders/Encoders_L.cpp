#include "Encoders_L.h"

Encoders_L* Encoders_L::instance = nullptr;
int Encoders_L::lastEncodedLeft = 0;
int Encoders_L::lastEncodedRight = 0;

Encoders_L::Encoders_L(int leftEncPinA, int leftEncPinB, int rightEncPinA, int rightEncPinB)
    : leftEncPinA(leftEncPinA), leftEncPinB(leftEncPinB), rightEncPinA(rightEncPinA), rightEncPinB(rightEncPinB) {
    pinMode(leftEncPinA, INPUT_PULLUP);
    pinMode(leftEncPinB, INPUT_PULLUP);
    pinMode(rightEncPinA, INPUT_PULLUP);
    pinMode(rightEncPinB, INPUT_PULLUP);

    instance = this;  

    attachInterrupt(digitalPinToInterrupt(leftEncPinA), updateLeftEncoder, CHANGE);
    attachInterrupt(digitalPinToInterrupt(leftEncPinB), updateLeftEncoder, CHANGE);
    attachInterrupt(digitalPinToInterrupt(rightEncPinA), updateRightEncoder, CHANGE);
    attachInterrupt(digitalPinToInterrupt(rightEncPinB), updateRightEncoder, CHANGE);
}

void Encoders_L::updateLeftEncoder() {
    int MSB = digitalRead(instance->leftEncPinA);
    int LSB = digitalRead(instance->leftEncPinB);

    int encoded = (MSB << 1) | LSB;
    int sum = (lastEncodedLeft << 2) | encoded;

    if (sum == 0b1101 || sum == 0b0100 || sum == 0b0010 || sum == 0b1011) {
        instance->leftEncoderCount++;
    } else if (sum == 0b1110 || sum == 0b0111 || sum == 0b0001 || sum == 0b1000) {
        instance->leftEncoderCount--;
    }

    lastEncodedLeft = encoded;
}

void Encoders_L::updateRightEncoder() {
    int MSB = digitalRead(instance->rightEncPinA);
    int LSB = digitalRead(instance->rightEncPinB);

    int encoded = (MSB << 1) | LSB;
    int sum = (lastEncodedRight << 2) | encoded;

    if (sum == 0b1101 || sum == 0b0100 || sum == 0b0010 || sum == 0b1011) {
        instance->rightEncoderCount++;
    } else if (sum == 0b1110 || sum == 0b0111 || sum == 0b0001 || sum == 0b1000) {
        instance->rightEncoderCount--;
    }

    lastEncodedRight = encoded;
}

void Encoders_L::resetCounts() {
    leftEncoderCount = 0;
    rightEncoderCount = 0;
}

