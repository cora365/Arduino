#include <LiquidCrystal.h>
LiquidCrystal lcd(12, 11, 5, 4, 3, 2);


void setup()
{
  lcd.begin(16,2);//lce 초기화
  lcd.print("hello, Arduino!!");
}

void loop()
{
  lcd.setCursor(0,1);
  lcd.print("123456789");
}