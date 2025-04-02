#include <LiquidCrystal.h>

// Inicjalizacja obiektu wyświetlacza
LiquidCrystal myDisplay(12, 11, 5, 4, 3, 2);
const int DISPLAY_COLS = 16;
const int DISPLAY_ROWS = 2;

// Przypisanie pinów do czujników
const int SENSOR_PIN = 10;
const int DISTANCE_TRIG = 8;
const int DISTANCE_ECHO = 9;
const int DIODE = 13;

void setup() {
  pinMode(SENSOR_PIN, INPUT);
  pinMode(DISTANCE_TRIG, OUTPUT);
  pinMode(DISTANCE_ECHO, INPUT);
  pinMode(DIODE, OUTPUT);
  digitalWrite(DISTANCE_TRIG, LOW);

  myDisplay.begin(DISPLAY_COLS, DISPLAY_ROWS);
  myDisplay.clear();
}

// Globalne zmienne sterujące stanem wyświetlacza
bool isDataUpdated = true;
bool motionDetected = false;
int distanceMeasured = 0;

void refreshDisplay() {
  if (!isDataUpdated) return;
  isDataUpdated = false;

  myDisplay.setCursor(0, 0);
  if (motionDetected) {
    myDisplay.print("ruch      ");
    digitalWrite(DIODE, HIGH);
  } else {
    myDisplay.print("brak ruchu");
    digitalWrite(DIODE, LOW);
  }

  myDisplay.setCursor(0, 1);
  myDisplay.print("dist: ");
  if (distanceMeasured <= 0) {
    myDisplay.print("OoR    ");
  } else {
    myDisplay.print(distanceMeasured);
    myDisplay.print("cm   ");
  }
}

unsigned long lastMotionTime = 0;
unsigned long motionTimeout = 2000;

void checkMotion() {
  int sensorValue = digitalRead(SENSOR_PIN);
  if (sensorValue == HIGH && millis() - lastMotionTime > motionTimeout) {
    lastMotionTime = millis();
  }
  if (lastMotionTime == 0 || millis() - lastMotionTime > motionTimeout) {
    if (motionDetected) isDataUpdated = true;
    motionDetected = false;
  } else {
    if (!motionDetected) isDataUpdated = true;
    motionDetected = true;
  }
}

const double MICROSECONDS_TO_CM = 29.1 * 2;
const unsigned int MAX_DISTANCE_CM = 200;
const unsigned long ULTRA_TIMEOUT_MICROS = ceil(MAX_DISTANCE_CM * MICROSECONDS_TO_CM);

void measureDistance() {
  digitalWrite(DISTANCE_TRIG, HIGH);
  delayMicroseconds(10);
  digitalWrite(DISTANCE_TRIG, LOW);

  int echoDuration = pulseIn(DISTANCE_ECHO, HIGH, ULTRA_TIMEOUT_MICROS);
  distanceMeasured = echoDuration / MICROSECONDS_TO_CM;
  isDataUpdated = true;
}

void loop() {
  checkMotion();
  measureDistance();
  refreshDisplay();
}
