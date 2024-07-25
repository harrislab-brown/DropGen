#ifndef PARAM_H
#define PARAM_H
#include <Preferences.h>

struct Param {

  public:
    
    // Wifi definitions.
    const bool WIFI_STATION = true;    // True for external WiFi connection
    const char* SSID        = "Brown-Guest";
    const char* PASSWORD    = "";
    const char* DNS_NAME    = "dropgen";
    
    // Drop generator pin definitions
    const int DIR_A      = 5;          // Piezo PWN pin 1
    const int DIR_B      = 4;          // Piezo PWN pin 2
    const int TRIG       = 6;          // Camera trigger
    const int LED        = 21;         // BUILTIN_LED

    // Motor pin definitions
    const int RES_DIAG    = 9;         // Reservoir motor diagnostic pin
    const int RES_STEP    = 7;         // Reservoir motor step pin
    const int RES_DIR     = 8;         // Reservoir motor direction pin
    const int RES_ENDSTOP = 3;         // Reservoir mechanical end stop pin

    // Drop generator parameters
    int pulseWidth       = 1000;       // Piezo pulse width in microseconds
    int dropDelay        = 500;        // Time between droplets in milliseconds
    int dropNum          = 100;        // Number of droplets
    int delayTime        = 15;         // Camera delay from drop pulse end in milliseconds
    volatile bool camera = false;      // Camera on/off
    volatile bool drop   = false;      // Droplet generator on/off
    
    // Motor variables
    bool calibrate       = false;      // Calibrate the motors
    double resMotPos     = 0.0;        // Reservoir motor position
    double travelLength  = 1;          // Motor travel length (mm)
    int SPEED            = 5000;       // Motor speed
    int CALIB_SPEED      = 2000;       // Calibration Motor speed
    int ACCEL            = 4000;       // Motor acceleration
    int LEAD_SIZE        = 1280;       // Motor round size (steps / mm)

    void loadParameters(Preferences &pref){
      pulseWidth       = pref.getInt("PW", pulseWidth);
      dropDelay        = pref.getInt("DD", dropDelay);
      dropNum          = pref.getInt("DN", dropNum);
      travelLength     = pref.getInt("TL", travelLength);
      delayTime        = pref.getInt("DT", delayTime);
      resMotPos        = pref.getInt("RMP", resMotPos);
    }
};
extern Param param;

#endif
