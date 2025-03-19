//#include <LiquidCrystal.h>
//#include <i2c_BMP280.h>
//
//LiquidCrystal myLCD(12, 11, 5, 4, 3, 2);
//BMP280 myBMP;
//
//const int SCREEN_WIDTH = 16;
//const int SCREEN_HEIGHT = 2;
//
//const char DEGREE_CHAR = 223;
//
//bool bmpInitialized = false;
//
//float temperatureReadings[60];
//int readingIndex = 0;
//float maxTemperature = -999;
//float sumTemperature = 0;
//
//void setup() {
//  myLCD.begin(SCREEN_WIDTH, SCREEN_HEIGHT);
//  myLCD.clear();
//
//  if (!myBMP.initialize()) {
//    myLCD.write("Failed to init!!");
//    return;
//  }
//  bmpInitialized = true;
//
//  myLCD.setCursor(0, 0);
//  //myLCD.write("Hello World!");
//}
//
//void refreshReadings() {
//  if (!bmpInitialized)
//    return;
//
//  myBMP.awaitMeasurement();
//  float currentTemperature;
//  myBMP.getTemperature(currentTemperature);
//
//  // Aktualizacja odczytów temperatury
//  sumTemperature -= temperatureReadings[readingIndex]; // Usuwanie najstarszego
//  temperatureReadings[readingIndex] = currentTemperature; // Dodanie nowego
//  sumTemperature += currentTemperature; // Dodanie nowego do sumy
//
//  if (currentTemperature > maxTemperature) {
//    maxTemperature = currentTemperature; // Aktualizacja maksymalnej temperatury
//  }
//
//  readingIndex = (readingIndex + 1) % 60; // Cykl po tablicy odczytów
//}
//
//void displayScreen1() {
//  char tempBuffer[10];
//  float avgTemperature = sumTemperature / 60.0;
//
//  myLCD.clear();
//  dtostrf(avgTemperature, 1, 2, tempBuffer);
//  myLCD.setCursor(0, 0);
//  myLCD.print("Srednia temp:");
//  myLCD.setCursor(0, 1);
//  myLCD.print(tempBuffer);
//  myLCD.print((char)DEGREE_CHAR);
//  myLCD.print("C");
//}
//
//void displayScreen2() {
//  char tempBuffer[10];
//
//  myLCD.clear();
//  dtostrf(maxTemperature, 1, 2, tempBuffer);
//  myLCD.setCursor(0, 0);
//  myLCD.print("Max temp:");
//  myLCD.setCursor(0, 1);
//  myLCD.print(tempBuffer);
//  myLCD.print((char)DEGREE_CHAR);
//  myLCD.print("C");
//}
//
//const long updateInterval = 1000; // Czas aktualizacji temperatur
//long previousUpdateTime = millis();
//const long screenInterval = 5000; // Przełączanie ekranu co 5 sek.
//long previousScreenTime = millis();
//int currentScreen = 1;
//
//void loop() {
//  if (millis() - previousUpdateTime > updateInterval) {
//    previousUpdateTime = millis();
//    refreshReadings();
//  }
//
//  if (millis() - previousScreenTime > screenInterval) {
//    previousScreenTime = millis();
//    currentScreen = (currentScreen % 2) + 1; // Przełączenie między ekranami
//    if (currentScreen == 1) {
//      displayScreen1();
//    } else {
//      displayScreen2();
//    }
//  }
//}

//#include <LiquidCrystal.h>
//#include <i2c_BMP280.h>
//
//LiquidCrystal myLCD(12, 11, 5, 4, 3, 2);
//BMP280 myBMP;
//
//const int SCREEN_WIDTH = 16;
//const int SCREEN_HEIGHT = 2;
//const char DEGREE_CHAR = 223;
//
//bool bmpInitialized = false;
//
//float temperatureReadings[60];
//int readingIndex = 0;
//float minTemperature = 999;
//float maxTemperature = -999;
//
//// Wzorce niestandardowych znaków dla "pikseli"
//byte pixelPatterns[8][8] = {
//  {0b00000, 0b00000, 0b00000, 0b00000, 0b00000, 0b00000, 0b00000, 0b11111}, // 1/8
//  {0b00000, 0b00000, 0b00000, 0b00000, 0b00000, 0b00000, 0b11111, 0b11111}, // 2/8
//  {0b00000, 0b00000, 0b00000, 0b00000, 0b00000, 0b11111, 0b11111, 0b11111}, // 3/8
//  {0b00000, 0b00000, 0b00000, 0b00000, 0b11111, 0b11111, 0b11111, 0b11111}, // 4/8
//  {0b00000, 0b00000, 0b00000, 0b11111, 0b11111, 0b11111, 0b11111, 0b11111}, // 5/8
//  {0b00000, 0b00000, 0b11111, 0b11111, 0b11111, 0b11111, 0b11111, 0b11111}, // 6/8
//  {0b00000, 0b11111, 0b11111, 0b11111, 0b11111, 0b11111, 0b11111, 0b11111}, // 7/8
//  {0b11111, 0b11111, 0b11111, 0b11111, 0b11111, 0b11111, 0b11111, 0b11111}, // 8/8
//};
//
//void setup() {
//  myLCD.begin(SCREEN_WIDTH, SCREEN_HEIGHT);
//  myLCD.clear();
//
//  // Ładowanie niestandardowych znaków do LCD
//  for (int i = 0; i < 8; i++) {
//    myLCD.createChar(i, pixelPatterns[i]);
//  }
//
//  if (!myBMP.initialize()) {
//    myLCD.write("Failed to init!!");
//    return;
//  }
//  bmpInitialized = true;
//
//  for (int i = 0; i < 60; i++) {
//    temperatureReadings[i] = 0.0;
//  }
//}
//
//void refreshReadings() {
//  if (!bmpInitialized) return;
//
//  myBMP.awaitMeasurement();
//  float currentTemperature;
//  myBMP.getTemperature(currentTemperature);
//
//  // Aktualizacja zakresu temperatur
//  if (currentTemperature < minTemperature) minTemperature = currentTemperature;
//  if (currentTemperature > maxTemperature) maxTemperature = currentTemperature;
//
//  // Aktualizacja danych wykresu
//  temperatureReadings[readingIndex] = currentTemperature;
//  readingIndex = (readingIndex + 1) % 60;
//}
//
//void displayGraph() {
//  myLCD.clear();
//  float range = maxTemperature - minTemperature;
//  if (range == 0) range = 1;
//
//  for (int x = 0; x < SCREEN_WIDTH; x++) {
//    int index = (readingIndex + x) % 60;
//    float normalizedValue = (temperatureReadings[index] - minTemperature) / range;
//    int pixelLevel = normalizedValue * 8; // Skalowanie na poziomy pikseli
//
//    for (int y = 0; y < SCREEN_HEIGHT; y++) {
//      int charIndex = (pixelLevel > 7) ? 7 : pixelLevel; // Ograniczenie do 8 poziomów
//      myLCD.setCursor(x, SCREEN_HEIGHT - 1 - y);
//      myLCD.write(charIndex);
//      pixelLevel -= 8;
//      if (pixelLevel <= 0) break; // Przestań rysować, gdy wszystkie poziomy są wyświetlone
//    }
//  }
//}
//
//const long updateInterval = 1000; // Aktualizacja co sekundę
//long previousUpdateTime = millis();
//
//void loop() {
//  if (millis() - previousUpdateTime > updateInterval) {
//    previousUpdateTime = millis();
//    refreshReadings();
//    displayGraph();
//  }
//}

#include <LiquidCrystal.h>
#include <i2c_BMP280.h>

LiquidCrystal myLCD(12, 11, 5, 4, 3, 2);
BMP280 myBMP;

const int SCREEN_WIDTH = 16;
const int SCREEN_HEIGHT = 2;
const char DEGREE_CHAR = 223;

bool bmpInitialized = false;

float temperatureReadings[60];
int readingIndex = 0;
float minTemperature = 999;
float maxTemperature = -999;

// Niestandardowe wzory pikseli (0-7 poziomów)
byte pixelPatterns[8][8] = {
  {0b00000, 0b00000, 0b00000, 0b00000, 0b00000, 0b00000, 0b00000, 0b11111}, // 1/8
  {0b00000, 0b00000, 0b00000, 0b00000, 0b00000, 0b00000, 0b11111, 0b11111}, // 2/8
  {0b00000, 0b00000, 0b00000, 0b00000, 0b00000, 0b11111, 0b11111, 0b11111}, // 3/8
  {0b00000, 0b00000, 0b00000, 0b00000, 0b11111, 0b11111, 0b11111, 0b11111}, // 4/8
  {0b00000, 0b00000, 0b00000, 0b11111, 0b11111, 0b11111, 0b11111, 0b11111}, // 5/8
  {0b00000, 0b00000, 0b11111, 0b11111, 0b11111, 0b11111, 0b11111, 0b11111}, // 6/8
  {0b00000, 0b11111, 0b11111, 0b11111, 0b11111, 0b11111, 0b11111, 0b11111}, // 7/8
  {0b11111, 0b11111, 0b11111, 0b11111, 0b11111, 0b11111, 0b11111, 0b11111}, // 8/8
};

void setup() {
  myLCD.begin(SCREEN_WIDTH, SCREEN_HEIGHT);
  myLCD.clear();

  // Załaduj wzory niestandardowych znaków
  for (int i = 0; i < 8; i++) {
    myLCD.createChar(i, pixelPatterns[i]);
  }

  if (!myBMP.initialize()) {
    myLCD.write("Failed to init!!");
    return;
  }
  bmpInitialized = true;

  for (int i = 0; i < 60; i++) {
    temperatureReadings[i] = 0.0;
  }
}

void refreshReadings() {
  if (!bmpInitialized) return;

  myBMP.awaitMeasurement();
  float currentTemperature;
  myBMP.getTemperature(currentTemperature);

  // Aktualizacja minimalnej i maksymalnej temperatury
  if (currentTemperature < minTemperature) minTemperature = currentTemperature;
  if (currentTemperature > maxTemperature) maxTemperature = currentTemperature;

  // Aktualizacja danych wykresu
  temperatureReadings[readingIndex] = currentTemperature;
  readingIndex = (readingIndex + 1) % 60;
}

void displayGraph() {
  myLCD.clear();
  float range = maxTemperature - minTemperature;
  if (range == 0) range = 1; // Uniknięcie dzielenia przez zero

  // Wyświetlanie wykresu
  for (int x = 0; x < SCREEN_WIDTH; x++) {
    int index = (readingIndex + x) % 60;
    float normalizedValue = (temperatureReadings[index] - minTemperature) / range;
    int pixelLevel = normalizedValue * 16; // Skala na 16 poziomów

    // Wyświetlanie kolumny słupka
    for (int y = 0; y < SCREEN_HEIGHT; y++) {
      int charIndex = (pixelLevel > 7) ? 7 : pixelLevel; // Limit 8 poziomów na znak
      myLCD.setCursor(x, SCREEN_HEIGHT - 1 - y);
      myLCD.write(charIndex);
      pixelLevel -= 8;
      if (pixelLevel <= 0) break; // Zakończ rysowanie, gdy wszystkie poziomy wyświetlone
    }
  }
}

const long updateInterval = 1000; // Aktualizacja co sekundę
long previousUpdateTime = millis();

void loop() {
  if (millis() - previousUpdateTime > updateInterval) {
    previousUpdateTime = millis();
    refreshReadings();
    displayGraph();
  }
}
