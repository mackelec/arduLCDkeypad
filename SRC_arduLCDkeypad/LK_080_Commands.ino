
//#include "GString.h"
//#include <fauxStreamString.h>
#include <cppQueue.h>

Queue q(sizeof(char),200,FIFO);
//char buff[21]; //i2cBuff[40];
//fauxStreamString fStreamString(i2cBuff);

StreamCommand cmdSerial;
StreamCommand cmdI2C;
//char gbuff[40];
char delim[2] = {'~',0};


//PString pbuff(buff,sizeof(buff));
//PString pI2Cbuff(i2cBuff,sizeof(i2cBuff));

void setup_Commands()
{
  cmdSerial.setStream(Serial);
  cmdI2C.setQueue(q);
  
  cmdSerial.setDelim(delim);
  cmdSerial.addCommand("P",commandSPrint);
  cmdSerial.addCommand("PA",commandSPrintAt);
  cmdSerial.addCommand("C",commandSCursor);
  cmdSerial.addCommand("Clr",commandClear);
  cmdSerial.addCommand("Contrast",commandSContrast);
  cmdSerial.addCommand("BackLight",commandSBackLight);
  cmdSerial.addCommand("BLt",commandSBackLightTimeout);

  cmdI2C.setDelim(delim);  
  cmdI2C.addCommand("P",commandIPrint);
  cmdI2C.addCommand("PA",commandIPrintAt);
  cmdI2C.addCommand("C",commandICursor);
  cmdI2C.addCommand("Clr",commandClear);
  cmdI2C.addCommand("Contrast",commandIContrast);
  cmdI2C.addCommand("BackLight",commandIBackLight);
  cmdI2C.addCommand("BLt",commandIBackLightTimeout);
  cmdI2C.addCommand("Bz",commandIBuzzer);

  cmdI2C.addCommand("UF",commandIUpdateLcdField);
  cmdI2C.addCommand("Uf",commandIUpdateLcdFieldFormat);
  cmdI2C.addCommand("US",commandIUpdateString);
  cmdI2C.addCommand("UV",commandIUpdateValue);          //  update value

  cmdI2C.addCommand("UT",commandIUpdateTime); 
  cmdI2C.addCommand("SAT",commandISetAutoTime); 

  cmdI2C.setDefaultHandler(unrecognized_I2C);
}

void readStreams()
{
  if (hold) return;
  cmdSerial.readStream();
  cmdI2C.readStream();
}

void I2CreceiveEvent(int howMany)
{
  while (Wire.available())  
  {
    char c = Wire.read();
    q.push(& c); 
  }
}


void commandSPrint()
{
  char *arg = cmdSerial.next();
  if (arg != NULL) commandPrint (arg);
}
void commandIPrint()
{
  char *arg = cmdI2C.next();
  if (arg != NULL) commandPrint (arg);
  //Serial << "Iprint :" <<  arg << endl;
}
void commandSPrintAt()
{
  char *arg = cmdSerial.next();
  char *argCol = cmdSerial.next();
  char *argRow = cmdSerial.next();
  char *argblank = cmdSerial.next();
  int col=0,row=0,blank=0;
  if (argCol != NULL) col = atoi(argCol);
  if (argRow != NULL) row = atoi(argRow);
  if (argblank != NULL) blank = atoi(argblank);
  printBlank(col,row,blank); 
  if (arg != NULL) commandPrint (arg);
  //Serial << "Iprint AT :" <<  arg << "," << col << "," << row << "," << blank << endl;
}
void commandIPrintAt()
{
  char *arg = cmdI2C.next();
  char *argCol = cmdI2C.next();
  char *argRow = cmdI2C.next();
  char *argblank = cmdI2C.next();
  int col=0,row=0,blank=0;
  if (argCol != NULL) col = atoi(argCol);
  if (argRow != NULL) row = atoi(argRow);
  if (argblank != NULL) blank = atoi(argblank);
  printBlank(col,row,blank); 
  if (arg != NULL) commandPrint (arg);
  //Serial << "Iprint AT :" <<  arg << "," << col << "," << row << "," << blank << endl;
}
void commandPrint (char* mess)
{
  //PString pbuff(buff,sizeof(buff));
  char buff[21];
  PString pbuff(buff,sizeof(buff));
  pbuff.begin();
  pbuff.print(mess);
  lcd << pbuff;
  //Serial << "commandPrint : " << buff << endl;
}

void commandSCursor()
{
  char *argCol = cmdSerial.next();
  char *argRow = cmdSerial.next();
  int col=0,row=0;
  if (argCol != NULL) col = atoi(argCol);
  if (argRow != NULL) row = atoi(argRow);
  //Serial << "Cursor = " << col << "," << row << endl;
  lcd.setCursor(col,row);
  //commandCursor(col,row);
}

void printBlank(int col,int row, int blank)
{
  char buff[21];
  PString pbuff(buff,sizeof(buff));
  lcd.setCursor(col,row);
  if (blank == 0) return;
  //pbuff.begin();
  pbuff << F( "                    ");
  if (blank > 20) blank = 20;
  buff[blank] = 0;
  lcd.print(buff); 
  lcd.setCursor(col,row);
}

void commandICursor()
{
  char *argCol = cmdI2C.next();
  char *argRow = cmdI2C.next();
  int col=0,row=0;
  if (argCol != NULL) col = atoi(argCol);
  if (argRow != NULL) row = atoi(argRow);
  //Serial << "Cursor = " << col << "," << row << endl;
  lcd.setCursor(col,row);
}

void commandCursor(int col, int row)
{
  lcd.setCursor(col,row);
}

void commandClear()
{
  lcd.clear();
  lcd.setCursor(0,0);
  for (uint8_t i = 0; i<sizeof(lcdFields);i++)
  {
    lcdFields[i].Clear(1);
  }
}

void commandSContrast()
{
  char *arg = cmdSerial.next();
  if (arg != NULL) contrastPWM = atoi(arg);
  //analogWrite(contrast,contrastPWM);
  SoftPWMSet(contrast,contrastPWM);
  //Serial << "Contrast = " << contrastPWM << "arg = " << arg << endl;
}

void commandIContrast()
{
  char *arg = cmdI2C.next();
  if (arg != NULL) contrastPWM = atoi(arg);
  //analogWrite(contrast,contrastPWM);
  SoftPWMSet(contrast,contrastPWM);
  //Serial << "Contrast = " << contrastPWM << "arg = " << arg << endl;
}

void commandSBackLight()
{
  char *argStandby = cmdSerial.next();
  char *argActive = cmdSerial.next();
  if (argStandby != NULL) BackLight_standby = atoi(argStandby);
  if (argActive != NULL) BackLight_active = atoi(argActive);
  //SoftPWMSet(backLight,backLightPWM);
}

void commandIBackLight()
{
  char *argStandby = cmdI2C.next();
  char *argActive = cmdI2C.next();
  if (argStandby != NULL) BackLight_standby = atoi(argStandby);
  if (argActive != NULL) BackLight_active = atoi(argActive);
  //Serial << "Backlight :" << BackLight_standby << "," << BackLight_active << endl;
}

void commandSBackLightTimeout()
{
  char *argTimeout = cmdSerial.next();
  if (argTimeout != NULL) seconds_BacklightTmr = atoi(argTimeout);
}
void commandIBackLightTimeout()
{
  char *argTimeout = cmdI2C.next();
  if (argTimeout != NULL) seconds_BacklightTmr = atoi(argTimeout);
}

void commandSBuzzer()
{
  char *arg = cmdSerial.next();
  long t=50;
  if (arg != NULL) t = atol(arg);
  digitalWrite(buzzer,HIGH);
  buttonClickTimer.set(t);
  buttonClickTimer.start();
}

void commandIBuzzer()
{
  char *arg = cmdI2C.next();
  long t=50;
  if (arg != NULL) t = atol(arg);
  digitalWrite(buzzer,HIGH);
  buttonClickTimer.set(t);
  buttonClickTimer.start();
}

void  unrecognized_I2C(const char *command)
{
  Serial << F("unrecognised I2C :") << command << endl;
}

void commandIUpdateValue()
{
  char *argId = cmdI2C.next();
  char *argVarType = cmdI2C.next();
  uint8_t id=0,varType=0;
  if (argId != NULL) id = atoi(argId);
  if (argVarType != NULL) varType = atoi(argVarType);
  //varToBytes u;
  bool changed = false;
  for (int i=0;i<4;i++)
  {
    char *arg = cmdI2C.next();
    //--  read in ascii hex to int
    int v = strtol(arg,NULL,16);
    if (v != lcdFields[id].u.bytes[i])
    {
      lcdFields[id].u.bytes[i] = v;
      changed = true;
    }
    //--- if nothing changed , dont waste time
    //if (lcdFields[id].isCleared() == 1 ) changed = true;
    //if (!changed ) return;
  }
  //unsigned long thisVar = atol
  lcdFields[id].varType = varType ;
  //Serial << F("UV long = ") << lcdFields[id].u.long_value << endl;
  lcdFields[id].genString();
}

void commandIUpdateString()
{
  //Serial << "UpdateString" << endl;
  char *argId = cmdI2C.next();
  char *argVarType = cmdI2C.next();
  char *argString = cmdI2C.next();
  uint8_t id=0;
  if (argId != NULL) id = atoi(argId);
  if (id >= numLcdFields) return;
  if (argVarType != NULL) lcdFields[id].varType = atoi(argVarType);
  if (argString != NULL) 
  {
    printBlank(lcdFields[id].col,lcdFields[id].row,lcdFields[id].len);
    commandPrint(argString);
  }
  
}

void commandIUpdateLcdFieldFormat()
{
  char *argId = cmdI2C.next();
  char *argFormat = cmdI2C.next();
  uint8_t id=0;
  if (argId != NULL) id = atoi(argId);
  if (id >= numLcdFields) return;
  lcdFields[id].id = id;
  if (argFormat != NULL) 
  {
    charHeap_pushString(id,argFormat);
    //charHeap_printHeap();
  }
}

void commandIUpdateLcdField()
{
  char *argId = cmdI2C.next();
  char *argCol = cmdI2C.next();
  char *argRow = cmdI2C.next();
  char *argLen = cmdI2C.next();
  char *argVis = cmdI2C.next();
  char *argFormat = cmdI2C.next();

  uint8_t id=0;
  bool visible = false;
  if (argId != NULL) id = atoi(argId);
  if (id >= numLcdFields) return;
  lcdFields[id].id = id;
  if (argCol != NULL) lcdFields[id].col = atoi(argCol);
  if (argRow != NULL) lcdFields[id].row = atoi(argRow);
  if (argLen != NULL) lcdFields[id].len = atoi(argLen);
  if (argVis != NULL) lcdFields[id].visible((bool)(atoi(argVis)));
  if (argFormat != NULL) 
  {
    charHeap_pushString(id,argFormat);
  }
}

void commandIUpdateTime()
{
  char *argTime = cmdI2C.next();
  time_t _now;
  if (argTime != NULL) _now = (time_t)(atol(argTime));
  setTime(_now);
}

void commandISetAutoTime()
{
  char *argAutoTime = cmdI2C.next();
  if (argAutoTime != NULL) autoTime = (int8_t)(atoi(argAutoTime));
  if (autoTime >= 0)  lcdFields[autoTime].varType = 9;
}

