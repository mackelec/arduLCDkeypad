
#include "arduLCDkeypad.h"

arduLCDkeypad::arduLCDkeypad(Stream &dev)
{
  comms = &dev;
}

arduLCDkeypad::arduLCDkeypad(Stream &dev,byte add)
{
  comms = &dev;
  address = add;
}

void arduLCDkeypad::clear()
{
  PString pBuff(buff,sizeof(buff));
  pBuff << "Clr" << endl;
  sendPrintCommand(buff);
}

void arduLCDkeypad::setCursor(int col,int row)
{
  PString pBuff(buff,sizeof(buff));
  pBuff << "C" << DELIM << col << DELIM << row << endl;
  sendPrintCommand(buff);
}

void arduLCDkeypad::Contrast(int percent)
{
  PString pBuff(buff,sizeof(buff));
  pBuff << "Contrast" << DELIM << percent << endl;
  sendPrintCommand(buff);
}

void arduLCDkeypad::BackLight(int percent_standby,int percent_active)
{
  PString pBuff(buff,sizeof(buff));
  pBuff << "BackLight" << DELIM << percent_standby << DELIM << percent_active << endl;
  sendPrintCommand(buff);
}

void arduLCDkeypad::BackLightTimer(int backlightTimeout)
{
  PString pBuff(buff,sizeof(buff));
  pBuff << "BLt" << DELIM << backlightTimeout << endl;
  sendPrintCommand(buff);
}

void arduLCDkeypad::Buzz(int buzzTime)
{
  PString pBuff(buff,sizeof(buff));
  pBuff << "Bz" << DELIM << buzzTime << endl;
  sendPrintCommand(buff);
}

void arduLCDkeypad::printAt(const char message[],uint8_t col,uint8_t row,uint8_t blank)
{
  PString pBuff(buff,sizeof(buff));
  pBuff << "PA" << DELIM << message << DELIM << col << DELIM << row << DELIM << blank << endl;
  sendPrintCommand(buff);
  
}
void arduLCDkeypad::printAt(const char message[],uint8_t col,uint8_t row)
{
  printAt(message,col,row,0);
}
void arduLCDkeypad::updateLcdField(uint8_t id,uint8_t col,uint8_t row,uint8_t len,bool visible,char strFormat[])
{
  PString pBuff(buff,sizeof(buff));
  pBuff << "UF" << DELIM << id << DELIM << col << DELIM << row << DELIM << len << DELIM << visible <<   DELIM << strFormat << endl;
  sendPrintCommand(buff);
}
void arduLCDkeypad::updateLcdFieldFormat(uint8_t id, char strFormat[])
{
  PString pBuff(buff,sizeof(buff));
  pBuff << "Uf" << DELIM << id << DELIM << strFormat << endl;
  sendPrintCommand(buff);
}
void arduLCDkeypad::updateStrContent(uint8_t id,char strContent[])
{
  PString pBuff(buff,sizeof(buff));
  pBuff << "US" << DELIM << id << DELIM << strContent << endl; 
  sendPrintCommand(buff);
}

void arduLCDkeypad::updateTime(unsigned long _time)
{
  PString pBuff(buff,sizeof(buff));
  pBuff  << F("UT") << DELIM << _time << endl;
  sendPrintCommand(buff);
}

void arduLCDkeypad::setAutoTime(int8_t fieldNum)
{
  PString pBuff(buff,sizeof(buff));
  pBuff  << F("SAT") << DELIM << fieldNum << endl;
  sendPrintCommand(buff);
}

void arduLCDkeypad::sendPrintCommand(char message[])
{
  if (address > 0)
  {
    Wire.beginTransmission(address);
    Wire.write(message);
    Wire.endTransmission(); 
    //Serial << "write *mess : " << message ;  
  }
  else
  {
    comms->print(message);
  }
}

char arduLCDkeypad::keyPressed()
{
  char key = 0;
  if (address > 0)
  {
    key = keyPressed_I2C();
  }
  else
  {
    key = keyPressed_Serial();
  }
  return key;
}

bool arduLCDkeypad::recReset()
{
  if (receivedReset)
  {
    receivedReset = false;
    return true; 
  }
  return false;
}
  
bool arduLCDkeypad::present() {return alive;}

char arduLCDkeypad::keyPressed_I2C()
{
  Wire.requestFrom(42, 7);
  char key = checkKeydata();
  return key;
}

char arduLCDkeypad::checkKeydata()
{
  char buff[8];
  int ptr = 0;
  char retvalue = 0;
  
  alive = false;
  while (Wire.available())  
  {
    char c = Wire.read(); 
    if (c == '\n') 
    {
      buff[ptr]=0;
      int _reset = strncmp("reset",buff,5);
      if (_reset == 0) 
      {
        receivedReset = true;
        alive = true;
      }
      int cmp = strncmp("key=",buff,4);
      if (cmp==0 )
      {
        alive = true;
        if ( buff[4] != i2cNoKey)  
        {
          retvalue = buff[4]; 
        }
        return retvalue; 
      }
    }
    buff[ptr] = c;
    ptr++;
  }
  return retvalue;
}


char arduLCDkeypad::keyPressed_Serial()
{
  PString pBuff(buff,sizeof(buff));
  char retvalue = 0;
  while (comms->available()>0)
  {
    char c = comms->read();
    if (c == '\n') break;
    pBuff += c;  
  } 
  int cmp = strncmp("key=",buff,4);
  if (cmp==0 && buff[4] != i2cNoKey) 
  {
    retvalue = buff[4]; 
  }
  return retvalue; 
}

