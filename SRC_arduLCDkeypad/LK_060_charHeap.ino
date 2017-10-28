
#define numLcdFields      5
#define charHEAP_SIZE     100
#define charHEAP_MAXLEN   25

int8_t autoTime = -1;
char charHeap[charHEAP_SIZE];

uint8_t charHeapOffsets[numLcdFields *2];

void init_charHeap()
{
  memset(charHeap,0,sizeof(charHeap));
  for (int i =0;i < sizeof (charHeapOffsets);i++)charHeapOffsets[i] = 255;
}

char* charHeap_String(uint8_t index)
{
  char *p = NULL;
  if (charHeapOffsets[index] < charHEAP_SIZE)
  {
    p = charHeap;
    p=p+charHeapOffsets[index];
  }
  return p;
}

bool charHeap_pushString(uint8_t index,char *strValue)
{
  charHeap_deleteString(index);
  if (strValue == NULL) return false;
  uint8_t len = strlen(strValue);
  if (len < 1 ) return false;
  len ++;
  uint8_t freespace = charHeap_freeMem();
  if (len > freespace) return false;
  uint8_t lastptr = charHeap_lastCharOffset();
  charHeapOffsets[index] = charHeap_getOffset(index);
  charHeap_makeSpace(index,len);
  
  char *p = charHeap;
  p=p+charHeapOffsets[index];
  strcpy(p,strValue);
}

uint8_t charHeap_getOffset(uint8_t index)
{
  if (index==0) return 0;
  uint8_t lastPtr=-1,len=0,offset=0;
  for (int i = index-1;i>=0;i--)
  {
    lastPtr = charHeapOffsets[i];
    if (lastPtr < charHEAP_SIZE)
    {
      len = charHeap_len(i);
      offset = charHeapOffsets[i] + len;
      break;
    }
  }
  return offset;
}

void charHeap_deleteString(uint8_t index)
{
  if (charHeapOffsets[index] < sizeof(charHeap))
  {
    charHeap[charHeapOffsets[index]] = 0;
    charHeapOffsets[index] = 255;
    charHeap_deFrag();
  }
}

void charHeap_makeSpace(uint8_t index, uint8_t len)
{
  uint8_t lastptr = charHeap_lastCharOffset();
  if (lastptr < sizeof(charHeap))
  {
    for (uint8_t i = lastptr;i >= charHeapOffsets[index] && i<sizeof(charHeap) ;i--)
    {
      charHeap[i+len] = charHeap[i]; 
    }
    for (uint8_t i = index +1;i < sizeof(charHeapOffsets);i++)
    {
      if (charHeapOffsets[i] < sizeof(charHeap) ) charHeapOffsets[i] += len; 
    }
  }
}

uint8_t charHeap_lastCharOffset()
{
  uint8_t len=0;
  uint8_t ptr=0;
  for (int i = sizeof (charHeapOffsets)-1;i>=0;i--)
  {
    if (charHeapOffsets[i] < charHEAP_SIZE)
    {
      len = charHeap_len(i);
      ptr = charHeapOffsets[i];
      break;
    }
  }
  uint8_t lastCharOffset = ptr + len -1;
  return lastCharOffset;
}

uint8_t charHeap_freeMem()
{
  uint8_t len=0;
  uint8_t ptr=0;
  for (int i = numLcdFields*2;i>0;i--)
  {
    if (charHeapOffsets[i-1] < charHEAP_SIZE)
    {
      len = charHeap_len(i-1);
      ptr = charHeapOffsets[i-1];
      break;
    }
  }
  uint8_t freespace = sizeof(charHeap) - ptr - len;
  return freespace;
}


void charHeap_deFrag()
{
  uint8_t lastptr=0;
  for (uint8_t i=1;i<sizeof(charHeapOffsets);i++)
  {
    if (charHeapOffsets[i] >= sizeof(charHeap)) continue;
    uint8_t len = 0;
    for (int8_t j=i-1;j>=0;j--)
    {
      if (charHeapOffsets[j] >= sizeof(charHeap) && j!=0) continue;
      len = charHeap_len(j);
      lastptr = charHeapOffsets[j];
      if (lastptr >= sizeof(charHeap)) lastptr = 0;
      uint8_t ptr = lastptr + len ;
      uint8_t shiftChars = 0;
      if (charHeapOffsets[i] > ptr) shiftChars = charHeapOffsets[i] - ptr;
      if (shiftChars != 0) 
      {
        charHeap_Compress(i,ptr);
        charHeapOffsets[i] = ptr;
      }
      break;
    }
  }
  charHeap_Clean();
}

void charHeap_Compress(uint8_t index, uint8_t newOffSet)
{
  uint8_t len=charHeap_len(index);
  uint8_t oldPtr = charHeapOffsets[index];
  uint8_t d = oldPtr - newOffSet ;
  char *p=&charHeap[newOffSet];
  for (uint8_t i=0;i<len+1;i++)
  {
    p[i] = p[i+d];
  }
}

void charHeap_Clean()
{
  uint8_t lastptr = charHeap_lastCharOffset()+1;
  for (uint8_t i=lastptr;i< sizeof(charHeap);i++)
  {
    charHeap[i] = 0;
  }
}

uint8_t charHeap_len(uint8_t index)
{
  if (charHeapOffsets[index] > sizeof(charHeap)) return 0;
  uint8_t p = charHeapOffsets[index];
  char *str = &charHeap[p];
  uint8_t len = strlen(str)+1;
  return len;
}

void charHeap_printHeap()
{
  Serial << F("--- charHeap Offsets --- ") ;
  for (int i = 0;i<sizeof(charHeapOffsets);i++) Serial << charHeapOffsets[i] << ", " ;
  Serial << endl;
  Serial << F("charHeap contents") <<  endl; 
  bool nonZero = false;
  for (int i =0; i< sizeof(charHeap)-1; i++)
  {
    
    char c = charHeap[i];
    if (c  >= ' ' )
    {
      nonZero = true;
      Serial << c;
    }
    else
    {
      if (nonZero && (int)(c) == 0)
      {
        Serial << "< CHAR > " << (int)(c) << endl; 
        nonZero = false;
      }
      else if (! nonZero && (int)(c) == 0)
      {
        Serial << '_';
      }
      else 
      {
        Serial << "< CHAR > " << (int)(c) << endl;
      }
    }
  }
  Serial << endl;
}

