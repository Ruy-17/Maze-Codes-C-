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

    void resetCounts();  // Declaración del método para reiniciar los contadores

private:
    static Encoders_L* instance;  // Puntero estático a la instancia de la clase

    int leftEncPinA, leftEncPinB;
    int rightEncPinA, rightEncPinB;

    static int lastEncodedLeft;  // Variables para los encoders
    static int lastEncodedRight;
};

#endif
