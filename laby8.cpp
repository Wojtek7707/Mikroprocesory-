// SWiM: Lab08
#include <MFRC522.h>
#include <stdio.h>

MFRC522 rfid(10, 9);
MFRC522::MIFARE_Key key;

const int BUZZ_PIN = A5;
const int LEDR_PIN = 2;
const int LEDG_PIN = 3;

void setup() {
  // put your setup code here, to run once:
  pinMode(LEDR_PIN, OUTPUT);
  pinMode(LEDG_PIN, OUTPUT);

  SPI.begin();
  rfid.PCD_Init();
  Serial.begin(9600);
  Serial.println("Started!");
}

const int KEY_SIZE = 4;
uint8_t read_key[KEY_SIZE];
char key_repr_buffer[KEY_SIZE*2 + 1];
bool new_key = false;

uint8_t our_key[KEY_SIZE] = {0x01, 0x02, 0x03, 0x04};

bool key_match() {
  bool matches = true;
  for (int i=0; i<KEY_SIZE; ++i) {
    if (read_key[i] != our_key[i]) {
      matches = false;
      break;
    }
  }

  return matches;
}

const int INFO_TIME = 1000;
unsigned long timeout = 0;
bool timeouted = false;

void loop() {
  if (rfid.PICC_IsNewCardPresent() && rfid.PICC_ReadCardSerial()) {
    for (int i=0; i<KEY_SIZE; ++i) {
      read_key[i] = rfid.uid.uidByte[i];
    }
    for (int i=0; i<KEY_SIZE; ++i) {
      sprintf(key_repr_buffer + i*2, "%02x", read_key[i]);
    }
    Serial.print("New key: ");
    Serial.println(key_repr_buffer);
    new_key = true;
  }

  if (new_key) {
    new_key = false;
    if (key_match()) {
      digitalWrite(LEDG_PIN, HIGH);
      tone(BUZZ_PIN, 900);
    } else {
      digitalWrite(LEDR_PIN, HIGH);
      tone(BUZZ_PIN, 200);
    }

    timeout = millis() + INFO_TIME;
    timeouted = false;
  }

  if (!timeouted && timeout < millis()) {
    noTone(BUZZ_PIN);
    digitalWrite(LEDR_PIN, LOW);
    digitalWrite(LEDG_PIN, LOW);
    timeouted = true;
  }
}
