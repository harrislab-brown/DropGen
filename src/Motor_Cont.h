#ifndef Motor_Cont_H
#define Motor_Cont_H

#include "Wifi_Cont.h"
#include <AccelStepper.h>
class Motor{
  public:

  // Functions that are used by other controllers
    void motorSetup();
    void calibrate();
    void stop();
    void reset();
    void moveRelative(int, int);
    void moveToAbsolute();
    void moveBegin();
    void lock();
    void return_to();
    void updateMotorStatus();

  private:
    bool isStopping = false;
    bool emergencyStop = false;
};

#endif
