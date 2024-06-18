#ifndef Wifi_Cont_H
#define Wifi_Cont_H

class Motor;

#include "Drop_Gen.h"
#include "Motor_Cont.h"

class WifiClass{
  public:
  
  // Functions that are used by other controllers
    void update(bool);
    void wfSetup(Motor &, Generator& generator);
    void wfRun(Motor &);

  private:
    void setupServer(Motor &, Generator& generator);
    void handleFile(char*, char*);
};

// Declare wifiCont as an extern variable
extern WifiClass wifiCont;

#endif