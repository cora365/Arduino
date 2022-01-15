#include <Wire.h>
#include <LiquidCrystal_I2C.h>
LiquidCrystal_I2C lcd(0x27, 16, 2);
void setup() {
  Serial.begin(9600);
  lcd.init();
  lcd.backlight();
  lcd.setCursor(0, 0);
  lcd.println("2022Bizcool Camp");
  lcd.setCursor(0, 1);
  lcd.println("Hello world!!   ");
}

void loop() {
  for (int i=0; i<300; i++){    
    lcd.scrollDisplayRight();
    delay(800);
  }
}
