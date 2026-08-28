#ifndef PARAM_H
#define PARAM_H
#include <Preferences.h>

struct Param {

  public:
    
    // Wifi definitions.
    const bool WIFI_STATION = true;    // True for external WiFi connection
    const char* SSID        = "Brown-Guest";
    const char* PASSWORD    = "";
    const char* DNS_NAME    = "Iris_Controller";
    
    // Drop generator pin definitions
    const int SERVO      = 4;          // Servo signal pin (L293D IN3 header, XIAO D3)
    const int TRIG       = 9;          // Camera trigger
    const int LED        = 21;         // BUILTIN_LED

    // Motor pin definitions
    // const int RES_DIAG    = 9;         // Reservoir motor diagnostic pin
    const int RES_STEP    = 7;         // Reservoir motor step pin
    const int RES_DIR     = 8;         // Reservoir motor direction pin
    const int RES_ENDSTOP = 3;         // Reservoir mechanical end stop pin

    // Drop generator parameters
    double openDelay     = 1.0;        // Time the servo stays open in seconds
    int openAngle        = 90;         // Servo angle for the open position in degrees
    int closedAngle      = 0;          // Servo angle for the closed position in degrees
    int cameraDelay      = 15;         // Camera delay from the iris opening in milliseconds
    volatile bool camera = false;      // Camera on/off
    volatile bool drop   = false;      // Droplet generator on/off
    
    // Servo pulse limits (microseconds) for the 9g servo travel of 0 - 180 degrees
    const int SERVO_MIN_US  = 500;
    const int SERVO_MAX_US  = 2400;
    const int SERVO_FREQ    = 50;      // Servo PWM frequency in Hz
    const int SERVO_CHANNEL = 4;       // LEDC channel used by the servo

    // Motor variables
    bool calibrate       = false;      // Calibrate the motors
    double resMotPos     = 0.0;        // Reservoir motor position
    double travelLength  = 1;          // Motor travel length (mm)
    int SPEED            = 5000;       // Motor speed
    int CALIB_SPEED      = 2000;       // Calibration Motor speed
    int ACCEL            = 4000;       // Motor acceleration
    int LEAD_SIZE        = 1280;       // Motor round size (steps / mm)

    void loadParameters(Preferences &pref){
      openDelay        = pref.getInt("OD", (int)(openDelay * 1000)) / 1000.0;
      openAngle        = pref.getInt("OA", openAngle);
      closedAngle      = pref.getInt("CA", closedAngle);
      travelLength     = pref.getInt("TL", (int)(travelLength * 1000)) / 1000.0;
      cameraDelay      = pref.getInt("DT", cameraDelay);
      resMotPos        = pref.getInt("RMP", resMotPos);
    }

    // Write the current configuration to NVS so it survives a power cycle.
    // The two fractional values are stored as thousandths to keep them integers.
    void saveParameters(Preferences &pref){
      pref.putInt("OD", (int)(openDelay * 1000));
      pref.putInt("OA", openAngle);
      pref.putInt("CA", closedAngle);
      pref.putInt("TL", (int)(travelLength * 1000));
      pref.putInt("DT", cameraDelay);
    }
};
extern Param param;

#endif
