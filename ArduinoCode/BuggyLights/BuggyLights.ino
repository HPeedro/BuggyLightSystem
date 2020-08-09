//PIN_OUTPUT
#define PIN_DAY_LIGHT 12
#define PIN_LOW_LIGHT 11
#define PIN_HIGH_LIGHT 10

#define PIN_BLINK_LEFT 9
#define PIN_BLINK_RIGHT 8

#define PIN_BREAK_LEFT_LIGHT 7
#define PIN_BREAK_RIGHT_LIGHT 6
#define PIN_REVERSE_LIGHT 5
//PIN_INPUT

//MISC
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
#define DEBUG_ENABLED 1

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
  pinMode(PIN_BREAK_LEFT_LIGHT, OUTPUT);
  pinMode(PIN_BREAK_RIGHT_LIGHT, OUTPUT);
  pinMode(PIN_REVERSE_LIGHT, OUTPUT);

  if (DEBUG_ENABLED == 1) {
    Serial.begin(9600);
  }
  //digitalWrite(PIN_LOW_LIGHT, HIGH);
  //analogWrite(PIN_LOW_LIGHT, 5);
  //timer(BLINK_DELAY,"TEST");
}

void loop() {
  // put your main code here, to run repeatedly:
  int _input = -1;
  if (Serial.available() > 0) {
    _input = Serial.readString().toInt();
    Serial.println(_input);
  }
  if (_input != -1) {
    blinker(_input);
    Serial.println("BLINK!");
  } else {
    blinker(blinkStatus);
  }

}

//FUNCTIONS
//BLINKER
bool blinker(byte _mode) {
  /* 0 - Disabled
     1 - left
     2 - right
     3 - both
  */
  if (_mode != blinkStatus) {
    if(!(awaits(BLINK_TIMER_SHUT) || awaits(BLINK_TIMER_NAME))){
      timer(0,BLINK_TIMER_NAME);
    }
    
  }
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
      digitalWrite(PIN_BLINK_LEFT, LOW);
      digitalWrite(PIN_BLINK_RIGHT, LOW);
    }
  }
  blinkStatus = _mode;
}
bool shutBlinker() {

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
