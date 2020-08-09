//PIN_OUTPUT
#define PIN_DAY_LIGHT 12
#define PIN_LOW_LIGHT 11
#define PIN_HIGH_LIGHT 10

#define PIN_BLINK_LEFT 9
#define PIN_BLINK_RIGHT 8

#define PIN_BRAKE_LEFT_LIGHT 7
#define PIN_BRAKE_RIGHT_LIGHT 6
#define PIN_REVERSE_LIGHT 5
//PIN_INPUT
#define INPUT_DAY_LIGHT A1
#define INPUT_LOW_LIGHT A2
#define INPUT_HIGH_LIGHT A3
#define INPUT_BLINK_LEFT A4
#define INPUT_BLINK_RIGHT A5
#define INPUT_BLINK_BOTH A6
#define INPUT_BRAKE_LIGHT A7
#define INPUT_REVERSE_LIGHT A0

//BLINKERS
#define BLINK_DELAY 450
#define BLINK_TIMER_NAME "BLINK"
#define BLINK_TIMER_SHUT "DONT"
#define BLINK_NONE 0
#define BLINK_LEFT 1
#define BLINK_RIGHT 2
#define BLINK_BOTH 3
//CLK AND TIMER
#define TIMER_LIST_SIZE 5
String timerNames[TIMER_LIST_SIZE];
long timerMillis[TIMER_LIST_SIZE];
//IMPORTS
//#include <BuggyLights_Functions.ino>
//DEBUG
#define DEBUG_ENABLED 0

//GLOBAL_VARS
byte blinkStatus = 0;
/* 0 - Disabled
   1 - left
   2 - right
   3 - both
*/
void setup() {
  // put your setup code here, to run once:
  pinMode(PIN_DAY_LIGHT, OUTPUT);
  pinMode(PIN_LOW_LIGHT, OUTPUT);
  pinMode(PIN_HIGH_LIGHT, OUTPUT);
  pinMode(PIN_BLINK_LEFT, OUTPUT);
  pinMode(PIN_BLINK_RIGHT, OUTPUT);
  pinMode(PIN_BRAKE_LEFT_LIGHT, OUTPUT);
  pinMode(PIN_BRAKE_RIGHT_LIGHT, OUTPUT);
  pinMode(PIN_REVERSE_LIGHT, OUTPUT);

  pinMode(INPUT_DAY_LIGHT, INPUT);
  pinMode(INPUT_LOW_LIGHT, INPUT);
  pinMode(INPUT_HIGH_LIGHT, INPUT);
  pinMode(INPUT_BLINK_LEFT, INPUT);
  pinMode(INPUT_BLINK_RIGHT, INPUT);
  pinMode(INPUT_BLINK_BOTH, INPUT);
  pinMode(INPUT_BRAKE_LIGHT, INPUT);
  pinMode(INPUT_REVERSE_LIGHT, INPUT);

  if (DEBUG_ENABLED == 1) {
    Serial.begin(9600);
  }
  //digitalWrite(PIN_LOW_LIGHT, HIGH);
  //analogWrite(PIN_LOW_LIGHT, 5);
  //timer(BLINK_DELAY,"TEST");
}

void loop() {
  // put your main code here, to run repeatedly:
  byte _blink = getBlinkerInput();
  blinker(_blink);


}

//FUNCTIONS
//PIN_READERS
bool analogPinRead(int _pin){
  if(analogRead(_pin) > 200){
    return true;
  }
  return false;
}
//BLINKER
byte getBlinkerInput() {
  bool _both = (bool)analogPinRead(INPUT_BLINK_BOTH);
  if (_both) {
    return BLINK_BOTH;
  }
  bool _left = (bool)analogPinRead(INPUT_BLINK_LEFT);
  bool _right = (bool)analogPinRead(INPUT_BLINK_RIGHT);
  if (_left && _right) {
    return BLINK_BOTH;
  }
  if (_left) {
    return BLINK_LEFT;
  }
  if (_right) {
    return BLINK_RIGHT;
  }
  return BLINK_NONE;
}
bool blinker(byte _mode) {
  /* 0 - Disabled
     1 - left
     2 - right
     3 - both
  */
  if (_mode != blinkStatus) {
    if (!(awaits(BLINK_TIMER_SHUT) || awaits(BLINK_TIMER_NAME))) {
      timer(0, BLINK_TIMER_NAME);
    }
  }
  blinkStatus = _mode;
  if (clk(BLINK_TIMER_NAME)) {
    timer(BLINK_DELAY, BLINK_TIMER_SHUT);
    switch (_mode) {
      case BLINK_LEFT:
        digitalWrite(PIN_BLINK_LEFT, HIGH);
        break;
      case BLINK_RIGHT:
        digitalWrite(PIN_BLINK_RIGHT, HIGH);
        break;
      case BLINK_BOTH:
        digitalWrite(PIN_BLINK_LEFT, HIGH);
        digitalWrite(PIN_BLINK_RIGHT, HIGH);
        break;
      default:
        break;
    }
  }
  if (clk(BLINK_TIMER_SHUT)) {
    if (_mode != BLINK_NONE) {
      timer(BLINK_DELAY, BLINK_TIMER_NAME);
    }
    digitalWrite(PIN_BLINK_LEFT, LOW);
    digitalWrite(PIN_BLINK_RIGHT, LOW);
  }
}
//END OF BLINKER
//CLK AND TIMER
bool clk(String _clk) {
  int index = -1;
  for (byte i = 0; i < TIMER_LIST_SIZE; i = i + 1) {
    if (timerNames[i].equals(_clk)) {
      index = i;
      break;
    }
  }
  if (index == -1) {
    return false;
  }
  unsigned long currentMillis = millis();
  if (currentMillis >= timerMillis[index]) {
    timerNames[index] = "";
    timerMillis[index] = 0;
    return true;
  }
  return false;
}
bool timer(unsigned long _timer, String _name) {
  for (byte i = 0; i < TIMER_LIST_SIZE; i = i + 1) {
    if (timerNames[i].equals("")) {
      timerNames[i] = _name;
      timerMillis[i] = millis() + _timer;
      return true;
    }
  }
  return false;
}
bool awaits(String _clk) {
  for (byte i = 0; i < TIMER_LIST_SIZE; i = i + 1) {
    if (timerNames[i].equals(_clk)) {
      return true;
    }
  }
  return false;
}
bool removeClk(String _clk) {
  for (byte i = 0; i < TIMER_LIST_SIZE; i = i + 1) {
    if (timerNames[i].equals(_clk)) {
      timerNames[i] = "";
      timerMillis[i] = 0;
      return true;
    }
  }
  return false;
}
//END OF CLK AND TIMER
