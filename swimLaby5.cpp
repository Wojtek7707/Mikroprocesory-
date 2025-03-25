#include <LiquidCrystal.h>
#include <TimerOne.h>

LiquidCrystal LCD(12, 11, 7, 6, 5, 4);
const int SCR_W = 16;
const int SCR_H = 2;

const int PIN_B1 = 8;
const int PIN_B2 = 2;

void setup() {
  pinMode(PIN_B1, INPUT_PULLUP);
  pinMode(PIN_B2, INPUT_PULLUP);

  Timer1.initialize(500000);

  LCD.begin(SCR_W, SCR_H);
  LCD.clear();
  LCD.setCursor(0, 0);

  readValue();
  setTimer();
}

volatile bool UPD = true;
volatile double VOLT;
volatile bool TMR_MODE = false;

void updateLCD() {
  if(!UPD) return;
     UPD = false;

  LCD.setCursor(0, 0);
  LCD.print("A5 [V]: ");
  LCD.print(VOLT);
  LCD.setCursor(0, 1);
  if(TMR_MODE) {
    LCD.print("Timer mode ");
  } else {
    LCD.print("Button mode");
  }
}

void readValue() {
  VOLT = analogRead(A5)/1024.0 * 5;
  UPD = true;
}

void setManual() {
  TMR_MODE = false;
  Timer1.detachInterrupt();
  attachInterrupt(digitalPinToInterrupt(PIN_B2), readValue, FALLING);
  UPD = true;
}

void setTimer() {
  TMR_MODE = true;
  detachInterrupt(digitalPinToInterrupt(PIN_B2));
  Timer1.attachInterrupt(readValue);
  UPD = true;
}

int BTN_STATE;
int BTN_LAST;
unsigned long DEBOUNCE_T = 0;
const unsigned long DEBOUNCE_D = 50;

void onPress() {
  if(TMR_MODE) {
    setManual();
  } else {
    setTimer();
  }
}

void readButton() {
  int READ = digitalRead(_PIN_B1);
 
  if (READ != BTN_LAST) {
    DEBOUNCE_T = millis();
  }

  if ((millis() - DEBOUNCE_T) > DEBOUNCE_D) {
    if (READ != BTN_STATE) {
      BTN_STATE = READ;

      if (BTN_STATE == LOW) {
        onPress();
      }
    }
  }

  BTN_LAST = READ;
}

void loop() {
  readButton();
  updateLCD();
}