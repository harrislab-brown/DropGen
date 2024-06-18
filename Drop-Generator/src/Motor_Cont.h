#ifndef Motor_Cont_H
#define Motor_Cont_H

#include "Wifi_Cont.h"
#include <AccelStepper.h>
class Motor{
  public:

  // Functions that are used by other controllers
    void motorSetup();
    void calibrate();
    void runMotor();
    void stop();
    void reset();
    void move(int, int);
    void moveToAbsolute();
    void lock();
    void return_to();
    void updateMotorStatus();

  private:
    void calculateDist(double&, AccelStepper&, int);
    bool isStopping = false;
};

#endif
