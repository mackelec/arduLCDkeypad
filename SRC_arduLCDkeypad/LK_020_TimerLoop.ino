
unsigned long milliTimer,millitimer_10,millitimer_1000;

Neotimer millitimer_100 = Neotimer(100);
//Neotimer millitimer_1000 = Neotimer(1000);


void timerLoop()
{
  static time_t lastT= now();
  if (millis() != milliTimer) 
  {
    milliTimer = millis();
    Loop_1();
    if (millis()-millitimer_10>9)
    {
      millitimer_10 = millis();
      Loop_10();
      if (millitimer_100.repeat()) 
      {
        Loop_100();
        if (now() != lastT)
        {
          lastT = now();
          Loop_oneSecond();
        }
      }
      if (millis()-millitimer_1000>999)
      {
        millitimer_1000 = millis();
        Loop_1000();
      }
    }
  }
}


void Loop_1()
{
  
}
void Loop_10()
{
  readStreams();
  buttonPressClickTimeout();
  checkKeyPad();
  
}

void Loop_100()
{
  procBacklight();
}

void Loop_1000()
{
  
}

void Loop_oneSecond()
{
  updateTimeField();
}

