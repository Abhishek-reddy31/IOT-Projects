/**
  ******************************************************************************
  * @file    vr_sample_control_led.ino
  * @author  JiapengLi
  * @brief   This file provides a demostration on 
              how to control led by using VoiceRecognitionModule
  ******************************************************************************
  * @note:
        voice control led
  ******************************************************************************
  * @section  HISTORY
    
    2013/06/13    Initial version.
  */

#include "DFRobot_SpeechSynthesis_V2.h"
#include <SoftwareSerial.h>
#include "VoiceRecognitionV3.h"
SoftwareSerial ssSerial1(4,5);  //RX, TX
DFRobot_SpeechSynthesis_UART ss;


/**        
  Connection
  Arduino    VoiceRecognitionModule
   2   ------->     TX
   3   ------->     RX
*/
VR myVR(2,3);    // 2:RX 3:TX, you can choose your favourite pins.

uint8_t records[7]; // save record
uint8_t buf[64];

int led = 13;

#define On    (0)
#define Off   (1) 

/**
  @brief   Print signature, if the character is invisible, 
           print hexible value instead.
  @param   buf     --> command length
           len     --> number of parameters
*/
void printSignature(uint8_t *buf, int len)
{
  int i;
  for(i=0; i<len; i++){
    if(buf[i]>0x19 && buf[i]<0x7F){
      Serial.write(buf[i]);
    }
    else{
      Serial.print("[");
      Serial.print(buf[i], HEX);
      Serial.print("]");
    }
  }
}

/**
  @brief   Print signature, if the character is invisible, 
           print hexible value instead.
  @param   buf  -->  VR module return value when voice is recognized.
             buf[0]  -->  Group mode(FF: None Group, 0x8n: User, 0x0n:System
             buf[1]  -->  number of record which is recognized. 
             buf[2]  -->  Recognizer index(position) value of the recognized record.
             buf[3]  -->  Signature length
             buf[4]~buf[n] --> Signature
*/
void printVR(uint8_t *buf)
{
  Serial.println("VR Index\tGroup\tRecordNum\tSignature");

  Serial.print(buf[2], DEC);
  Serial.print("\t\t");

  if(buf[0] == 0xFF){
    Serial.print("NONE");
  }
  else if(buf[0]&0x80){
    Serial.print("UG ");
    Serial.print(buf[0]&(~0x80), DEC);
  }
  else{
    Serial.print("SG ");
    Serial.print(buf[0], DEC);
  }
  Serial.print("\t");

  Serial.print(buf[1], DEC);
  Serial.print("\t\t");
  if(buf[3]>0){
    printSignature(buf+4, buf[3]);
  }
  else{
    Serial.print("NONE");
  }
  Serial.println("\r\n");
}

void setup()
{
//  pinMode(LED1,OUTPUT);
//  pinMode(LED2,OUTPUT);
//  pinMode(LED3,OUTPUT);
  /** initialize */
  ssSerial1.begin(115200);
  //Init speech synthesis sensor
  ss.begin(ssSerial1);
  myVR.begin(9600);
  Serial.begin(115200);
  Serial.println("Elechouse Voice Recognition V3 Module\r\nControl LED sample");
  
  pinMode(led, OUTPUT);
    
  if(myVR.clear() == 0){
    Serial.println("Recognizer cleared.");
  }else{
    Serial.println("Not find VoiceRecognitionModule.");
    Serial.println("Please check connection and restart Arduino.");
    while(1);
  }
  
  if(myVR.load((uint8_t)On) >= 0){
    Serial.println("onRecord loaded");
  }
  
  if(myVR.load((uint8_t)Off) >= 0){
    Serial.println("offRecord loaded");
  }
}

void loop()
{
//  digitalWrite(LED1,HIGH);
//  delay(1000);
//  digitalWrite(LED1,LOW);
//  digitalWrite(LED2,HIGH);
//  digitalWrite(LED3,HIGH);
//  s1.write(0);
  int ret;
  ret = myVR.recognize(buf, 50);
  if(ret>0){
    if(buf[1] == On){
        Serial.println("Command: ON");
        /** turn on LED */
        digitalWrite(led, HIGH);
        delay(2000);
        ss.speak(F("Turned On"));
      
    }
        
    else if (buf[1] == Off) {
      Serial.println("Command: OFF");
  /** turn off LED */
  //digitalWrite(led, LOW);
  ss.speak(F("Turned Off"));
  
}
    /** voice recognized */
    printVR(buf);
    
  }
}
