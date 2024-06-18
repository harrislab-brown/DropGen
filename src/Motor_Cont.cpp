#include <AccelStepper.h>
#include "Motor_Cont.h"
#include "param.h"
#include "Wifi_Cont.h"

// Initializing the motors
AccelStepper resStep(AccelStepper::DRIVER, param.RES_STEP, param.RES_DIR);

volatile bool stopRes = false;
volatile int remember_pos = false;

void IRAM_ATTR resStalled(){
  Serial.print("End Stop Triggered");
  if(digitalRead(param.RES_ENDSTOP) == LOW){
    resStep.setAcceleration(200000);
    resStep.moveTo(param.resMotPos);
    resStep.setAcceleration(param.ACCEL);
    stopRes = true;
  }
}

extern WifiClass wifiCont;

void Motor::stop() {
    if(resStep.isRunning()) {
      resStep.stop();
      isStopping = true; // Set the stopping flag
    }
}

void Motor::updateMotorStatus() {
    if (isStopping && !resStep.isRunning()) {
        isStopping = false; // Reset the stopping flag
        param.resMotPos = resStep.currentPosition();
        param.resMotPos = param.resMotPos / 1280;
        wifiCont.update(true);
    }
}

void Motor::reset(){
  if(resStep.isRunning()){
    resStep.stop();
  } 
  param.resMotPos = 0;
  resStep.setCurrentPosition(param.resMotPos);
}

// This function sets up the motor acceleration, position, and speed
void Motor::motorSetup(){

  resStep.setMaxSpeed(param.SPEED);
  resStep.setAcceleration(param.ACCEL);
  resStep.setCurrentPosition(param.resMotPos);
  // attachInterrupt(digitalPinToInterrupt(param.RES_ENDSTOP),
                                    //      resStalled, FALLING);
}

// This function runs the motors
void Motor::runMotor(){
  if(param.calibrate) calibrate();
  resStep.run();
}

void Motor::move(int dir, int motor){
  switch(motor) {
    default: // Generator Motor
      calculateDist(param.resMotPos, resStep, dir);
      break;
    }
}

void Motor::calculateDist(double& motorPos, AccelStepper& stepper, int dir) {
  if(!stepper.isRunning()){
    motorPos += param.travelLength * dir;
    stepper.moveTo(motorPos * param.LEAD_SIZE);
  }
}

void Motor::moveToAbsolute(){
  if(!resStep.isRunning()) resStep.moveTo(param.resMotPos * param.LEAD_SIZE);
}


void Motor::calibrate(){
  Serial.println("in calibrate");
  resStep.setSpeed(param.CALIB_SPEED);
  //param.resMotPos+=param.LEAD_SIZE;
  delay(100);
  while(digitalRead(param.RES_ENDSTOP)){
    resStep.runSpeed();
    // delay(100);
    // Serial.println("in loop");
  }
  Serial.println("endstop detected");
  param.resMotPos = 0;
  resStep.setCurrentPosition(0);
  param.calibrate = false;
//    stopRes = false;

}

void Motor::lock(){
    remember_pos = resStep.currentPosition();
}

void Motor::return_to(){
    if (resStep.currentPosition() != remember_pos){
       resStep.moveTo(remember_pos);
       param.resMotPos = remember_pos/1280;
    }
}