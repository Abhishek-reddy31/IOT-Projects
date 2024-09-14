

int trig = 9;
int echo = 10;

long timeInMicro;
long distanceInCm;

void setup() {
  Serial.begin(9600);
  pinMode(trig, OUTPUT);
  pinMode(echo, INPUT);
}

void loop() {
  digitalWrite(trig, LOW);
  delayMicroseconds(2);
  digitalWrite(trig, HIGH);
  delayMicroseconds(10);
  digitalWrite(trig, LOW);

  timeInMicro = pulseIn(echo, HIGH);

  distanceInCm = ((timeInMicro / 29) / 2);

  Serial.println(distanceInCm);
  delay(100);
}
