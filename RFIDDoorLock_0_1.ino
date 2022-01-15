//====================================================================================//
// RFID를 이용한 도어락
//
// RFID 핀번호
// SDA  : 10
// SCK  : 13
// MOSI : 11
// MISO : 12
// IRQ  : NC
// GND  : GND
// RST  : 9
// 3.3V : 3.3V
//====================================================================================//

int piezoPin        = 6;

//====================================================================================//
// RFID 암호기억을 위한 EEPROM
//====================================================================================//
#include <EEPROM.h>



//====================================================================================//
// RFID
//====================================================================================//
#include <MFRC522.h>                // RFID 라이브러
#include <SPI.h>                    // RFID를 위한 SPI 라이브러

#define SS_PIN 10                   // RFID SS(SDA:ChipSelect) PIN
#define RST_PIN 9                   // RFID Reset PIN

MFRC522 rfid(SS_PIN, RST_PIN);      // RFID 객체

//====================================================================================//
// 서보모터
//====================================================================================//
#include <Servo.h>                  // 서보 라이브러리

#define SERVO_PIN       5           // 서보 PIN
#define SERVO_MIN       10          // 서보 최소 각도
#define SERVO_MAX       170         // 서보 최대 각도

Servo   myservo;                    //  서보모터 객체


/* 등록된 RF CARD ID */
#define PICC_0 0x63
#define PICC_1 0xC6
#define PICC_2 0x40
#define PICC_3 0x16

byte    PICC[4] = {0xFF, 0xFF, 0xFF, 0xFF};

//====================================================================================//
// LCD_TYPE
// 0 : 16x2 LCD Normal
// 1 : 20x4 LCD Normal
// 2 : 16x2 LCD I2C type
// 3 : 20x4 LCD I2C type
//====================================================================================//
#define LCD_1602_Normal     1
#define LCD_2004_Normal     2
#define LCD_1602_I2C        10
#define LCD_2004_I2C        20

#define LCD_TYPE            LCD_1602_I2C
//------------------------------------------------------------------------------------//
// I2C LCD를 제어하기 위한 라이브러리를 추가
//------------------------------------------------------------------------------------//
#if (LCD_TYPE == LCD_1602_I2C) || (LCD_TYPE == LCD_2004_I2C)
    #include <Wire.h>
    #include <LiquidCrystal_I2C.h>

    #define LCD_I2C_Address 0x23
    #define Num_Of_Col      16
    #define Num_Of_Row      2
#else
    #include <LiquidCrystal.h>

    #define LCD_RS          12
    #define LCD_EN          11
    #define LCD_D4          5
    #define LCD_D5          4
    #define LCD_D6          3
    #define LCD_D7          2
#endif
 
//------------------------------------------------------------------------------------//
// LCD 객체를 생성합니다.
// LCD I2C Address : 0x23
//------------------------------------------------------------------------------------//
#if (LCD_TYPE == LCD_1602_I2C) || (LCD_TYPE == LCD_2004_I2C)
    LiquidCrystal_I2C   lcd(LCD_I2C_Address, Num_Of_Col, Num_Of_Row);
#else
    LiquidCrystal       lcd(LCD_RS, LCD_EN, LCD_D4, LCD_D5, LCD_D6, LCD_D7);
#endif

//====================================================================================//
// Tact Switch 포트
//====================================================================================//
#define Tact_SW             2


int eepromWriteEnable       = 0;




//====================================================================================//
// SETUP() 함수
//====================================================================================//
void setup()
{
    int     cmpCount = 0;

    pinMode(piezoPin, OUTPUT);
    
   //--------------------------------------------------------------------------------//
    // 시리얼 모니터 시작
    //--------------------------------------------------------------------------------//
    Serial.begin(9600);

    //--------------------------------------------------------------------------------//
    // RFID 시작
    //--------------------------------------------------------------------------------//
    SPI.begin();                // SPI 시작
    rfid.PCD_Init();            // RF 모듈 시작

    //--------------------------------------------------------------------------------//
    // 서보모터 시작
    //--------------------------------------------------------------------------------//
    myservo.attach(SERVO_PIN); //서보 시작
    myservo.write(SERVO_MIN); //초기 서보 모터를 0도로 위치 시킴

    //--------------------------------------------------------------------------------//
    // LCD의 백라이트를 켜줍니다.
    //--------------------------------------------------------------------------------//
    lcd.init();
    lcd.backlight();

    String lcdLine1 = "dgsw.enoz.kr";
    String lcdLine2 = "Hackertone Day";
    
    lcd.setCursor(2, 0);
    lcd.print(lcdLine1);
    lcd.setCursor(1, 1);
    lcd.print(lcdLine2);
    delay(3000);

    //--------------------------------------------------------------------------------//
    // EEPROM 초기화
    //--------------------------------------------------------------------------------//
    for (int i = 0; i < 4; i++)
    {
        byte    loByteInit  = EEPROM.read(i*2);
        byte    hiByteInit  = EEPROM.read(i*2+1);

        int     eepromValue = 0;
        
        eepromValue = word(hiByteInit, loByteInit);
        if (eepromValue == 0xFFFF)
        {
            cmpCount++;
        }
    }

    if (cmpCount >= 4)
    {
        RfidReadAndEEPROMSave();
        cmpCount = 0;
    }

    delay(500);
    
    EepromReadAndBufferSave();
    
    //--------------------------------------------------------------------------------//
    // Tact Switch 눌림 인터럽트
    //--------------------------------------------------------------------------------//
    pinMode(Tact_SW, INPUT);
    attachInterrupt(digitalPinToInterrupt(Tact_SW), TactSwitchService, FALLING);

    digitalWrite(piezoPin, HIGH);
    delay(500);
    digitalWrite(piezoPin, LOW);
    
}
 
//====================================================================================//
// LOOP() 함수
//====================================================================================//
void loop()
{
    RfidReadAndEEPROMSave();

    //--------------------------------------------------------------------------------//
    //  카드가 인식 안되었다면 더이상 진행하지 말고 빠져나감
    //--------------------------------------------------------------------------------//
    if ( ! rfid.PICC_IsNewCardPresent())    { return; }

    //--------------------------------------------------------------------------------//
    // RF카드의 ID가 인식 안되었다면 더이상 진행하지 말고 빠져나감
    //--------------------------------------------------------------------------------//
    if ( ! rfid.PICC_ReadCardSerial())      { return; }


    //--------------------------------------------------------------------------------//
    // PICC 타입 읽고 시리얼 프린트에 출력
    //--------------------------------------------------------------------------------//
    MFRC522::PICC_Type piccType = rfid.PICC_GetType(rfid.uid.sak);
    Serial.print(F("PICC type: "));
    Serial.println(rfid.PICC_GetTypeName(piccType));


    //--------------------------------------------------------------------------------//
    // MIFARE 방식이 아니라면 더이상 진행하지 말고 빠져나감
    //--------------------------------------------------------------------------------//
    if (piccType != MFRC522::PICC_TYPE_MIFARE_MINI &&  
        piccType != MFRC522::PICC_TYPE_MIFARE_1K &&
        piccType != MFRC522::PICC_TYPE_MIFARE_4K)
    {
        Serial.println(F("Your tag is not of type MIFARE Classic."));
        return;
    }


    digitalWrite(piezoPin, HIGH);
    delay(500);
    digitalWrite(piezoPin, LOW);

    //--------------------------------------------------------------------------------//
    // ID가 등록된 ID와 동일하다면
    //--------------------------------------------------------------------------------//
    if (rfid.uid.uidByte[0] == PICC[0] || 
        rfid.uid.uidByte[1] == PICC[1] || 
        rfid.uid.uidByte[2] == PICC[2] || 
        rfid.uid.uidByte[3] == PICC[3])
    {
        Serial.println(F("This is a confirmed Card."));
        Serial.print(F("In hex: "));
        printHex(rfid.uid.uidByte, rfid.uid.size);
        Serial.println(" ");
        Serial.println(F("Motor On!!"));   

        //----------------------------------------------------------------------------//
        // 서보 MAX 도로 이동 후 일정시간 유지
        //----------------------------------------------------------------------------//
        myservo.write(SERVO_MAX);
        delay(3000);
        myservo.write(SERVO_MIN);
    }
    else
    {
        digitalWrite(piezoPin, HIGH);
        delay(150);
        digitalWrite(piezoPin, LOW);
        delay(150);

        digitalWrite(piezoPin, HIGH);
        delay(150);
        digitalWrite(piezoPin, LOW);
        delay(150);
        
        digitalWrite(piezoPin, HIGH);
        delay(150);
        digitalWrite(piezoPin, LOW);
        delay(150);

        //----------------------------------------------------------------------------//
        // 등록된 카드가 아니라면 시리얼 모니터로 ID 표시
        //----------------------------------------------------------------------------//
        Serial.println(F("This is an unconfirmed Card."));
        Serial.print(F("In hex: "));
        printHex(rfid.uid.uidByte, rfid.uid.size);    
        Serial.println(" ");
    }

    //--------------------------------------------------------------------------------//
    // 종료
    //--------------------------------------------------------------------------------//
    rfid.PICC_HaltA();
    rfid.PCD_StopCrypto1();

    //--------------------------------------------------------------------------------//
    // 다시 시작!
    //--------------------------------------------------------------------------------//
}

//====================================================================================//
// 카드 UID를 16진수 표시
//====================================================================================//
void printHex(byte *buffer, byte bufferSize)
{
    for (byte i = 0; i < bufferSize; i++)
    {
        Serial.print(buffer[i] < 0x10 ? " 0" : " ");
        Serial.print(buffer[i], HEX);
    }
}

//====================================================================================//
// Tact Switch 인터럽트 서비스
//====================================================================================//
void TactSwitchService()
{
    eepromWriteEnable = 1;
}

//====================================================================================//
// RFID를 읽어서 EEPROM에 저장
//====================================================================================//
void RfidReadAndEEPROMSave()
{
    //--------------------------------------------------------------------------------//
    // EEPROM Write Enable
    //--------------------------------------------------------------------------------//
    if (eepromWriteEnable == 1)
    {
        int returnEnable = 0;
        
        lcd.init();

        String lcdLine1 = "Write EEPROM Mod";
        String lcdLine2 = "Attach RFID Card";
        
        lcd.setCursor(0, 0);
        lcd.print(lcdLine1);
        lcd.setCursor(0, 1);
        lcd.print(lcdLine2);
        
        //----------------------------------------------------------------------------//
        //  카드가 인식 안되었다면 더이상 진행하지 말고 빠져나감
        //----------------------------------------------------------------------------//
        if ( ! rfid.PICC_IsNewCardPresent())    { returnEnable = 1; }
    
        //----------------------------------------------------------------------------//
        // RF카드의 ID가 인식 안되었다면 더이상 진행하지 말고 빠져나감
        //----------------------------------------------------------------------------//
        if ( ! rfid.PICC_ReadCardSerial())      { returnEnable = 1; }

        //--------------------------------------------------------------------------------//
        // PICC 타입 읽기
        //--------------------------------------------------------------------------------//
        MFRC522::PICC_Type piccType = rfid.PICC_GetType(rfid.uid.sak);
        
        if (piccType != MFRC522::PICC_TYPE_MIFARE_MINI &&  
            piccType != MFRC522::PICC_TYPE_MIFARE_1K &&
            piccType != MFRC522::PICC_TYPE_MIFARE_4K)
        {
            Serial.print(F("PICC type: "));
            Serial.println(rfid.PICC_GetTypeName(piccType));
            Serial.println(F("Your tag is not of type MIFARE Classic."));
            returnEnable = 1;
        }

        if (returnEnable == 1)  return;

        digitalWrite(piezoPin, HIGH);
        delay(500);
        digitalWrite(piezoPin, LOW);
        //--------------------------------------------------------------------------------//
        // EEPROM Write
        //--------------------------------------------------------------------------------//
        for (int i = 0; i < 4; i++)
        {
            byte hiByte = highByte(0xFF);
            byte loByte = lowByte(rfid.uid.uidByte[i]);

            EEPROM.write(i*2,   loByte);
            EEPROM.write(i*2+1, hiByte);
            
            delay(1000);
           
            String lcdLine1 = "                ";
            String lcdLine2 = "                ";

            lcd.setCursor(0, 0);
            lcd.print(lcdLine1);
            lcd.setCursor(0, 1);
            lcd.print(lcdLine2);

            lcdLine1 = "dgsw.enoz.kr";
            lcdLine2 = "Hackertone Day";
            
            lcd.setCursor(2, 0);
            lcd.print(lcdLine1);
            lcd.setCursor(1, 1);
            lcd.print(lcdLine2);

            delay(500);

            EepromReadAndBufferSave();

            eepromWriteEnable = 0;
        }
        digitalWrite(piezoPin, HIGH);
        delay(300);
        digitalWrite(piezoPin, LOW);
        delay(300);

        digitalWrite(piezoPin, HIGH);
        delay(300);
        digitalWrite(piezoPin, LOW);
        delay(300);
        
        digitalWrite(piezoPin, HIGH);
        delay(300);
        digitalWrite(piezoPin, LOW);
        delay(300);
    }
}

//====================================================================================//
// EEPROM을 읽어서 버퍼에 저장
//====================================================================================//
void EepromReadAndBufferSave()
{
    for (int i = 0; i < 4; i++)
    {
        byte    loByte  = EEPROM.read(i*2);
        byte    hiByte  = EEPROM.read(i*2+1);

        PICC[i] = loByte;
    }
}
