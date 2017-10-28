#ifndef arduLCDkeypad_H
#define arduLCDkeypad_H

#if !defined(__time_t_defined) // avoid conflict with newlib or other posix libc
typedef unsigned long time_t;
#endif


#include <Streaming.h>
#include <Arduino.h>
#include <Wire.h>
#include <PString.h>

#define DELIM       '~'
#define NO_KEY       0
#define i2cNoKey    '!'


class arduLCDkeypad 
{
public:
  arduLCDkeypad(Stream &dev);
  arduLCDkeypad(Stream &dev,byte add);
  
  void printAt(const char message[],uint8_t col,uint8_t row);
  void printAt(const char message[],uint8_t col,uint8_t row,uint8_t blank);
  void clear();
  void setCursor(int col,int row);
  void Contrast(int percent);
  void BackLight(int percent_standby,int percent_active);
  void BackLightTimer(int backlightTimeout);
  void Buzz(int buzzTime);
  char keyPressed();
  bool recReset();
  bool present();
  void updateLcdField(uint8_t id,uint8_t col,uint8_t row,uint8_t len,bool visible,char strFormat[]);
  void updateLcdFieldFormat(uint8_t id, char strFormat[]);
  void updateStrContent(uint8_t id,char strContent[]);
  void updateTime(unsigned long _time);
  void setAutoTime(int8_t fieldNum);
  
/**
*    Prints functions
**/

  template <class ANY>
  void Prints(ANY data[])
  {
    PString pBuff(buff,sizeof(buff));
    pBuff << "P" << DELIM << data << endl;
    sendPrintCommand(buff);
  }
  
  template <class ANY>
  void Prints(ANY data)
  {
    PString pBuff(buff,sizeof(buff));
    pBuff << "P" << DELIM << data << endl;
    sendPrintCommand(buff);
  }
   
  //template <typename ANY>
  union varToBytes
  {
    uint8_t uint8_value;
    int8_t int8_value;
    unsigned int uint_value;
    int int_value;
    unsigned long ulong_value;
    long long_value;
    float float_value;
    byte bytes[4];
  };

/***
*      send an Update value to LCD
*
***/
   
   template <class ANY>
   void updateValue(uint8_t id, ANY data)
   {
     uint8_t itype = varType(data);
     varToBytes u;
     u.ulong_value = 0;
     switch (itype)
     {
       case 2:  u.uint8_value = data;  break;
       case 3:  u.int8_value = data;  break;
       case 4:  u.uint_value = data;  break;
       case 5:  u.int_value = data; break;
       case 6:  u.ulong_value = data; break;
       case 7:  u.long_value = data; break;
       case 8:  u.float_value = data; break;
     }
     char strValue[24];
     PString pvalue(strValue,sizeof(strValue));
     pvalue << "US" << DELIM << id << DELIM << itype << DELIM << data;
     if (itype > 1)
     {
       pvalue.begin();
       pvalue << "UV" << DELIM << id << DELIM << itype;
       for (int i=0;i<4;i++)
       {
         pvalue << DELIM << _HEX(u.bytes[i]) ;
       }
     }
     pvalue << endl;
     //Serial << "Update Value : " << pvalue <<  endl;
     sendPrintCommand(strValue);
     //Serial << "vartype = " << itype << endl;
   }
   
   
   
  int8_t varType(char var){return 0;}
  int8_t varType(char var[]){return 1;}
  int8_t varType(uint8_t var){return 2;}
  int8_t varType(int8_t var){return 3;}
  int8_t varType(unsigned int var){return 4;}
  int8_t varType(int var){return 5;}
  int8_t varType(unsigned long var){return 6;}
  int8_t varType(long var){return 7;}
  int8_t varType(float var){return 8;}

  

  
private:
  Stream *comms;
  byte address = 0;
  bool receivedReset = false;
  bool alive = false;
  char buff[48];
  char checkKeydata();
  char keyPressed_I2C();
  char keyPressed_Serial();
  void sendPrintCommand(char message[]);
  
  
};


#endif

//    http://arduiniana.org/libraries/pstring/
