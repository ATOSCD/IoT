

Servo myservo;

void setup() {
  myservo.attach(15); 
  myservo.write(0);
  delay(1000);
  myservo.write(180);
  delay(1000);
  myservo.write(90); 
}

void loop() {
  
}