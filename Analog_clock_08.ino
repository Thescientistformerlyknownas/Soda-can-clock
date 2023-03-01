
/***************************************************************************************************************************************************************************************
  Title:       Analog clock

  Version:     08
  Goal:        Use DS3231 as RTC an then calculated angles of analog clock to display on Nokia 5110
  Hardware:    Arduino Pro Mini 3.3V, 8MHz
  LCD:         Nokia 5110 using LCD5110_Graph library
  Sensors:     Only a real time clock (DS3231) module (RTC) is used
  Software:    IDE Arduino 1.8.4, Libraries see below
  Software:    Setup Screen with version
  Aktuatoren:  none
  Energy:      Lipo 18650 battery
  Storage:     No data storage
  Change:      Compared to version 07 the LowPower function was included to safe energy
  Remarks:     LEDs are removed from Arduino Pro Mini and the DS3231 to safe energy
  Done by:     The scientist formerly known as Naegeli ... please subscribe to my YouTube channel www.youtube.com/@LeFreak
**************************************************************************************************************************************************************************************/

#include <LCD5110_Graph.h>      //Library for Nokia 5110 display
#include <RTClib.h>             //Library for time
#include <Wire.h>               //Library for i2c, serial communication
#include <LowPower.h>           //Library to safe energy

LCD5110 myGLCD(13,11,9,8,10);   //13=CLK; 11=DIN; DC=9; CE=10; RST=8;

RTC_DS3231 rtc;

int clockCenterX = 42;
int clockCenterY = 24;

extern uint8_t SmallFont[];

void setup()
{
  Serial.begin(57600);
  Wire.begin(); 
  rtc.begin();
  myGLCD.InitLCD();
  myGLCD.setContrast(65);
  myGLCD.setFont(SmallFont);
  myGLCD.print("Analog clock", CENTER, 8);
  myGLCD.print("DS3231", CENTER, 17);
  myGLCD.print("Version 08", CENTER, 26);
  myGLCD.update();
  delay(5000);
  myGLCD.clrScr();
}

void loop()
{
  myGLCD.clrScr();
  DateTime now = rtc.now();
  drawMinutes();
  drawHours();
  drawDisplay();
  myGLCD.update();      
  delay(800);
  sleepForSomeTime();
}

 void drawMinutes()
{
  float x1, y1, x2, y2, x3, y3, x4, y4, Am;
  DateTime now = rtc.now();                 
  Am = (now.minute() * 6);
  
  x1=25*sin(Am*0.0175);
  y1=25*-cos(Am*0.0175);
  x2=3*sin(Am*0.0175);
  y2=3*-cos(Am*0.0175);
  x3=10*sin((Am+8)*0.0175);
  y3=10*-cos((Am+8)*0.0175);
  x4=10*sin((Am-8)*0.0175);
  y4=10*-cos((Am-8)*0.0175);
  
  myGLCD.drawLine(x1+clockCenterX, y1+clockCenterY, x3+clockCenterX, y3+clockCenterY);
  myGLCD.drawLine(x3+clockCenterX, y3+clockCenterY, x2+clockCenterX, y2+clockCenterY);
  myGLCD.drawLine(x2+clockCenterX, y2+clockCenterY, x4+clockCenterX, y4+clockCenterY);
  myGLCD.drawLine(x4+clockCenterX, y4+clockCenterY, x1+clockCenterX, y1+clockCenterY);
  myGLCD.drawCircle(clockCenterX, clockCenterY,2);
}

void drawHours()
{
  float x1, y1, x2, y2, x3, y3, x4, y4, Ah;             
  DateTime now = rtc.now();
  Ah = (now.hour() * 30) + (now.minute() * 0.5);
  
  x1=20*sin(Ah*0.0175);
  y1=20*-cos(Ah*0.0175);
  x2=3*sin(Ah*0.0175);
  y2=3*-cos(Ah*0.0175);
  x3=8*sin((Ah+14)*0.0175);
  y3=8*-cos((Ah+14)*0.0175);
  x4=8*sin((Ah-14)*0.0175);
  y4=8*-cos((Ah-14)*0.0175);
  
  myGLCD.drawLine(x1+clockCenterX, y1+clockCenterY, x3+clockCenterX, y3+clockCenterY);
  myGLCD.drawLine(x3+clockCenterX, y3+clockCenterY, x2+clockCenterX, y2+clockCenterY);
  myGLCD.drawLine(x2+clockCenterX, y2+clockCenterY, x4+clockCenterX, y4+clockCenterY);
  myGLCD.drawLine(x4+clockCenterX, y4+clockCenterY, x1+clockCenterX, y1+clockCenterY);
  }

 void drawDisplay()
{  
  for (int i=0; i<12; i++)    // Draw a small mark for every hour
  {
  float x1, y1, x2, y2, Ah;
  Ah = i * 30; 
  x1=30*sin(Ah*0.0175);
  y1=30*-cos(Ah*0.0175);
  x2=26*sin(Ah*0.0175);
  y2=26*-cos(Ah*0.0175);
  myGLCD.drawLine(x1+clockCenterX, y1+clockCenterY, x2+clockCenterX, y2+clockCenterY);
  myGLCD.setContrast(65);
  myGLCD.setFont(SmallFont);
  myGLCD.print("2",73,3);
  myGLCD.print("3",78,20);
  myGLCD.print("4",73,37);  
  myGLCD.print("8",7,37);
  myGLCD.print("9",0,20);
  myGLCD.print("10",2,3);
  }
 }
 
void sleepForSomeTime()
  {
  for (int i = 0; i < 6; i++)
    LowPower.powerDown(SLEEP_8S, ADC_OFF, BOD_OFF); // after 7 intervals (0-6) of 8 seconds the Arduino wakes up to make a time check. Stopping the time between intervals revealed 52 seconds (not 56)
  }
