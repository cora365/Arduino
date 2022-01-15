#include <LiquidCrystal_I2C.h>

#include <Wire.h>
int  Value = 0;               
LiquidCrystal_I2C lcd(0x27, 16, 2); // 0x27 또는 0x3F 둘다 안될경우에는 확인해봐야함
#define Sensing_pin A0

void setup()
{
  Serial.begin(9600);
  Value = analogRead(Sensing_pin);
  lcd.init();
  lcd.backlight();
  Serial.println("LM35 + I2C LCD");
  Serial.println("Hello world~!!");
  lcd.setCursor(0, 0);
  lcd.println("2021Bizcool Camp");
  lcd.setCursor(0, 1);
  lcd.println("Hello world!!   ");
  delay(2000);
}

void loop()
{
  
  int Value = analogRead(Sensing_pin);
  Value = map(Value,0,1023,0,1100);
  //float voltage = Value * 5.0 / 1023;
  float temperature =  Value * 5.0 / 10;
  
  lcd.clear();
  lcd.init();
  lcd.backlight();
  lcd.setCursor(0, 0);
  lcd.println("LM35 + I2C LCD");
  lcd.setCursor(0, 1);
  lcd.print("Temperature : ");
  lcd.print(temperature);
  lcd.print("%");
  delay(1000);
}
