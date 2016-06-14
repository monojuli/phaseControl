#ifndef TEMPERATURE_H_
#define TEMPERATURE_H_

#include "configuration.h"
#include "thermistortables.h"

//These two values limit the value of T1: T1+TB+delta<TSHALF
#define TB 500 //Time high in [us] //
#define TSHALF 8333 //Half Period of AC wave in [us]
#define T1MIN 100
#define T1MAX 7600

#define TMAX 300
#define TMIN 50




class T_handler{
  
  public:
  T_handler();
  void manage_heaters();
  void manage_newPhase(); //Handle new phase interrupt
  void manage_interrupt(); //Handle TimerOne interrupt
  void updateAllTemperatures();

  //Read only access
  float getTemp(int i);  //read only access to temperature
  int getT1(int i);
  int getScheduleTime(int i);
  int getScheduleHeater(int i);
  
  //Write access  
  void setT1(int newT1, int heater); //change T1 value for a heater
  void setT1(int *newT1); // change all T1 values
  void setSTemp(double newST, int heater); //change T1 value for a heater
  void setSTemp(double *newST); // change all T1 values

  
  //change implementation
  double Temp[HEATERS]; //last temperature measured from thermistors
  double STemp[HEATERS]; //Set point temperature vector
  //change implementation

  private:
  int Temp_raw[HEATERS]; //last raw data measured from thermistors
  int power[HEATERS]; //Power index of each heater 0-255 0 is off 255 is fully on
  int T1[HEATERS]; // Time to wait before turning on [us]
  int pin[HEATERS]; // Pin assigned to each heater
  
  
  /**------------------------------------------------------------------- **/
  // Watch out that this get updated as fast as you can to avoid problems with interrupts
  int * scheduleTime; //Time of interruptions
  int * scheduleHeater;//Heater activated on each scheduled time 
  /**------------------------------------------------------------------- **/
  
  void sortSchedule();
  
  long t; // Time of start of last new phase  
  
  float analog2temp(int raw);
 
  
 
  
};



#endif /* CONFIGURATION_H_*/
