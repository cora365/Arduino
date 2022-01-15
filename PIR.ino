void setup()
{
  pinMode(8, OUTPUT);
  pinMode(7, INPUT);
}

void loop()
{
  int value = digitalRead(7);
  
  if(value == HIGH){
    digitalWrite(8,HIGH);
    delay(30);
  }
  else
    digitalWrite(8, LOW);
}
