void setup()
{
  pinMode(11, OUTPUT);
  pinMode(10, OUTPUT);
  pinMode(9, OUTPUT);
  Serial.begin(9600);
}

void loop()
{
  if(Serial.available()){
    char value=Serial.read();
    
    switch(value){
      case '1' :
        digitalWrite(11,HIGH);
        digitalWrite(10,LOW);
        digitalWrite(9,LOW);
        break;
      case '2' :
        digitalWrite(12,HIGH);
        break;
      default:
        digitalWrite(13,LOW);
        digitalWrite(12,LOW);
    }
  }
}
