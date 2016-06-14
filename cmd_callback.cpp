#include "phaseControl.h"

/**
-----------------------------
  SERIAL COMMANDS HANDLING
-----------------------------
*/
void showTemp(){
  int i;
  Serial.print("Measured Temperature: ");
  for (i=0;i<HEATERS-1;i++){
    Serial.print(tmanager.getTemp(i));
    Serial.print(",");       
  }
  Serial.print(tmanager.getTemp(i));
  Serial.println(" [Celsius]");
  
  Serial.print("Setpoint Temperature: ");
  for (i=0;i<HEATERS-1;i++){
    Serial.print(tmanager.STemp[i]);
    Serial.print(",");       
  }
  Serial.print(tmanager.STemp[i]);
  Serial.println(" [Celsius]");
  
}

void toggleLed(){
  digitalWrite(LED_PIN, !digitalRead(LED_PIN));
 // digitalWrite(HEATER_B_PIN, !digitalRead(HEATER_B_PIN));
  Serial.println(analogRead(A0));
}
void unrecognized(){
  Serial.println("?");
}

void setT1(){
  char * arg;
  int i;
  int newT1[HEATERS];
  Serial.print("New T1:"); 
  for (i=0;i<HEATERS;i++){
    arg=cmd.next();  
     if (arg != NULL)      // As long as it existed, take it
     {     
       newT1[i]=atoi(arg);
       Serial.print(newT1[i]);
       Serial.print(" ");
     }
     else {
       Serial.println("'Invalid arguments'");
       return;
     }     
  }
  if (i!=HEATERS){
       Serial.println("'Invalid arguments'");
       return;    
  } 
  Serial.println(';');
  tmanager.setT1(newT1);
}

void setSTemp(){
  char * arg;
  int i;
  double newSTemp[HEATERS];
  Serial.print("New ST:"); 
  for (i=0;i<HEATERS;i++){
    arg=cmd.next();  
     if (arg != NULL)      // As long as it existed, take it
     {     
       newSTemp[i]=atof(arg);
       Serial.print(newSTemp[i]);
       Serial.print(" ");
     }
     else {
       Serial.println("Invalid arguments'");
       return;
     }     
  }
  if (i!=HEATERS){
       Serial.println("'Invalid arguments'");
       return;    
  } 
  Serial.println(';');
  tmanager.setSTemp(newSTemp);
}

void report(){
  int i;
  showTemp();
  Serial.print("T1: ");  
  for (i=0;i<HEATERS-1;i++){
    Serial.print(tmanager.getT1(i));
    Serial.print(",");       
  }
    Serial.print(tmanager.getT1(i));
    Serial.println(" [us]");

  Serial.print("ScheduleTime: ");   
  for (i=0;i<HEATERS-1;i++){
    Serial.print(tmanager.getScheduleTime(i));
    Serial.print(",");       
  }
    Serial.print(tmanager.getScheduleTime(i));
    Serial.println(" [us]");

  Serial.print("ScheduleHeater: ");   
  for (i=0;i<HEATERS-1;i++){
    Serial.print(tmanager.getScheduleHeater(i));
    Serial.print(",");       
  }
    Serial.print(tmanager.getScheduleHeater(i));
    Serial.println(" [us]");      
    
    
}
