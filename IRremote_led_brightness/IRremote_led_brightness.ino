#include <IRremote.h>

IRrecv irrecv(8);//클래스, 인스턴스변수
decode_results results;//decode_results 함수에 results변수를 만들어 IR센서의 수신값 저장(15번줄에서 받아옴)
int brightness = 0;
unsigned long lastValue=0;

void setup()
{
  irrecv.enableIRIn(); //적외선 수신부 활성화시킴
  pinMode(9, OUTPUT);
  Serial.begin(9600);
}

void loop()
{
  if(irrecv.decode(&results)){ //decode 메소스에서 적외선 신호를 받아 해석해서 results 변수에 값 넣음
    Serial.print(results.value,HEX);//results.valu: 적외선 신호를 16진수 값으로 저장
    Serial.print("\t");
    Serial.println(brightness);
    if(results.value == 0xFFFFFFFF)
      results.value=lastValue;   
    if(results.value ==0xFF30CF){//results.value에 '0'이 수신되면 (0xFD30CF은 10진수로 '0')
      brightness-=10;
      if(brightness<0)
        brightness=0;
      analogWrite(9,brightness);
    }
    
    else if(results.value == 0xFF18E7){
      brightness+=10;
      if(brightness>255)
        brightness=255;
      analogWrite(9,brightness);
    }
    lastValue = results.value; 
    delay(30);
    irrecv.resume();//다음 적외선 신호를 받기위함
  }
}
