#define BUFFER_LENGTH 48

#include <Time.h>
#include <TimeLib.h>


#include <StreamCommand.h>
#include <Streaming.h>
#include <SoftPWM.h>
#include <SoftPWM_timer.h>
#include <PString.h>

#include <Key.h>
#include <Keypad.h>

#include <LiquidCrystal.h>
#include <WSWire.h>
#include <neotimer.h>
#include <avr/pgmspace.h>
#include <stdio.h>
//#include <Time.h>

char messBuffer[8];
PString pMessage(messBuffer,sizeof(messBuffer));

bool hold = false;
int contrastPWM = 0;
int backLightPWM = 0;   // to be implemented

const byte I2CADDRESS = 42;

const int buzzer = 9;  // can use pwm
const int contrast = 3;
const int backLight = 21;
const int rs = 2,rw = 4,  en = 20, d4 = 5, d5 = 6, d6 = 7, d7 = 8;
LiquidCrystal lcd(rs,rw, en, d4, d5, d6, d7);

#define BUFFER_LENGTH 48

void setup() 
{
  init_charHeap();
  Serial.begin(9600);
  Wire.begin(I2CADDRESS);
  Wire.onReceive(I2CreceiveEvent);
  Wire.onRequest(I2CrequestEvent);
  setup_Commands();
  pMessage << F("reset") ;
  //pinMode(backLight,OUTPUT);
  pinMode(buzzer,OUTPUT);
  //pinMode(contrast,OUTPUT);
  contrastPWM = 5;//5;
  SoftPWMBegin();
  backLightPWM = 100;
  SoftPWMSet(backLight,backLightPWM);
  //analogWrite(contrast,contrastPWM);
  SoftPWMSet(contrast,contrastPWM);
  lcd.begin(20,4);
  //digitalWrite(backLight,HIGH);
  //Serial << F("mackLCDkeypad 20171013") << endl;;
  lcd << F("mackLCD 20171013");
  delay (1000);
  
}

void loop() 
{
  timerLoop();
}
