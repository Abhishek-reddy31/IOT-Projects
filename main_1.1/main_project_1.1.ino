/*!
 * @file uart.ino
 * @brief Control speech synthesis sensor via UART, and synthetise speech
 * @details phenomena£ºthe speaker of the module plays English on a loop
 * @copyright   Copyright (c) 2010 DFRobot Co.Ltd (http://www.dfrobot.com)
 * @License     The MIT License (MIT)
 * @author [fengli](li.feng@dfrobot.com)
 * @version  V1.0
 * @date  2020-11-6
 * @url https://github.com/DFRobot/DFRobot_SpeechSynthesis_V2
 */
#include "DFRobot_SpeechSynthesis_V2.h"
#include <SoftwareSerial.h>
SoftwareSerial ssSerial1(4,5);  //RX, TX
DFRobot_SpeechSynthesis_UART ss;
#include<Servo.h>
Servo s1;
const int LED1 = 10;
const int LED2 = 11;
const int LED3 = 12;

void setup() {
  pinMode(LED1,OUTPUT);
  pinMode(LED2,OUTPUT);
  pinMode(LED3,OUTPUT);
  s1.attach(9);
  ssSerial1.begin(115200);
  //Init speech synthesis sensor
  ss.begin(ssSerial1);
  
  //Set voice volume to 5
  //ss.setVolume(5);
  //Set playback speed to 5
  //ss.setSpeed(5);
  //Set tone to 5
  //ss.setTone(5);
  //For English, speak word 
  //ss.setEnglishPron(ss.WORD);
}

void loop() {
  s1.write(0);
  delay(2000);
  s1.write(180);
  ss.speak(F("Locker Opened"));
  digitalWrite(LED1,HIGH);
  delay(5000);
  digitalWrite(LED1,LOW);
  s1.write(0);
  digitalWrite(LED2,HIGH);
  ss.speak(F("Locker closed"));
  
  delay(5000);
  digitalWrite(LED2,LOW);

  /*Use text control identifier*/
  //Voice volume identifier 
  //ss.speak(F("[v3]Hello [v8]world"));
  //Word Pronounce mode identifier
  //ss.speak(F("[h1]Hello [h2]world"));
}
