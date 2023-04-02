#include <SPI.h>
#include <MFRC522.h>
#include <ArduinoJson.h>

#define RST 9  // RFID RST 핀번호 지정
#define SDA 18 // RFID SDA 핀번호 지정

MFRC522 mfrc522(SDA, RST); // MFRC522 객체 생성

void setup() {
  Serial.begin(9600); // Initialize serial communications with the PC
  SPI.begin();        // Init SPI bus
  mfrc522.PCD_Init(); // Init MRFC522 card
}

void loop() {
  rfid_reader(); // RFID TAG 읽기
}