#include <SoftwareSerial.h>
#include<LiquidCrystal.h>
#define LM35 A0
#define gaspin A1
LiquidCrystal lcd(12, 11, 5, 4, 3, 2);
SoftwareSerial SIM900(7, 8);


float temperature, Temp_alert_val, Temp_shut_val,Smoke_alert_val,Smoke_shut_val;
int  Fire_Set=0;
int buzzer = 13;
String temp,temp1;
int analogValue,smoke;


void setup()
{
  lcd.begin(16, 2);
  pinMode(LM35, INPUT);
  pinMode(gaspin, INPUT);
  SIM900.begin(9600);
  Serial.begin(9600);
  SIM900.print("AT");
  delay(1000);
}

void loop()
{
  CheckFire();
  CheckShutDown();
}

void CheckFire()
{
  lcd.setCursor(0,0);
  lcd.print("Fire Scan - ON   ");
  Temp_alert_val = CheckTemp();
  Smoke_alert_val=CheckSmoke();
  if (Temp_alert_val > 45 || Smoke_alert_val > 300)

  {
    if (Fire_Set == 0)
    {
    Fire_Set = 1;
    lcd.setCursor(0, 1);
    lcd.print("Fire Detetcted");
    tone(buzzer, 1000, 3000);
    SendTextMessage();
    delay(3000);
    }
  }
}


float CheckSmoke()
{
  smoke=analogRead(gaspin);
  return smoke;
}
float CheckTemp()
{
  analogValue = analogRead(LM35);
  temperature = float(analogValue) / 1023.00;
  temperature = temperature * 500.00;
  delay(500);
  return temperature;
}



void CheckShutDown()
{
  if (Fire_Set == 1)
  {

    Temp_shut_val = CheckTemp();
    Smoke_shut_val=CheckSmoke();
    if (Temp_shut_val < 35 && Smoke_shut_val < 150 )
    {
  
      lcd.setCursor(0, 1);
      lcd.print("SAFE NOW :)    ");
      Fire_Set = 0;
    }
  }
}

void SendTextMessage()
{

  temp = CheckTemp();
  temp1=CheckSmoke();
  SIM900.println("AT+IPR=9600");
  delay(1000);

  SIM900.println("AT+CMGF=1");
  delay(1000);


  SIM900.println("AT+CMGS =\"+923235000336\"");
  delay(1000);

  SIM900.print("Fire Detected in room ! , T : " + temp + "& Smoke level : "+temp1);
  delay(500);

  SIM900.println((char)26);
  delay(2000);

  SIM900.println("AT+IPR=9600");
  delay(1000);

  SIM900.println("AT+CMGF=1");
  delay(1000);

  SIM900.println("AT+CMGS =\"+923348112580\"");
  delay(1000);

  SIM900.print("Fire Detected in room ! , T : " + temp);
  delay(1000);

  SIM900.println((char)26);
  delay(1000);

 
}
