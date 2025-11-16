#include "Encoders_L.h"
#include <MotorController.h>
#include <Wire.h>

// Pines para los motores
MotorController leftMotor1(15, 14, 21, 20);
MotorController leftMotor2(15, 14, 21, 20);
MotorController rightMotor1(11, 12, 9, 10);
MotorController rightMotor2(11, 12, 9, 10);

// Pines del encoder
const int leftEncPinA = 23; 
const int leftEncPinB = 22;  
const int rightEncPinA = 0;
const int rightEncPinB = 1;

// Pines de Irs
const int sensorIzquierdo = A2;
const int sensorDerecho = A3;
const int fotoResistencia = A12;

Encoders_L encoders(leftEncPinA, leftEncPinB, rightEncPinA, rightEncPinB);

// Variables volátiles para comunicación entre interrupciones y loop principal
volatile int uno = 0;
volatile int dos = 0;
volatile float izq = 0;
volatile float der = 0;
volatile bool resetEncodersFlag = false;

// Variables para mediciones IR
volatile uint32_t valorIzquierdo = 0;
volatile uint32_t valorDerecho = 0;
volatile uint32_t valorFoto = 0;
volatile uint32_t valorProm = 0;

// Temporización no bloqueante
unsigned long lastUpdate = 0;
const unsigned long updateInterval = 10;  // 10 ms

void setup() {
  Serial.begin(9600);
  Wire.begin(9);  // Dirección I2C 0x09
  Wire.setClock(100000);  // 400 kHz
  pinMode(13, OUTPUT);
  pinMode(sensorIzquierdo, INPUT);
  pinMode(sensorDerecho, INPUT);

  Wire.onReceive(receiveEvent);
  Wire.onRequest(requestEvent);

  digitalWrite(13, HIGH);  // LED indicador
}

void loop() {
  if (millis() - lastUpdate >= updateInterval) {
    lastUpdate = millis();
    
    // Actualizar velocidades de motores
    updateMotors();
    
    // Manejar reset de encoders
    if (resetEncodersFlag) {
      encoders.resetCounts();
      resetEncodersFlag = false;
    }
    
    // Actualizar valores de sensores
    updateSensors();
    
    // Debug serial (opcional)
    printDebugInfo();
  }
}

void updateMotors() {
  leftMotor1.setSpeed(izq);
  leftMotor2.setSpeed(izq);
  rightMotor1.setSpeed(der);
  rightMotor2.setSpeed(der);
}

void updateSensors() {
  valorIzquierdo = analogRead(sensorIzquierdo);
  valorDerecho = analogRead(sensorDerecho);
  valorFoto = analogRead(fotoResistencia);
}

void printDebugInfo() {
  // Solo para depuración, puede desactivarse
  Serial.print("Encoders L/R: ");
  Serial.print(encoders.leftEncoderCount);
  Serial.print(" / ");
  Serial.print(encoders.rightEncoderCount);
  Serial.print(" | IRs: ");
  Serial.print(valorIzquierdo);
  Serial.print(" / ");
  Serial.print(valorDerecho);
  Serial.print(" | FotoR:");
  Serial.println(valorFoto);
}

void receiveEvent(int howMany) {
  while (Wire.available() >= 3) {
    // Leer comando y valor
    uno = Wire.read();
    int byteAlto = Wire.read();
    int byteBajo = Wire.read();
    dos = (byteAlto << 8) | byteBajo;

    // Procesar comandos
    switch (uno) {
      case 11:  // Motor izquierdo
        izq = (dos > 100) ? (dos - 100) * -1 : dos;
        break;
      case 12:  // Motor derecho
        der = (dos > 100) ? (dos - 100) * -1 : dos;
        break;
      case 13:  // Reset encoders
        resetEncodersFlag = true;
        break;
    }
  }
  
  // Limpiar buffer si hay bytes sobrantes
  while (Wire.available() > 0) Wire.read();
}

void requestEvent() {
  // Preparar datos
  float rotations_right = encoders.rightEncoderCount / 2987.7504;
  float rotations_left = encoders.leftEncoderCount / 2987.7504;
  
  // Crear buffer de 12 bytes (2 floats + 2 enteros de 32 bits)
  uint8_t buffer[16];
  
  // Copiar datos al buffer
  valorProm = (round((valorDerecho + valorIzquierdo)/2));
  memcpy(&buffer[0], &rotations_right, 4);
  memcpy(&buffer[4], &rotations_left, 4);
  memcpy(&buffer[8], &valorProm, 4);
  memcpy(&buffer[12], &valorFoto, 4);

  // Enviar todos los datos en una operación
  Wire.write(buffer, 16);
}

