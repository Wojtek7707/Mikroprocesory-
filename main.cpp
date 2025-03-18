// SWiM: Lab04
#include <LiquidCrystal.h>
#include <i2c_BMP280.h>

LiquidCrystal myLCD(12, 11, 5, 4, 3, 2);
BMP280 myBMP;

const int SCREEN_WIDTH = 16;
const int SCREEN_HEIGHT = 2;

const char DEGREE_CHAR = 223;

bool bmpInitialized = false;

void setup() {
  myLCD.begin(SCREEN_WIDTH, SCREEN_HEIGHT);
  myLCD.clear();

  if (!myBMP.initialize()) {
    myLCD.write("Failed to init!!");
    return;
  }
  bmpInitialized = true;

  myLCD.setCursor(0, 0);
  myLCD.write("Hello World!");
}

char displayBuffer[SCREEN_HEIGHT][SCREEN_WIDTH + 1];

void refreshReadings() {
  if (!bmpInitialized)
    return;

  myBMP.awaitMeasurement();
  float currentPressure;
  float currentTemperature;
  myBMP.getPressure(currentPressure);
  myBMP.getTemperature(currentTemperature);

  char tempBuffer[10];
  dtostrf(currentPressure, 1, 2, tempBuffer);
  sprintf(displayBuffer[0], "psi [Pa]: %-6s", tempBuffer);
  dtostrf(currentTemperature, 1, 2, tempBuffer);
  sprintf(displayBuffer[1], "tmp [%cC]: %-6s", DEGREE_CHAR, tempBuffer);

  for (int i = 0; i < SCREEN_HEIGHT; ++i) {
    myLCD.setCursor(0, i);
    myLCD.print(displayBuffer[i]);
  }
}

const long updateInterval = 1000;
long previousUpdateTime = millis();

void loop() {
  if (millis() - previousUpdateTime > updateInterval) {
    previousUpdateTime = millis();
    refreshReadings();
  }
}
