// SWiM: Lab07 - zmienione nazwy zmiennych
#define _USE_MATH_DEFINES
#include <cmath>
#include <SPI.h>
#include "LCD.h"
#include "DEV_Config.h"
#include "ADXL345.h"

// Przykładowa instancja akcelerometru – nazwa została zmieniona
ADXL345 accelSensor;

void setup() {
  // Inicjalizacja LCD
  pinMode(LCD_CS, OUTPUT);
  pinMode(LCD_RST, OUTPUT);
  pinMode(LCD_DC, OUTPUT);

  SPI.begin();
  SPI.beginTransaction(SPISettings(8000000, MSBFIRST, SPI_MODE0));

  LCD_SCAN_DIR lcdScanDirection = SCAN_DIR_DFT;
  LCD.LCD_Init(lcdScanDirection);
  LCD.LCD_Clear(BLACK);
  LCD.LCD_DisplayString(0, 6, "SWiM", &Font12, BLACK, YELLOW);
  //LCD.LCD_Show();   // testowanie wyświetlacza

  // Inicjalizacja akcelerometru
  accelSensor.begin();
  accelSensor.setRange(ADXL345_RANGE_4G);
  Serial.begin(9600);
}

// Punkty bazowe kwadratu (współrzędne 3D)
// oryginalne "points" zmienione na "basePoints"
float basePoints[4][3] = {
  {-0.5, -0.5, 0},
  {-0.5,  0.5, 0},
  { 0.5,  0.5, 0},
  { 0.5, -0.5, 0},
};

// Tablica dla obróconych punktów; zmiana nazwy "rotated" -> "rotPoints"
float rotPoints[4][3] = {
  {0, 0, 0},
  {0, 0, 0},
  {0, 0, 0},
  {0, 0, 0},
};

// Stałe transformacji – zmienione nazwy
const int OFFSET_X = 40;
const int OFFSET_Y = 40;
const float SCALE_FACTOR = 60;

// Funkcja rysująca linie pomiędzy punktami
// zmieniona nazwa funkcji: "drawPoints" -> "renderShape"
void renderShape(COLOR col) {
  for (int i = 0; i < 4; ++i) {
    int nextPoint = i + 1;
    if (nextPoint == 4) nextPoint = 0;
    LCD_DrawLine(rotPoints[i][0], rotPoints[i][1],
                 rotPoints[nextPoint][0], rotPoints[nextPoint][1],
                 col, LINE_SOLID, DOT_PIXEL_1X1);
  }
}

// Funkcja obracająca punkty na podstawie kątów
// zmieniona nazwa funkcji: "rotatePoints" -> "applyRotationToPoints"
void applyRotationToPoints(float pitch, float roll, float yaw) {
  // Obliczenia funkcji trygonometrycznych – nazwy zmiennych skrócone
  float cp = cos(pitch);
  float sp = sin(pitch);
  float cr = cos(roll);
  float sr = sin(roll);
  float cy = cos(yaw);
  float sy = sin(yaw);
  
  for (int i = 0; i < 4; ++i) {
    // Rotacja względem osi X (pitch)
    rotPoints[i][0] = basePoints[i][0];
    rotPoints[i][1] = cp * basePoints[i][1] - sp * basePoints[i][2];
    rotPoints[i][2] = sp * basePoints[i][1] + cp * basePoints[i][2];

    // Rotacja względem osi Y (roll)
    float tempVal = cr * rotPoints[i][0] + sr * rotPoints[i][2];
    rotPoints[i][2] = -sr * rotPoints[i][0] + cr * rotPoints[i][2];
    rotPoints[i][0] = tempVal;

    // Rotacja względem osi Z (yaw)
    tempVal = cy * rotPoints[i][0] - sy * rotPoints[i][1];
    rotPoints[i][1] = sy * rotPoints[i][0] + cy * rotPoints[i][1];
    rotPoints[i][0] = tempVal;

    // Skalowanie punktów
    for (int j = 0; j < 3; ++j) {
      rotPoints[i][j] *= SCALE_FACTOR;
    }

    // Translacja punktów (przesunięcie)
    rotPoints[i][0] += OFFSET_X;
    rotPoints[i][1] += OFFSET_Y;
  }
}

// Funkcja rysująca ramkę – zmieniona nazwa: "drawFrame" -> "renderFrame"
void renderFrame(float pitch, float roll, float yaw) {
    // Czyszczenie poprzedniego rysunku
    renderShape(BLACK);
    // Obrót punktów zgodnie z kątami
    applyRotationToPoints(pitch, roll, yaw);
    // Rysowanie nowego rysunku
    renderShape(YELLOW);
}

// Definicje stałych dla bufora tekstowego na LCD – zmienione nazwy
const int NUM_LINES = 3;
const int LCD_LINE_LENGTH = 22;
char lcdBuffer[NUM_LINES][LCD_LINE_LENGTH];
const int LCD_LINE_HEIGHT = 16;
const int LCD_START_Y = 22;

void loop() {
  // Odczyt i filtracja danych z akcelerometru
  Vector rawAccel = accelSensor.readNormalize();
  Vector filteredAccel = accelSensor.lowPassFilter(rawAccel, 0.15);

  Serial.print("ax = ");
  Serial.print(filteredAccel.XAxis);
  Serial.print(", ay = ");
  Serial.print(filteredAccel.YAxis);
  Serial.print(", az = ");
  Serial.print(filteredAccel.ZAxis);

  // Obliczanie kątów (pitch, roll, yaw) w stopniach
  float pitch = atan(filteredAccel.XAxis / sqrt(filteredAccel.YAxis * filteredAccel.YAxis + filteredAccel.ZAxis * filteredAccel.ZAxis)) * 180 / M_PI;
  float roll  = atan(filteredAccel.YAxis / sqrt(filteredAccel.XAxis * filteredAccel.XAxis + filteredAccel.ZAxis * filteredAccel.ZAxis)) * 180 / M_PI;
  float yaw   = atan(filteredAccel.ZAxis / sqrt(filteredAccel.XAxis * filteredAccel.XAxis + filteredAccel.ZAxis * filteredAccel.ZAxis)) * 180 / M_PI;

  Serial.print(", pitch = ");
  Serial.print(pitch);
  Serial.print(", roll = ");
  Serial.print(roll);
  Serial.print(", yaw = ");
  Serial.println(yaw);

  // Rysowanie ramki bazującej na obróconych punktach
  // renderFrame(pitch, roll, yaw);  // odkomentować, gdy rysowanie będzie potrzebne

  // Konwersja kątów do wartości całkowitych
  int intPitch = pitch;
  int intRoll  = roll;
  int intYaw   = yaw;
  
  // Formatowanie wartości akcelerometru do wyświetlenia
  char numBuffer[10];
  dtostrf(filteredAccel.XAxis, 1, 2, numBuffer);
  sprintf(lcdBuffer[0], "x: %d %-6s", intPitch, numBuffer);
  dtostrf(filteredAccel.YAxis, 1, 2, numBuffer);
  sprintf(lcdBuffer[1], "y: %d %-6s", intRoll, numBuffer);
  dtostrf(filteredAccel.ZAxis, 1, 2, numBuffer);
  sprintf(lcdBuffer[2], "z: %d %-6s", intYaw, numBuffer);

  // Wyświetlanie bufora na LCD
  for (int i = 0; i < NUM_LINES; ++i) {
    LCD.LCD_DisplayString(0, LCD_START_Y + i * LCD_LINE_HEIGHT, lcdBuffer[i], &Font12, BLACK, YELLOW);
  }

  delay(100);   // opóźnienie – może być modyfikowane
}
