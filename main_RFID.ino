#include <SPI.h>                 // RFID를 위한 SPI
#include <MFRC522.h>
#define RST_PIN         9   // Rfid 핀번호
#define SS_PIN         10
MFRC522 rfid(SS_PIN, RST_PIN);// RFID
byte nuidPICC[4]={0x67, 0x29, 0xDB, 0xA6}; // 등록된 Rfid카드 NUID 0x53, 0x3B, 0x19, 0x3E

void setup(){
  Serial.begin(9600);
  SPI.begin(); // Init SPI bus
  rfid.PCD_Init(); 
}
 
void loop(){
  getRfid();
}

 
void getRfid(){
  // 카드를 반복해서 계속 읽지 안도록 빠져나감
  if ( ! rfid.PICC_IsNewCardPresent())    return;

  // ID가 인식되지 않았다면 더이상 진행하지 말고 빠져나감
  if ( ! rfid.PICC_ReadCardSerial())    return;

  // 카드 타입 읽기
  MFRC522::PICC_Type piccType = rfid.PICC_GetType(rfid.uid.sak);

  // MIFARE 타입이 아니라면 더이상 진행하지 말고 빠져나감
  if (piccType != MFRC522::PICC_TYPE_MIFARE_MINI &&  
    piccType != MFRC522::PICC_TYPE_MIFARE_1K &&
    piccType != MFRC522::PICC_TYPE_MIFARE_4K) {
    Serial.println(F("Your tag is not of type MIFARE Classic."));
    return;
  }

  // 읽은 ID와 등록된 ID값을 비교
  if (rfid.uid.uidByte[0] == nuidPICC[0] && 
    rfid.uid.uidByte[1] == nuidPICC[1] && 
    rfid.uid.uidByte[2] == nuidPICC[2] && 
    rfid.uid.uidByte[3] == nuidPICC[3] ) {
    Serial.println(F("Good"));
    //doorMode = 1; // 문열기
    }
  else {
    //등록된 ID가 아니라면 tag 정보 표시
    Serial.print(F("Unconfirmed Card: "));
    printHex(rfid.uid.uidByte, rfid.uid.size); // 사용자 화면에 부정 tag 정보 출력
    Serial.println();
    //doorMode = 0; // 문닫기
  }

  // rfid 종료
  rfid.PICC_HaltA();  // PICC 종료
  rfid.PCD_StopCrypto1();  // 암호화 종료
}


// 카드 tag 출력
void printHex(byte *buffer, byte bufferSize) {
  for (byte i = 0; i < bufferSize; i++) {
    Serial.print(buffer[i] < 0x10 ? " 0" : " ");
    Serial.print(buffer[i], HEX);
  }}
