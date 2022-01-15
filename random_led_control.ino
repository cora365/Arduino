int firefly[]={11,10,9,6,5};

void setup()
{
  for (int i=0; i<5; i++)
    pinMode(firefly[i],OUTPUT);
  Serial.begin(9600);
}

void loop()
{
  int index = random(5);
  Serial.println(index);
  
  for(int i=0; i<=255; i++){
    analogWrite(firefly[index], i);  
    delay(10);
  }
  
  for(int i=255; i>=0; i--){
    analogWrite(firefly[index], i);
    delay(10);
  }
}
