#include <Servo.h>

Servo myServo;
void setup()
{
  myServo.attach(9);
  pinMode(7, INPUT);
}

void loop()
{
  int detect = digitalRead(7);
  
  if(detect == HIGH){
    myServo.write(90);
    delay(100);
  }
  else{
    myServo.write(0);
    delay(100);
  }
}
