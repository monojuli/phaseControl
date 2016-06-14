#ifndef PHASECONTROL_H_
#define PHASECONTROL_H_

#include "TimerOne.h"
#include "configuration.h"

#ifdef SERIAL_EN
#include "SerialCommand.h"
#include "SoftwareSerial.h"
extern SerialCommand cmd;
#endif

#include "temperature.h"
extern T_handler tmanager;

#include "PID_v1.h"

// TimerOne Interrupt function
void timerOne_INT();


/**
-----------------------------
  SERIAL COMMANDS HANDLING
-----------------------------
*/
void showTemp();
void toggleLed();
void unrecognized();
void setT1();
void setSTemp();
void report();

#endif
