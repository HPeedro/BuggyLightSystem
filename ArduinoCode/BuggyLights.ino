//PIN
#define PIN_DAY_LIGHT 12
#define PIN_LOW_LIGHT 11
#define PIN_HIGH_LIGHT 10

#define PIN_BLINK_LEFT 9
#define PIN_BLINK_RIGHT 8

#define PIN_BREAK_LEFT_LIGHT 7
#define PIN_BREAK_RIGHT_LIGHT 6
#define PIN_REVERSE_LIGHT 5
//MISC
#define BLINK_DELAY 450
//IMPORTS
//#include <BuggyLights_Functions.ino>
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

  Serial.begin(9600);
  digitalWrite(PIN_LOW_LIGHT, HIGH);
  //analogWrite(PIN_LOW_LIGHT, 5);
  timer(BLINK_DELAY,"TEST");
}

void loop() {
  // put your main code here, to run repeatedly:
  if(clk("TEST")){
    digitalWrite(PIN_LOW_LIGHT, LOW);
    timer(BLINK_DELAY,"TIME");
  }
  if(clk("TIME")){
    digitalWrite(PIN_LOW_LIGHT, HIGH);
    timer(BLINK_DELAY,"TEST");
  }
}

//FUNCTIONS
//CLK AND TIMER
#define TIMER_LIST_SIZE 10
String timerNames[TIMER_LIST_SIZE];
long timerMillis[TIMER_LIST_SIZE];
bool clk(String _clk){
  int index = -1;
  for (byte i = 0; i < TIMER_LIST_SIZE; i = i + 1) {
    if(timerNames[i].equals(_clk)){
      index = i;
      break;
    }
  }
  if(index == -1){
    return false;
  }
  unsigned long currentMillis = millis();
  if(currentMillis >= timerMillis[index]){
    timerNames[index] = "";
    timerMillis[index] = 0;
    return true;
  }
  return false;
}
bool timer(unsigned long _timer, String _name){
  for (byte i = 0; i < TIMER_LIST_SIZE; i = i + 1) {
    if(timerNames[i].equals("")){
      timerNames[i] = _name;
      timerMillis[i] = millis()+_timer;
      return true;
    }
  }
  return false;
}
//END OF CLK AND TIMER
