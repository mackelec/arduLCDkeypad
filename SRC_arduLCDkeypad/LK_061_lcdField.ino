char preBuffer[10],postBuffer[10];



const char timeFormat_1[] PROGMEM = {"HH:mm:ss"};
const char timeFormat_2[] PROGMEM = {"HH:mm"};
const char timeFormat_3[] PROGMEM = {"YYYY-MM-dd HH:mm:ss"};
const char timeFormat_4[] PROGMEM = {"YYYY-MM-dd HH:mm"};

const char printTimeFormat_1[] PROGMEM = {"%02d:%02d:%02d"};
const char printTimeFormat_2[] PROGMEM = {"%02d:%02d"};
const char printTimeFormat_3[] PROGMEM = {"%04d-%02d-%02d %02d:%02d:%02d"};
const char printTimeFormat_4[] PROGMEM = {"%04d-%02d-%02d %02d:%02d"};

class lcdField
{
public:  
  uint8_t id=255;
  char *displayString; 
  char *formatString;
  uint8_t col;
  uint8_t row;
  uint8_t len;
 //---- flags : bit 0 - visible, bit 1 = cleared / over written, bit 2 = flashing
  byte flags=0;
  //bool visible;
  //bool flashing;
  uint8_t flashOn=10; //milliSec * 100
  uint8_t flashOff=5; //milliSec * 100
  uint8_t  flashModeTimeout = 0; // seconds
  uint8_t  visibleModeTimeout = 0;
  uint8_t varType = 0;
  void updateVar(unsigned long Lvar);
  bool isVisible();
  void visible(bool v);
  uint8_t isCleared();
  void Clear(byte clr);
  

  Neotimer tmrFlashing = Neotimer();
  void genString();
  void buffPrint_float(PString *pStr);
  void buffPrint_time(char *buff);
  union _varToBytes
  {
    uint8_t uint8_value;
    int8_t int8_value;
    unsigned int uint_value;
    int int_value;
    unsigned long ulong_value;
    long long_value;
    float float_value;
    time_t time_value;
    byte bytes[4];
  }u;
  //_varToBytes u;
};


void lcdField::updateVar(unsigned long Lvar)
{
  u.long_value = Lvar;
}

void lcdField::genString()
{
  
  if (displayString)
  {
    
  
  }

  formatString = charHeap_String(id);
  if (! formatString) return;
  char _buff[21];
  PString pStr(_buff,sizeof(_buff));
  if (formatString)
  {
    //Serial << F("formatString = [") << formatString << F("] ") << varType << endl;
    pStr << formatString;
    if (varType == 6)
    {
      if (strcmp_P(formatString,PSTR("HH:mm:ss")) == 0) varType = 9;
      if (strcmp_P(formatString,PSTR("HH:mm")) == 0) varType = 9;
      if (strcmp_P(formatString,PSTR("YYYY-MM-dd HH:mm:ss")) == 0) varType = 9;
      if (strcmp_P(formatString,PSTR("YYYY-MM-dd HH:mm")) == 0) varType = 9;
    }
  }

  //Serial << F("varType = ") << varType << endl;
  pStr.begin();
  switch (varType)
  {
    case 2:
      pStr.format(formatString,u.uint8_value);
      break;
    case 3:
      break;
    case 4:
      break;
    case 5:
      pStr.format(formatString,u.int_value);
    case 6:
      break;
    case 7:
      break;
    
    case 8:
      buffPrint_float(&pStr);
      break;
    case 9:
      //--- time format
      buffPrint_time(_buff);
      break;
  }
  //Serial << F("strFormat = [") << formatString << F("]") << endl;  
  lcd.setCursor((int)col,(int)row);
  lcd.print(pStr) ;
  //Serial << F("detail :") << col << F(",") << row << F("  print string= [") << pStr << F("]") << endl;
}

void lcdField::buffPrint_time(char *buff)
{
  //pStr->begin();
  time_t t = u.time_value;
  //char *fmt;
  if (strcmp_P(formatString,PSTR("HH:mm:ss")) == 0) 
  {
    sprintf_P(buff,PSTR("%02d:%02d:%02d"),hour(t),minute(t),second(t));
  }

  if (strcmp_P(formatString,PSTR("YYYY-MM-dd HH:mm:ss")) == 0) 
  {
    sprintf_P(buff,PSTR("%04d-%02d-%02d %02d:%02d:%02d"),year(t),month(t),day(t),hour(t),minute(t),second(t));
  }
  
}

void lcdField::buffPrint_float(PString *pStr)
{
  char *strPreFormat,*strPostFormat;
  int precision = floatPrecision(formatString,preBuffer,postBuffer);
  int fSize = strlen(preBuffer) + strlen(postBuffer) +10;
  char strF[fSize];
  PString pstrF(strF,fSize);
  pstrF << preBuffer << F("ld.%0") << precision << F("lu") << postBuffer;
  //long mult=1;
  //if (u.float_value < 0)mult = -1L;
  long d = (long)( pow(10,precision)+.5);
  long Lf = (long)(u.float_value * d);
  Lf = (long) abs(Lf);
  
  //Serial << F("float var = ") << u.float_value << " , " << Lf << " [" << pstrF << "]" << endl;
  long _mod = (long) (Lf/d) ;
  _mod = _mod * d;
  _mod = Lf - _mod;
  long _k = (long)(Lf / d);
  pStr->begin();
  pStr->format(strF,(long)u.float_value, (long)(_mod));
  
}

uint8_t floatPrecision(char strFormat[],char *strPreFormat,char *strPostFormat)
{
  char * work;
  work = strchr(strFormat,'.');
  //Serial << F("format=[") << strFormat << "] " << endl;
  //---- ** precision
  uint8_t fp = work[1] - '0';
  //Serial << F("FP= ") << fp << "," << work << ":" << work[1] << endl;
  
 //---  create first part of new format string
 
  char *pre = preBuffer;
  //if (!pre) Serial << F("FAIL") << endl;
  //Serial << F("sizeof = ") << sizeof(strFormat) << "," << strlen(strFormat) <<  "," << sizeof(pre) << "," << sizeof(preBuffer) << endl;
  PString p1(preBuffer,sizeof(preBuffer));
  p1 << strFormat;
  //strcpy(pre,strFormat);
  //Serial << F("format2=[") << strFormat << "] " << pre << endl;
  //memcpy(pre,strFormat,strlen(strFormat)+1);
  char *a = strchr(pre,'.');
  //if (a == NULL) Serial << F("its NULL") << endl;
  //Serial << F("***preformat=[") << pre << "] " << a << endl;
  //a--;
  //*a = 'd';
  //a++;
  *a = 0; // -- terminate string
  strPreFormat = pre;
  //Serial << F("preformat=[") << strPreFormat << "]" << endl;
 //---- create second part of new format string
  
  //memcpy(after,strFormat,strlen(strFormat)+1);
  char *b = strchr(strFormat,'f');
  if (b == NULL)    b = strchr(strFormat,'F');
  b++;
  //char after[strlen(b)+1];
  //memcpy(after,b,strlen(b)+1);
  //int _size = sizeof(pre) + sizeof(after)+1;
  //char *_buf = (char *)malloc (sizeof(b)+1);
  PString p2(postBuffer,sizeof(postBuffer));
  p2 << b;
  strPostFormat = postBuffer;
  //Serial << F("FP buffers=[") << p1 << F("],[") << p2 << F("]") << endl;
  return fp;
}


uint8_t lcdField::isCleared()
{
  byte junk = flags;
  uint8_t b =  bitRead(junk,1);
  return (uint8_t)(1);
}
void lcdField::Clear(byte clr)
{
  //bitWrite(flags,1,1);
  //bitSet(flags,1);
}
bool lcdField::isVisible()
{
  return bitRead(flags,0);
}
void lcdField::visible(bool v)
{
  bitWrite(flags,0,v);
}

