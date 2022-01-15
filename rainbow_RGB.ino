int R = 11;
int G = 10;
int B = 9;

void setup() {
  pinMode(R, OUTPUT);
  pinMode(G, OUTPUT);
  pinMode(B, OUTPUT);
  Serial.begin(9600);
}

void loop() {
  if(Serial.available()){
    char value=Serial.read();
    
    switch(value){
      case '1' : // 빨강
        analogWrite(R,255);
        analogWrite(G,0);
        analogWrite(B,0);
        break;
      case '2' : // 주황
        analogWrite(R,255);
        analogWrite(G,20);
        analogWrite(B,0);
        break;
      case '3' : //노랑
        analogWrite(R,255);
        analogWrite(G,200);
        analogWrite(B,0);
        break;
      case '4' : //초록
        analogWrite(R,0);
        analogWrite(G,255);
        analogWrite(B,0);
        break;
      case '5' : //파랑
        analogWrite(R,0);
        analogWrite(G,191);
        analogWrite(B,255);
        break;
      case '6' : //남색
        analogWrite(R,0);
        analogWrite(G,0);
        analogWrite(B,255);
        break;
      case '7' : //보라
        analogWrite(R,255);
        analogWrite(G,0);
        analogWrite(B,255);
        break;
      default:
        analogWrite(R,0);
        analogWrite(G,0);
        analogWrite(B,0);
    }    
  }
}
