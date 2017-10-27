

lcdField lcdFields[numLcdFields];



void paintAll()
{
  
}

void paint(uint8_t id)
{
  if (lcdFields[id].isVisible())
  {
    printBlank(lcdFields[id].col,lcdFields[id].row,lcdFields[id].len);
    //lcd.print(lcdFields[id].displayString);
  }
}


void updateTimeField()
{
  if (autoTime < 0) return;
  long Ltime = (long)(now());
  lcdFields[autoTime].u.long_value = Ltime;
  lcdFields[autoTime].genString();
}

