#include <Arduino.h>
#include "Drop_Gen.h"
#include "param.h"

#define PIEZO_A (1 << param.DIR_A)
#define PIEZO_B (1 << param.DIR_B)
#define TRIGGER (1 << param.TRIG)
volatile int *pulsewidth = &param.pulseWidth;

// setup function for L298N H-bridge and camera
void Generator::dgSetup(){
  pinMode(param.LED,      OUTPUT);
  pinMode(param.DIR_A,    OUTPUT);
  pinMode(param.DIR_B,    OUTPUT);
  pinMode(param.TRIG,     OUTPUT);
  pinMode(param.RES_ENDSTOP, INPUT_PULLDOWN);
  digitalWrite(param.LED, HIGH);
}

// piezodisk trigger with camera function
void Generator::dgStart(){
  if(param.drop){
    Serial.println(*pulsewidth);
    noInterrupts();
    generate();
    interrupts();
    // if(param.camera) camera();
  }
  delay(param.dropDelay);
}

void Generator::camera(){
  delay(param.delayTime);
  GPIO.out_w1ts = TRIGGER;
  delayMicroseconds(6);
  GPIO.out_w1tc = TRIGGER;
}


void IRAM_ATTR Generator::generate(){
  GPIO.out_w1ts = PIEZO_A;  // Turn on left: HIGH
  GPIO.out_w1tc = PIEZO_B;  // Turn off right: LOW
  delayMicroseconds(*pulsewidth);  // Pulse width
  GPIO.out_w1tc = PIEZO_A;  // Turn off left: LOW
  GPIO.out_w1ts = PIEZO_B;  // Turn on right: HIGH
}
