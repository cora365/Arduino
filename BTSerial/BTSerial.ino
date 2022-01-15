#include <SoftwareSerial.h>

SoftwareSerial BTSerial(12,13);

void setup()
{
  Serial.begin(9600);
  BTSerial.begin(9600);
}

void loop()
{
  //블루투스 모듈의 입력값이 있을 때 시리얼 모니터에 출력
  if(BTSerial.available()){
    char ch = BTSerial.read();
    Serial.println(ch);
  }

  /*
    Serial.write(BTSerial.read());
 // 시리얼 모니터에 입력값이 있을 때 블루투스 모듈에 입력값 전달 
  if(Serial.available())
    BTSerial.write(Serial.read());
    */
}
