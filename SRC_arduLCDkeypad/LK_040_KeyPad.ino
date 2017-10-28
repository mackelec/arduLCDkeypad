
/*
 * 
 * 
 *    The KeyPad library strobes the Columns and reads the Rows. 
 *    After strobing the Columns it sets the pins as high impedance inputs.
 *    Digital pin 8 is shared between the keypad and LCD-D8, so I believe 
 *    it is best used as a Column, hence C1.
 *    
 *    
 */

//----- 4x4 KeyPad

/*
#define R1 10
#define R2 11
#define R3 12
#define R4 13
#define C1  8
#define C2 14
#define C3 15
#define C4 16
#define C5 17
*/

//----- 5x4 Keypad

#define C5 10
#define C4 11
#define C3 12
#define C2 13
#define C1  8   // easier to leave this alone  (shared with LCD-D7)
#define R4 14
#define R3 15
#define R2 16
#define R1 17

char key;
char i2cNoKey = '!';

const byte rows = 4;
const byte cols = 5;

char keys4x4[rows][cols]= {
  {'D','C','B','A','x'},
  {'#','9','6','3','x'},
  {'0','8','5','2','x'},
  {'*','7','4','1','x'}
  };

//------  5x4 Keypad

char keys[rows][cols]= {
  {'L','7','4','1','A'},
  {'0','8','5','2','B'},
  {'R','9','6','3','#'},
  {'E','X','D','U','*'}
  };
  
byte rowPins[rows] = {R1,R2,R3,R4};
byte colPins[cols] = {C1,C2,C3,C4,C5};

Keypad Kpad = Keypad(makeKeymap(keys),rowPins, colPins, rows, cols );

long buzzerTimeout = 50;
Neotimer buttonClickTimer = Neotimer(buzzerTimeout);

void checkKeyPad()
{
  char _key = Kpad.getKey();
  pinMode(C1,OUTPUT); // shared with LCD-D7
  
  if (_key != NO_KEY)
  {
    key = _key;
    //Serial << "key=" << _key <<endl;
    digitalWrite(buzzer,HIGH);
    buttonClickTimer.start();
    setBacklightActive();
  }
}

void I2CrequestEvent()
{
  if (pMessage == "reset")
  {
    pMessage << endl;
    Wire.write(messBuffer);
    //Serial << "sending Reset " << pMessage << endl;
    pMessage.begin();
    return;
  }
  char buff[7] = {'k','e','y','=','!','\n',0};
  buff[4] = i2cNoKey;
  if (key != NO_KEY)
  {
    buff[4] = key;
    key = NO_KEY;
    Wire.write(buff);
  }
  else  Wire.write(buff);
}




void buttonPressClickTimeout()
{
  if (buttonClickTimer.done())
  {
    digitalWrite(buzzer,LOW);
    buttonClickTimer.set(buzzerTimeout);
  }
}

