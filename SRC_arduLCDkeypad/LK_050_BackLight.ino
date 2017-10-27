

uint8_t BackLight_standby = 15;
uint8_t BackLight_active = 100;
unsigned int seconds_BacklightTmr = 60;


Neotimer tmrBacklightActive = Neotimer(seconds_BacklightTmr * 1000); // 60 seconds

void procBacklight()
{
  if (tmrBacklightActive.waiting())
  {
    backLightPWM = BackLight_active;
    SoftPWMSet(backLight,backLightPWM);
  }
  else
  {
    backLightPWM = BackLight_standby;
    SoftPWMSet(backLight,backLightPWM);
  }
}

void setBacklightActive()
{
  tmrBacklightActive.start();
}

