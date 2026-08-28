#include <Arduino.h>
#include "Drop_Gen.h"
#include "Wifi_Cont.h"
#include "param.h"

#define TRIGGER (1 << param.TRIG)
#define SERVO_RES_BITS 14   // ESP32-S3 LEDC timers are 14 bit max

// setup function for the servo and camera
void Generator::dgSetup(){
  pinMode(param.LED,      OUTPUT);
  pinMode(param.TRIG,     OUTPUT);
  digitalWrite(param.LED, HIGH);

#if ESP_ARDUINO_VERSION_MAJOR >= 3
  bool servoReady = ledcAttach(param.SERVO, param.SERVO_FREQ, SERVO_RES_BITS);
#else
  bool servoReady = ledcSetup(param.SERVO_CHANNEL, param.SERVO_FREQ, SERVO_RES_BITS) != 0;
  if(servoReady) ledcAttachPin(param.SERVO, param.SERVO_CHANNEL);
#endif
  if(!servoReady) Serial.println("Servo PWM setup failed");
  writeAngle(param.closedAngle);   // Park the servo closed on boot
}

// single servo drop cycle with camera function
void Generator::dgStart(){
  if(!param.drop) return;

  Serial.print("Drop: open ");    Serial.print(param.openAngle);
  Serial.print(" deg, hold ");    Serial.print(param.openDelay);
  Serial.print(" s, close ");     Serial.print(param.closedAngle);
  Serial.println(" deg");
  generate();

  param.drop = false;
  wifiCont.requestUpdate();   // Let the page reset the Drop button
}

// Wait out the camera delay measured from the iris opening, then pulse the trigger
void Generator::camera(){
  delay(param.cameraDelay);
  GPIO.out_w1ts = TRIGGER;
  delayMicroseconds(50);
  GPIO.out_w1tc = TRIGGER;
}

// Open the iris, hold it open for the open delay, then close it again. The
// camera is triggered part way through the hold, timed from the moment it opens.
void Generator::generate(){
  unsigned long holdTime = (unsigned long)(param.openDelay * 1000.0);

  writeAngle(param.openAngle);
  unsigned long openedAt = millis();

  if(param.camera) camera();

  unsigned long elapsed = millis() - openedAt;
  if(elapsed < holdTime) delay(holdTime - elapsed);
  writeAngle(param.closedAngle);
}

// Convert an angle in degrees to a servo pulse and drive the servo pin with it
void Generator::writeAngle(int angle){
  angle = constrain(angle, 0, 180);
  int pulse = map(angle, 0, 180, param.SERVO_MIN_US, param.SERVO_MAX_US);
  uint32_t duty = ((uint32_t)pulse * (1UL << SERVO_RES_BITS)) / (1000000UL / param.SERVO_FREQ);
#if ESP_ARDUINO_VERSION_MAJOR >= 3
  ledcWrite(param.SERVO, duty);
#else
  ledcWrite(param.SERVO_CHANNEL, duty);
#endif
}
