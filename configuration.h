#ifndef CONFIGURATION_H_
#define CONFIGURATION_H_

#define LED_PIN 13

//Zero crossing signal
#define ZERO_PIN 2 

//Number of heaters and pin assigned to each one
#define HEATERS 4
#define HEATER_A_PIN 7
#define HEATER_B_PIN 6
#define HEATER_C_PIN 5
#define HEATER_D_PIN 4
#define HEATER_E_PIN 3

//enable serial comunication
#define SERIAL_EN 

//enable PID control

//#define PID_EN

//#if ZERO_PIN==2
  //#define ZERO_INT 1
//#endif

#define MAXTEMP 250

//Type of temperature sensor (thermistor tables from reprap Marlin)
#define THERMISTORHEATER_0 1 

#define TS 100000 //Sample period [us]
#define TC 2000 //Control update period [ms]

#endif /* CONFIGURATION_H_*/
