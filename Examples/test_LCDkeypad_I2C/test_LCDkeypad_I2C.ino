#include <arduLCDkeypad.h>
#include <Streaming.h>
#include <Wire.h>
#include <PString.h>

#include <Time.h>

arduLCDkeypad myLCD = arduLCDkeypad(Wire,42);

void setup() 
{
  setTime(11,56,0,19,10,2017);
  Serial.begin(9600);
  Wire.begin();
  LCDkeypad_setup();
}

void loop() 
{
  
  char key = myLCD.keyPressed();
  if (myLCD.present())
  {
    if (key != NO_KEY)
    { 
      myLCD.updateValue(1,key);
      Serial << "key = [" << key << "]" << endl;
    }
    if (myLCD.recReset()) LCDkeypad_setup();
  }
  else
  {
    Serial << "." ;
  }
  delay(100);

}

void LCDkeypad_setup()
{
  //delay(1000);
  char key = myLCD.keyPressed();
  if (! myLCD.present())
  { 
    Serial << "LCD not present" << endl;
    return;
  }
  myLCD.clear();
  delay(20);
  myLCD.updateTime((unsigned long)(now()));
  //delay(5);
  myLCD.printAt("Good Morning",0,1);
  delay(20);
  myLCD.printAt("Key Pressed [ ]",0,2);
  delay(10);
  myLCD.updateLcdField(0,10,3,10,true,"%02.3f mA");
  myLCD.updateLcdField(2,0,0,19,true,"");
  myLCD.updateLcdFieldFormat(2,"YYYY-MM-dd HH:mm:ss");
  myLCD.setAutoTime(2);
  
  float current = 4.5432;
  myLCD.updateValue(0,current);
  delay(20);
  myLCD.updateLcdField(1,13,2,1,true,"%1d");
}
