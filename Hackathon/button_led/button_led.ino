const int led=5;
const int button=16;
int temp;
void setup() {
  Serial.begin(9600);
  // put your setup code here, to run once:
  pinMode(led,OUTPUT);
  pinMode(button,INPUT);
}

void loop() {
  temp = digitalRead(button);
  if(temp == !HIGH){
    digitalWrite(led,HIGH);
    Serial.println("LED Turned ON");
    delay(1000);
  }
  else{
    digitalWrite(led,LOW);
    Serial.println("LED Turned OFF");
    delay(1000);
  }
  // put your main code here, to run repeatedly:

}
