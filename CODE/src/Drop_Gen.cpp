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
  digitalWrite(param.LED, HIGH);
}

// piezodisk trigger with camera function
void Generator::dgStart(){
  int drop_index = 0;
  while(drop_index < param.dropNum && param.drop){
    Serial.println(*pulsewidth);
    noInterrupts();
    generate();
    interrupts();
    drop_index ++;

    int delayTime = param.dropDelay;
    if(param.camera){
     camera();
     delayTime -= param.cameraDelay;
    }
    delay(delayTime);
    
  }
  param.drop = false;

  // if(param.drop){
  //   Serial.println(*pulsewidth);
  //   noInterrupts();
  //   generate();
  //   interrupts();
  //   if(param.camera) camera();
  // }
}

void Generator::camera(){
  delay(param.cameraDelay);
  GPIO.out_w1ts = TRIGGER;
  delayMicroseconds(50);
  GPIO.out_w1tc = TRIGGER;
}


void IRAM_ATTR Generator::generate(){
  GPIO.out_w1ts = PIEZO_A;  // Turn on left: HIGH
  GPIO.out_w1tc = PIEZO_B;  // Turn off right: LOW
  delayMicroseconds(*pulsewidth);  // Pulse width
  GPIO.out_w1tc = PIEZO_A;  // Turn off left: LOW
  GPIO.out_w1ts = PIEZO_B;  // Turn on right: HIGH
}
