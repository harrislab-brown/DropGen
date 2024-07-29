#include <AccelStepper.h>
#include "Motor_Cont.h"
#include "param.h"
#include "Wifi_Cont.h"

// Initializing the motors
AccelStepper resStep(AccelStepper::DRIVER, param.RES_STEP, param.RES_DIR);

volatile int remember_pos = false;

extern WifiClass wifiCont;

void Motor::stop() {
    emergencyStop = true;
    if(resStep.isRunning()) {
      resStep.stop();
      isStopping = true; // Set the stopping flag
    }
}

void Motor::updateMotorStatus() {
    if (isStopping && !resStep.isRunning()) {
        isStopping = false; // Reset the stopping flag
        param.resMotPos = resStep.currentPosition();
        param.resMotPos = param.resMotPos / param.LEAD_SIZE; 
        wifiCont.update(true);
    }
}

void Motor::reset(){
  if(resStep.isRunning()){
    resStep.stop();
  }
  emergencyStop = false;
  param.resMotPos = 0; // reset position in millimeters stored in this class
  resStep.setCurrentPosition(0); // reset position in stepps in accelStepper class
}

// This function sets up the motor acceleration, position, and speed
void Motor::motorSetup(){

  resStep.setMaxSpeed(param.SPEED);
  resStep.setSpeed(param.SPEED);
  resStep.setAcceleration(param.ACCEL);
  resStep.setCurrentPosition(param.resMotPos);
  // initialize the motor endstop pin
  pinMode(param.RES_ENDSTOP, INPUT_PULLDOWN);

}

void Motor::moveBegin(){
  if(!resStep.isRunning() || emergencyStop) return;
  while(resStep.isRunning() && !emergencyStop){
    resStep.run();
  }
}

void Motor::moveRelative(int dir, int motor){
if(!resStep.isRunning()){
    param.resMotPos += param.travelLength * dir;
    resStep.moveTo(param.resMotPos * param.LEAD_SIZE);
    moveBegin();
  }
}

void Motor::moveToAbsolute(){
  if(!resStep.isRunning()){
    resStep.moveTo(param.resMotPos * param.LEAD_SIZE);
    moveBegin();
  } 
}

void Motor::calibrate(){
  Serial.println("Begin motor homeing sequence");
  resStep.setSpeed(param.CALIB_SPEED);
  //resStep.setAcceleration(20000);
  delay(100);

  // Run motor at constant speed until endstop is reached
  while(digitalRead(param.RES_ENDSTOP)){
    resStep.runSpeed();
  }
  Serial.println("endstop detected");

  param.calibrate = false;
  emergencyStop = false;
  resStep.setSpeed(param.SPEED);
  // zero motor to current position
  reset();

}

void Motor::lock(){
    remember_pos = resStep.currentPosition();
}

void Motor::return_to(){
    if (resStep.currentPosition() != remember_pos){
      param.resMotPos = remember_pos/param.LEAD_SIZE;
      resStep.moveTo(remember_pos);
      moveBegin();
    }
}