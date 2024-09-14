#include <SoftwareSerial.h>

SoftwareSerial bt(8, 9); // RX, TX


int in1 = 2;
int in2 = 4;
int enA = 3;

int in3 = 5;
int in4 = 7;
int enB = 6;

int trigFront = A0;
int echoFront = A1;
int trigLeft = A2;
int echoLeft = A3;
int trigRight = A4;
int echoRight = A5;

long distanceFront;
long distanceLeft;
long distanceRight;

void setup() {
  Serial.begin(9600);
  
  bt.begin(9600);
  
  pinMode(enA, OUTPUT);
  pinMode(in1, OUTPUT);
  pinMode(in2, OUTPUT);
  pinMode(enB, OUTPUT);
  pinMode(in3, OUTPUT);
  pinMode(in4, OUTPUT);

  pinMode(trigFront, OUTPUT);
  pinMode(echoFront, INPUT);

  pinMode(trigLeft, OUTPUT);
  pinMode(echoLeft, INPUT);

  pinMode(trigRight, OUTPUT);
  pinMode(echoRight, INPUT);
}

void loop() {
  readUltrasonicSensors();
  if (distanceFront < 20 || distanceLeft < 20 || distanceRight < 20) {
    stopMotors();

    if (distanceLeft > distanceRight) {
      turnLeft();
      delay(1000);
    } else {
      turnRight();
      delay(1000);
    }

    readUltrasonicSensors();
    if (distanceFront < 20 && distanceLeft < 20 && distanceRight < 20) {
      moveBackwardUntilClear();
    }
  } else {
    moveForward();
  }


  
  if (bt.available()) {
//    Serial.println("Bluetooth data received:");
    String data = bt.readStringUntil('\n');
    Serial.println(data);
}
}

void readUltrasonicSensors() {
  distanceFront = getDistance(trigFront, echoFront);
  distanceLeft = getDistance(trigLeft, echoLeft);
  distanceRight = getDistance(trigRight, echoRight);

//  Serial.print("Front: ");
//  Serial.print(distanceFront);
//  Serial.print("  Left: ");
//  Serial.print(distanceLeft);
//  Serial.print("  Right: ");
//  Serial.println(distanceRight);
}

long getDistance(int trigPin, int echoPin) {
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);

  unsigned long duration = pulseIn(echoPin, HIGH);
  if (duration == 0) {
    return 0;
  }

  return duration / 29 / 2;
}

void moveForward() {
//  Serial.println("Moving Forward");
  while (true) {
    digitalWrite(in1, HIGH);
    digitalWrite(in2, LOW);
    digitalWrite(in3, HIGH);
    digitalWrite(in4, LOW);
    analogWrite(enA, 200);
    analogWrite(enB, 200);

    if (checkObstacles()) {
      break;
    }
  }
}

bool checkObstacles() {
  readUltrasonicSensors();
  if (distanceFront < 20 || distanceLeft < 20 || distanceRight < 20) {
    stopMotors();

    // Stop and check for a clear side
    delay(500);  // Adjust this delay based on your needs

    if (distanceLeft > distanceRight) {
//      Serial.println("Turning Left");
      turnLeft();
      delay(1000);
    } else {
//      Serial.println("Turning Right");
      turnRight();
      delay(1000);
    }

    readUltrasonicSensors();
    if (distanceFront < 20 && distanceLeft < 20 && distanceRight < 20) {
      moveBackwardUntilClear();
    }

    return true;
  }

  return false;
}

void moveBackwardUntilClear() {
  int clearSide;
  while (distanceFront < 20 && distanceLeft < 20 && distanceRight < 20) {
    digitalWrite(in1, LOW);
    digitalWrite(in2, HIGH);
    digitalWrite(in3, LOW);
    digitalWrite(in4, HIGH);
    analogWrite(enA, 200);
    analogWrite(enB, 200);
    delay(100);
    readUltrasonicSensors();
  }

  if (distanceFront >= 20) {
    clearSide = 0;
  } else if (distanceLeft >= 20) {
    clearSide = 1;
  } else if (distanceRight >= 20) {
    clearSide = 2;
  }

  stopMotors();

  if (clearSide == 0) {
//    Serial.println("Moving Forward");
    moveForward();
  } else if (clearSide == 1) {
//    Serial.println("Turning Left");
    turnLeft();
    delay(1000);
    stopMotors();
    delay(500);
//    Serial.println("Moving Forward");
    moveForward();
  } else if (clearSide == 2) {
//    Serial.println("Turning Right");
    turnRight();
    delay(1000);
    stopMotors();
    delay(500);
//    Serial.println("Moving Forward");
    moveForward();
  }
}


void stopMotors() {
  digitalWrite(in1, LOW);
  digitalWrite(in2, LOW);
  digitalWrite(in3, LOW);
  digitalWrite(in4, LOW);
  analogWrite(enA, 0);
  analogWrite(enB, 0);
}

void turnLeft() {
  digitalWrite(in1, LOW);
  digitalWrite(in2, LOW);
  digitalWrite(in3, HIGH);
  digitalWrite(in4, LOW);
  analogWrite(enA, 50);
  analogWrite(enB, 150);
}

void turnRight() {
  digitalWrite(in1, HIGH);
  digitalWrite(in2, LOW);
  digitalWrite(in3, LOW);
  digitalWrite(in4, LOW);
  analogWrite(enA, 150);
  analogWrite(enB, 50);
}
