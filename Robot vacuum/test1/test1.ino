#include <SoftwareSerial.h>

SoftwareSerial bt(12, 13);
int in1 = 2;
int in2 = 4;
int enA = 3;

int in3 = 5;
int in4 = 7;
int enB = 6;

int trigfront = A0; // Connect the trigger pin to analog pin A0
int echofront = A1; // Connect the echo pin to analog pin A1
int trigleft = A2; 
int echoleft = A3;
int trigright = A4; 
int echoright = A5;
long timeInMicroF;
long timeInMicroL;
long timeInMicroR;
long distancefront;
long distanceleft;
long distanceright;

void setup() {
  Serial.begin(9600);
  bt.begin(9600);
  pinMode(trigfront, OUTPUT);
  pinMode(echofront, INPUT);
  pinMode(trigleft, OUTPUT);
  pinMode(echoleft, INPUT);
  pinMode(trigright, OUTPUT);
  pinMode(echoright, INPUT);
  // put your setup code here, to run once:
  pinMode(enA, OUTPUT);
  pinMode(in1, OUTPUT);
  pinMode(in2, OUTPUT);
  pinMode(enB, OUTPUT);
  pinMode(in3, OUTPUT);
  pinMode(in4, OUTPUT);
  // moveForward();
  // delay(1500);
  // moveBackward();
  // delay(1500);
}

void loop() {
  // put your main code here, to run repeatedly:
  if (bt.available()) {
    // Read the incoming data until a newline character is received
    String data = bt.readStringUntil('\n');
    Serial.println(data);

    if (data.equals("move forward")) {
      moveForward();
    } else if (data.equals("move backward")) {
      moveBackward();
    } else if (data.equals("turn left")) {
      turnLeft();
    } else if (data.equals("turn right")) {
      turnRight();
    } else if (data.equals("stop")) {
      stopMotors();
    }
  }
//front part
  digitalWrite(trigfront, LOW);
  delayMicroseconds(2);
  digitalWrite(trigfront, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigfront, LOW);

  timeInMicroF = pulseIn(echofront, HIGH);

  distancefront = ((timeInMicroF / 29) / 2);//distanceInCm

  Serial.println("FRONT: " + String(distancefront) + "\r");
  delay(100);


//left part
  digitalWrite(trigleft, LOW);
  delayMicroseconds(2);
  digitalWrite(trigleft, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigleft, LOW);

  timeInMicroL = pulseIn(echoleft, HIGH);

  distanceleft = ((timeInMicroL / 29) / 2);

  Serial.println("LEFT: " + String(distanceleft) + "\r");
  delay(100);


//right part
  digitalWrite(trigright, LOW);
  delayMicroseconds(2);
  digitalWrite(trigright, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigright, LOW);

  timeInMicroR = pulseIn(echoright, HIGH);

  distanceright = ((timeInMicroR / 29) / 2);

  Serial.println("RIGHT: " + String(distanceright) + "\r");
  delay(100);

  delay(1000);
}

void moveForward() {
  digitalWrite(in1, HIGH);
  digitalWrite(in2, LOW);
  digitalWrite(in3, HIGH);
  digitalWrite(in4, LOW);
  analogWrite(enA, 100);
  analogWrite(enB, 100);
}

void moveBackward() {
  digitalWrite(in1, LOW);
  digitalWrite(in2, HIGH);
  digitalWrite(in3, LOW);
  digitalWrite(in4, HIGH);
  analogWrite(enA, 100);
  analogWrite(enB, 100);
}

void stopMotors() {
  digitalWrite(in1, LOW);
  digitalWrite(in2, LOW);
  digitalWrite(in3, LOW);
  digitalWrite(in4, LOW);
  analogWrite(enA, 100);
  analogWrite(enB, 100);
}

void turnLeft() {
  digitalWrite(in1, LOW);
  digitalWrite(in2, LOW);
  digitalWrite(in3, HIGH);
  digitalWrite(in4, LOW);
  analogWrite(enA, 100);
  analogWrite(enB, 100);
}

void turnRight() {
  digitalWrite(in1, HIGH);
  digitalWrite(in2, LOW);
  digitalWrite(in3, LOW);
  digitalWrite(in4, LOW);
  analogWrite(enA, 100);
  analogWrite(enB, 100);
}
