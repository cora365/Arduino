const int RED = 11;
const int BLUE = 10;
const int GREEN = 9;
void setup()
{  
  pinMode(RED, OUTPUT);
  pinMode(BLUE, OUTPUT);
  pinMode(GREEN, OUTPUT);
}

void loop()
{
  int redvalue =random(256);
  int bluevalue = random(256);
  int greenvalue = random(256);
  analogWrite(RED, redvalue);
  analogWrite(BLUE, bluevalue);
  analogWrite(GREEN, greenvalue);
  delay (1000);
}
