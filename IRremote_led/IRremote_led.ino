#include <IRremote.h>

IRrecv irrecv(8);//클래스, 인스턴스변수
decode_results results;//decode_results 함수에 results변수를 만들어 IR센서의 수신값 저장(15번줄에서 받아옴)
  
void setup()
{
  irrecv.enableIRIn(); //적외선 수신부 활성화시킴
  pinMode(9, OUTPUT);
  Serial.begin(9600);
}

void loop()
{
  if(irrecv.decode(&results)){ //decode 메소스에서 적외선 신호를 받아 해석해서 results 변수에 값 넣음
    Serial.println(results.value,HEX);//results.valu: 적외선 신호를 16진수 값으로 저장
    if(results.value ==0xFD30CF){//results.value에 '0'이 수신되면 (0xFD30CF은 10진수로 '0')
      digitalWrite(9,HIGH);
    }
    else{
      digitalWrite(9,LOW);
    }
    delay(30);
    irrecv.resume();//다음 적외선 신호를 받기위함
  }
}
