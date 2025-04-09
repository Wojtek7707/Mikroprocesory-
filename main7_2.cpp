#include <SPI.h>
#include "LCD.h"
#include "DEV_Config.h"
#include "ADXL345.h"
#include <math.h>

#define PI 3.14159265358979323846

LCD_SCAN_DIR Lcd_ScanDir = SCAN_DIR_DFT;
ADXL345 accelerometer;

void setup() {
  pinMode(LCD_CS, OUTPUT);
  pinMode(LCD_RST, OUTPUT);
  pinMode(LCD_DC, OUTPUT);

  SPI.beginTransaction(SPISettings(8000000, MSBFIRST, SPI_MODE0));
  SPI.begin();

  LCD.LCD_Init(Lcd_ScanDir);
  LCD.LCD_Clear(BLACK); // Jednorazowe czyszczenie

  accelerometer.begin();
  accelerometer.setRange(ADXL345_RANGE_4G);

  Serial.begin(9600);
}

void loop() {
  Vector norm = accelerometer.readNormalize();
  Vector filtered = accelerometer.lowPassFilter(norm, 0.15);

  float ax = filtered.XAxis;
  float ay = filtered.YAxis;
  float az = filtered.ZAxis;

  float pitch = atan(ax / (ay + az)) * 180.0 / PI;
  float roll  = atan(ay / sqrt(ax * ax + az * az)) * 180.0 / PI;
  float yaw   = atan(az / sqrt(ax * ax + az * az)) * 180.0 / PI;

  // Bufory dla konwersji wartości numerycznych na tekst
  char axStr[10], ayStr[10], azStr[10], pitchStr[10], rollStr[10], yawStr[10];
  // Bufory dla pełnych linii wyświetlanego tekstu
  char line1[32], line2[32], line3[32], line4[32], line5[32], line6[32];
  // Konwersja wartości zmiennoprzecinkowych na tekst z dwoma miejscami po przecinku
  dtostrf(ax, 4, 2, axStr);
  dtostrf(ay, 4, 2, ayStr);
  dtostrf(az, 4, 2, azStr);
  dtostrf(pitch, 4, 2, pitchStr);
  dtostrf(roll, 4, 2, rollStr);
  dtostrf(yaw, 4, 2, yawStr);
  
  // Budowanie łańcuchów znaków dla kolejnych linii
  strcpy(line1, "ax: ");
  strcat(line1, axStr);
  
  strcpy(line2, "ay: ");
  strcat(line2, ayStr);
  
  strcpy(line3, "az: ");
  strcat(line3, azStr);
  
  strcpy(line4, "pitch: ");
  strcat(line4, pitchStr);
  
  strcpy(line5, "roll: ");
  strcat(line5, rollStr);
  
  strcpy(line6, "yaw: ");
  strcat(line6, yawStr);

  LCD.LCD_DisplayString(0,  0, line1, &Font12, BLACK, (ax < 0 ? RED : GREEN));
  LCD.LCD_DisplayString(0, 16, line2, &Font12, BLACK, (ay < 0 ? RED : GREEN));
  LCD.LCD_DisplayString(0, 32, line3, &Font12, BLACK, (az < 0 ? RED : GREEN));
  LCD.LCD_DisplayString(0, 48, line4, &Font12, BLACK, (pitch < 0 ? RED : GREEN));
  LCD.LCD_DisplayString(0, 64, line5, &Font12, BLACK, (roll < 0 ? RED : GREEN));
  LCD.LCD_DisplayString(0, 80, line6, &Font12, BLACK, (yaw < 0 ? RED : GREEN));


  Serial.print("ax = ");
  Serial.print(ax);
  Serial.print(", ay = ");
  Serial.print(ay);
  Serial.print(", az = ");
  Serial.println(az);

  delay(200);
}
