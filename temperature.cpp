#include "phaseControl.h"
#include "temperature.h"
#include "thermistortables.h"
#include "Arduino.h"

#include "PID_v1.h"


static bool meas_ready=false;
static int rawdata[HEATERS]={0};
static int scheduleIndex=0;

static int scheduleTimeTmp1[HEATERS];
static int scheduleHeaterTmp1[HEATERS];
static int scheduleTimeTmp2[HEATERS];
static int scheduleHeaterTmp2[HEATERS];
static int * scheduleTimeTmp=scheduleTimeTmp1;
static int * scheduleHeaterTmp=scheduleTimeTmp1;
static int sortedT1[HEATERS];
static int sortedHeaters[HEATERS];

static long t2;



static double pidOut[HEATERS]={0};

#ifdef PID_EN
static double kp=10;
static double ki=2;
static double kd=1;
#if HEATERS > 0  
    static PID heaterA_pid(&tmanager.Temp[0],&pidOut[0],&tmanager.STemp[0],kp,ki,kd,REVERSE);
#endif
#if HEATERS > 1
    static PID heaterB_pid(&tmanager.Temp[1],&pidOut[1],&tmanager.STemp[1],kp,ki,kd,REVERSE);
#endif
#if HEATERS > 2
    static PID heaterC_pid(&tmanager.Temp[2],&pidOut[2],&tmanager.STemp[2],kp,ki,kd,REVERSE);
#endif
#if HEATERS > 3
    static PID heaterD_pid(&tmanager.Temp[3],&pidOut[3],&tmanager.STemp[3],kp,ki,kd,REVERSE);
#endif
#if HEATERS > 4
    static PID heaterE_pid(&tmanager.Temp[4],&pidOut[4],&tmanager.STemp[4],kp,ki,kd,REVERSE);
#endif
#endif



T_handler::T_handler(){
   noInterrupts();
   scheduleTime=scheduleTimeTmp1;
   scheduleHeater=scheduleHeaterTmp1;
   
  #ifdef PID_EN
    #if HEATERS > 0  
       heaterA_pid.SetMode(AUTOMATIC);
       heaterA_pid.SetOutputLimits(-T1MAX/2,T1MAX/2);
       heaterA_pid.SetSampleTime(TC);  
  #endif
  #if HEATERS > 1
       heaterB_pid.SetMode(AUTOMATIC);
       heaterB_pid.SetOutputLimits(-T1MAX/2,T1MAX/2);
       heaterB_pid.SetSampleTime(TC);
  #endif
  #if HEATERS > 2
       heaterC_pid.SetMode(AUTOMATIC);
       heaterC_pid.SetOutputLimits(-T1MAX/2,T1MAX/2);
       heaterC_pid.SetSampleTime(TC);
  #endif
  #if HEATERS > 3
       heaterD_pid.SetMode(AUTOMATIC);
       heaterD_pid.SetOutputLimits(-T1MAX/2,T1MAX/2);
       heaterD_pid.SetSampleTime(TC);  
  #endif
  #if HEATERS > 4
       heaterE_pid.SetMode(AUTOMATIC);
       heaterE_pid.SetOutputLimits(-T1MAX/2,T1MAX/2);
       heaterE_pid.SetSampleTime(TC);
  #endif
  #endif
  
  #if HEATERS > 0  
    pin[0]=HEATER_A_PIN;      
    T1[0]=1000;
  #endif
  #if HEATERS > 1
    pin[1]=HEATER_B_PIN;
    T1[1]=1000;
  #endif
  #if HEATERS > 2
    pin[2]=HEATER_C_PIN;
    T1[2]=1000;
  #endif
  #if HEATERS > 3
    pin[3]=HEATER_D_PIN;
    T1[3]=1000;  
  #endif
  #if HEATERS > 4
    pin[4]=HEATER_E_PIN;
    T1[4]=1000;
  #endif
  sortSchedule();
  interrupts();
}

void T_handler::manage_heaters(){
  static int meas_count=0;
  static long lastTimeMeas=0;
  static long lastTimeControl=0;
  bool report_=false;
  long time=micros(); 
  //Measure a new Temperature value each TS us
  if (time>TS+lastTimeMeas){
      if (!meas_ready){
          for (int i=0;i<HEATERS;i++)rawdata[i]+=analogRead(i);
          meas_count++;      
     }
     lastTimeMeas=time;
   }
   //If all samples are done update raw data
   if (meas_count>=OVERSAMPLENR){
     for (int i=0;i<HEATERS;i++){
       Temp_raw[i]=rawdata[i];
       rawdata[i]=0;
     }
     meas_ready=true;     
     meas_count=0;     
     updateAllTemperatures();
   }   
   
   //Compute new input of the PID
   #ifdef PID_EN
   #if HEATERS > 0  
    if (heaterA_pid.Compute()){
        setT1( pidOut[0]+T1MIN+T1MAX/2,0);
        report_=true;
      }
    #endif
  #if HEATERS > 1
    if (heaterB_pid.Compute()){
        setT1( pidOut[1]+T1MIN+T1MAX/2,1);
        report_=true;
      }
#endif
   #if HEATERS > 2
    if (heaterC_pid.Compute()){
        setT1( pidOut[2]+T1MIN+T1MAX/2,2);
        report_=true;
      }
   #endif
   #if HEATERS > 3
    if (heaterD_pid.Compute()){
        setT1( pidOut[3]+T1MIN+T1MAX/2,3);
        report_=true;
      }
   #endif
   #if HEATERS > 4
    if (heaterE_pid.Compute()){
        setT1( pidOut[4]+T1MIN+T1MAX/2,4);
        report_=true;
      }
    #endif
    if (report_==true){
      report();
      report_=false;
    }
  #endif 
 }
 
void T_handler::updateAllTemperatures(){
  //Only if all raw measurements are ready
  if (meas_ready){
    for (int i=0;i<HEATERS;i++)Temp[i]=analog2temp(Temp_raw[i]);
    meas_ready=false;
  }
}

void T_handler::sortSchedule(){
  // Calculate schedule of interrupts from T1 vector  
  int swapT1,swapH;
  for (int i=0;i<HEATERS;i++){
    sortedT1[i]=T1[i];
    sortedHeaters[i]=i;
  }  
  for (int i=0;i<HEATERS;i++){
    for (int j=0;j<HEATERS-i-1;j++){
       if(sortedT1[j]>sortedT1[j+1]){
         swapT1=sortedT1[j];
         swapH=sortedHeaters[j];
         sortedT1[j]=sortedT1[j+1];
         sortedHeaters[j]=sortedHeaters[j+1];
         sortedT1[j+1]=swapT1;
         sortedHeaters[j+1]=swapH;        
       }
  }
  }
  
  if (scheduleTime==scheduleTimeTmp1){
    scheduleTimeTmp=scheduleTimeTmp2;
    scheduleHeaterTmp=scheduleHeaterTmp2;
  }
  else {
    scheduleTimeTmp=scheduleTimeTmp1;
    scheduleHeaterTmp=scheduleHeaterTmp1;
  }
  
  for (int i=HEATERS-1;i>0;i--){
    scheduleTimeTmp[i]=sortedT1[i]-sortedT1[i-1];
    scheduleHeaterTmp[i]=sortedHeaters[i];
  }
    scheduleTimeTmp[0]=sortedT1[0];
    scheduleHeaterTmp[0]=sortedHeaters[0];
  
  //Fast swap of schedule vectors  
  //1. Keep a reference to current vector space  
  scheduleTime=scheduleTimeTmp;
  scheduleHeater=scheduleHeaterTmp;
}

void T_handler::manage_newPhase(){
  t=micros();
  scheduleIndex=0;
  Timer1.attachInterrupt(timerOne_INT,scheduleTime[0]);
  //Interrupt times are scheduled 
}

void T_handler::manage_interrupt(){
  t2=micros();
  if (scheduleIndex<HEATERS){
      digitalWrite(pin[scheduleHeater[scheduleIndex]],HIGH);
      scheduleIndex++;
      while (scheduleIndex<HEATERS && scheduleTime[scheduleIndex]<100){ //If next schedule doesn't produce an interrupt (i.e. is simultaneous)
        digitalWrite(pin[scheduleHeater[scheduleIndex]],HIGH);
        scheduleIndex++;
      }
      if (scheduleIndex<HEATERS){
        Timer1.attachInterrupt(timerOne_INT,scheduleTime[scheduleIndex]);
      }
      else
      {
        Timer1.attachInterrupt(timerOne_INT,TB);  
      }
      
  }
  else 
  {
      for (int i=0;i<HEATERS;i++){
        digitalWrite(pin[i],LOW);
      }
      scheduleIndex=0;
      Timer1.stop(); //
  }
}

/* ------------------------------------------------
                HELPER FUNCTIONS
---------------------------------------------------*/
#define PGM_RD_W(x)   (short)pgm_read_word(&x)
float T_handler::analog2temp(int raw){
    float celsius = 0;
    uint8_t i;
    short (*tt)[][2] = (short (*)[][2])HEATER_0_TEMPTABLE;
    for (i = 1; i < HEATER_0_TEMPTABLE_LEN; i++)
    {
      if (PGM_RD_W(HEATER_0_TEMPTABLE[i][0]) > raw)
      {   
        celsius = PGM_RD_W(HEATER_0_TEMPTABLE[i - 1][1])+
                  (raw - PGM_RD_W(HEATER_0_TEMPTABLE[i - 1][0])) * 
                  (float)(PGM_RD_W(HEATER_0_TEMPTABLE[i][1]) - PGM_RD_W(HEATER_0_TEMPTABLE[i - 1][1])) 
                  / (float)(PGM_RD_W(HEATER_0_TEMPTABLE[i][0]) - PGM_RD_W(HEATER_0_TEMPTABLE[i - 1][0]));
        break;
      }
    }    
    // Overflow: Set to last value in the table
    //if (i == HEATER_0_TEMPTABLE_LEN) celsius = HEATER_0_TEMPTABLE[i - 1][1];
    return celsius;
}


/* ------------------------------------------------
                READ ACCESS FUNCTIONS
---------------------------------------------------*/
float T_handler::getTemp(int i){
  return Temp[i];
}

int T_handler::getT1(int i){
  return T1[i];
}

int T_handler::getScheduleTime(int i){
 return scheduleTime[i]; 
}

int T_handler::getScheduleHeater(int i){
 return scheduleHeater[i]; 
}


/* ------------------------------------------------
                WRITE ACCESS FUNCTIONS
---------------------------------------------------*/
void T_handler::setSTemp(double * newSTemp){
  for (int i=0;i<HEATERS;i++){
  if (newSTemp[i]<MAXTEMP){
    STemp[i]=newSTemp[i];
  }
}
}

void T_handler::setSTemp(double newSTemp, int heater){
if (0<heater<HEATERS && newSTemp<MAXTEMP) {
    STemp[heater]=newSTemp;
  }
}

void T_handler::setT1(int *newT1){
  for (int i=0;i<HEATERS;i++){   
    T1[i]=newT1[i];
  }   
  sortSchedule();
}

void T_handler::setT1(int newT1, int heater){
  if (0<heater<HEATERS){   
    T1[heater]=newT1;
  }   
  sortSchedule();
}

