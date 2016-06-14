#include <SerialCommand.h>


#include <TimerOne.h>
#include "phaseControl.h"

#include "temperature.h"
T_handler tmanager;
long t=0;

#define SERIAL_EN

#include <SoftwareSerial.h>
SerialCommand cmd;



#include "PID_v1.h"

void setup() {
  #define SERIAL_EN
  Serial.begin(115200);
  cmd.addCommand("T",showTemp);
  cmd.addCommand("D",toggleLed);
  cmd.addCommand("report",report);
  cmd.addCommand("setT1",setT1);      
  cmd.addCommand("setSTemp",setSTemp);
  cmd.addDefaultHandler(unrecognized); 
  Serial.println("Ready");
    
  pinMode(LED_PIN,OUTPUT);
  pinMode(ZERO_PIN,INPUT_PULLUP);
  digitalWrite(LED_PIN,LOW); 
  
  
  #if HEATERS > 0  
    pinMode(HEATER_A_PIN,OUTPUT);
  #endif
   #if HEATERS > 1
    pinMode(HEATER_B_PIN,OUTPUT);
  #endif
  #if HEATERS > 2
    pinMode(HEATER_C_PIN,OUTPUT);
  #endif
  #if HEATERS > 3
    pinMode(HEATER_D_PIN,OUTPUT);
  #endif
  #if HEATERS > 4
    pinMode(HEATER_E_PIN,OUTPUT);
  #endif
  
  //TEST
 // pinMode(HEATER_B_PIN,OUTPUT);
  
 // Timer1.initialize(1000000);
 // attachInterrupt(digitalPinToInterrupt(ZERO_PIN),newphase_INT,RISING);  
  
}

void loop() {
  //tmanager.manage_heaters();
  cmd.readSerial();  
//  Serial.print("hola");
//  report();
//  delay(1000);

}



/**
-----------------------------
      ISR INTERRUPTS
-----------------------------
*/


void newphase_INT(){
  tmanager.manage_newPhase();
}

void timerOne_INT(){
  tmanager.manage_interrupt();
}


