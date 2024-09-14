//Youtube|Tech at Home
#include "DFRobot_SpeechSynthesis_V2.h"
#include <SoftwareSerial.h>
//SoftwareSerial ssSerial1(4,5);  //RX, TX
//DFRobot_SpeechSynthesis_UART ss;

int trig=7;
int echo=6;

long timeInMicro;

long distanceInCm;

void setup() 
{
//  ssSerial1.begin(115200);
//  //Init speech synthesis sensor
//  ss.begin(ssSerial1);
  Serial.begin(9600);
  pinMode(7,OUTPUT);
  pinMode(6,INPUT);
}

void loop()  
{
 digitalWrite(trig,LOW);
 delayMicroseconds(2);
 digitalWrite(trig,HIGH);
 delayMicroseconds(10);
 digitalWrite(trig,LOW);

 timeInMicro= pulseIn(echo,HIGH);

 distanceInCm = ((timeInMicro/29)/2);

 Serial.println(distanceInCm);
// if(distanceInCm <30)
// {
//  ss.speak(F("Obstacle detected"));
// }
// delay(100);
 
}
