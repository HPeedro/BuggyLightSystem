#include <FastLED.h>

//PIN_OUTPUT
#define PIN_FRONT_LEFT 12
#define PIN_FRONT_RIGHT 11
#define PIN_REAR_LEFT 10
#define PIN_REAR_RIGHT 9

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
//COLLORS
CRGB c_blink = CRGB(255,255,0);
CRGB c_break = CRGB(255,0,0);
CRGB c_break_low = CRGB(100,0,0);
CRGB c_daylight = CRGB(255,255,255);
CRGB c_reverse = CRGB(255,255,255);
//ARRAY_SIZE
#define REAR_SIZE 5
CRGB rear_left[REAR_SIZE];
CRGB rear_right[REAR_SIZE];
#define FRONT_SIZE 5
CRGB front_left[FRONT_SIZE];
CRGB front_right[FRONT_SIZE];
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
  //take the traash out (arrays)
  for(int i = 0; i < REAR_SIZE; i++){
    rear_left[i] = CRGB(0,0,0);
    rear_right[i] = CRGB(0,0,0);
  }
  for(int i = 0; i < FRONT_SIZE; i++){
    front_left[i] = CRGB(0,0,0);
    front_right[i] = CRGB(0,0,0);
  }
  //pinMode
  pinMode(INPUT_DAY_LIGHT, INPUT_PULLUP);
  pinMode(INPUT_LOW_LIGHT, INPUT_PULLUP);
  pinMode(INPUT_HIGH_LIGHT, INPUT_PULLUP);
  pinMode(INPUT_BLINK_LEFT, INPUT_PULLUP);
  pinMode(INPUT_BLINK_RIGHT, INPUT_PULLUP);
  pinMode(INPUT_BLINK_BOTH, INPUT_PULLUP);
  pinMode(INPUT_BRAKE_LIGHT, INPUT_PULLUP);
  pinMode(INPUT_REVERSE_LIGHT, INPUT_PULLUP);
  //Set led things
  FastLED.addLeds<WS2812, PIN_FRONT_LEFT, GRB>(front_left, FRONT_SIZE);
  FastLED.addLeds<WS2812, PIN_FRONT_RIGHT, GRB>(front_right, FRONT_SIZE);
  FastLED.addLeds<WS2812, PIN_REAR_LEFT, GRB>(rear_left, REAR_SIZE);
  FastLED.addLeds<WS2812, PIN_REAR_RIGHT, GRB>(rear_right, REAR_SIZE);
  //DEBUG
  if (DEBUG_ENABLED == 1) {
    Serial.begin(9600);
  }
}

void loop() {
  
  byte _blink = getBlinkerInput();
  
  blinker(_blink);
  FastLED.show();
  dayLightAndBreak();

  if(awaits(BLINK_TIMER_SHUT)){
    Serial.println(blinkStatus);
    Serial.println("awaits");
  } else {
    Serial.println(blinkStatus);
    Serial.println("does not");
  }
  
  FastLED.show();
}

//FUNCTIONS
//BRAKE_LIGHTS
void dayLightAndBreak() {
  bool _lowLight = analogPinRead(INPUT_LOW_LIGHT);
  bool _breakLight = analogPinRead(INPUT_BRAKE_LIGHT);
  if(!(awaits(BLINK_TIMER_SHUT) && (blinkStatus == BLINK_BOTH || blinkStatus == BLINK_RIGHT))){
    Serial.println("JOIN RIGHT");
    if (_lowLight) {
      arrayFullChange(front_right, c_daylight);
      if (_breakLight) {
        arrayFullChange(rear_right, c_break);
      } else {
        arrayFullChange(rear_right, c_break_low);
      }
    } else {
      arrayFullChange(front_right, c_daylight);
      arrayFullChange(rear_right, c_break);
    }
  }
  if(!(awaits(BLINK_TIMER_SHUT) && (blinkStatus == BLINK_BOTH || blinkStatus == BLINK_LEFT))){
    Serial.println("JOIN LEFT");
    if (_lowLight) {
      arrayFullChange(front_left, c_daylight);
      if (_breakLight) {
        arrayFullChange(rear_left, c_break);
      } else {
        arrayFullChange(rear_left, c_break_low);
      }
    } else {
      arrayFullChange(front_left, c_daylight);
      arrayFullChange(rear_left, c_break);
    }
  }
  //analogPinRead(INPUT_BRAKE_LIGHT);
}
//PIN_READERS
bool analogPinRead(int _pin) {
  if (analogRead(_pin) > 450) {
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
        arrayFullChange(front_left, c_blink);
        arrayFullChange(rear_left, c_blink);
        break;
      case BLINK_RIGHT:
        arrayFullChange(front_right, c_blink);
        arrayFullChange(rear_right, c_blink);
        break;
      case BLINK_BOTH:
        arrayFullChange(front_left, c_blink);
        arrayFullChange(front_right, c_blink);
        arrayFullChange(rear_left, c_blink);
        arrayFullChange(rear_right, c_blink);
        break;
      default:
        break;
    }
  }
  if (clk(BLINK_TIMER_SHUT)) {
    if (_mode != BLINK_NONE) {
      timer(BLINK_DELAY, BLINK_TIMER_NAME);
    }
    //arrayFullChange(front_left, c_daylight);
    //arrayFullChange(front_right, c_daylight);
    //arrayFullChange(rear_left, c_break_low);
    //arrayFullChange(rear_right, c_break_low);
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
//ARRAY FUNCTIONS
void arrayFullChange(CRGB _array[], CRGB _color){
  int _size = sizeof(_array);
  for(int i = 0; i < _size; i++){
    _array[i] = _color;
  }
}
//END OF ARRAY FUNCTIONS
