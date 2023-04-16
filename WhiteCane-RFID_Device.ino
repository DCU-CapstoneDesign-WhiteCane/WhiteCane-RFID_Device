#include <SPI.h>
#include <MFRC522.h>
#include <ArduinoJson.h>

#define RST 9  // RFID RST 핀번호 지정
#define SDA 18 // RFID SDA 핀번호 지정

MFRC522 mfrc522(SDA, RST); // MFRC522 객체 생성

int mode = 0; // 0: Read, 1: Write
String data = ""; // 장소 데이터 저장

void setup() {
  Serial.begin(9600); // Initialize serial communications with the PC
  SPI.begin();        // Init SPI bus
  mfrc522.PCD_Init(); // Init MRFC522 card
}

void loop() {
  // data from App
  if(Serial.available()) {
    process_message();
  }

  // RFID
  // 키 준비 (초기 세팅은 FFFFFFFFFFFFh로 되어 있음)
  MFRC522::MIFARE_Key key;
  for (byte i = 0; i < 6; i++) {
    key.keyByte[i] = 0xFF;
  }

  // 새 TAG 찾기
  if (! mfrc522.PICC_IsNewCardPresent()) {
    return;
  }

  // ID 값이 정상인지 CHK'
  if (! mfrc522.PICC_ReadCardSerial()) {
    return;
  }

  /*
  // RFID Tag의 UID 출력
  Serial.print("Card UID:");
  for (byte i = 0; i < mfrc522.uid.size; i++) {
    Serial.print(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " ");
    Serial.print(mfrc522.uid.uidByte[i], HEX);
  }
  // PICC 카드의 Type을 출력
  Serial.print(" PICC type: "); // Dump PICC type
  MFRC522::PICC_Type piccType = mfrc522.PICC_GetType(mfrc522.uid.sak);
  Serial.println(mfrc522.PICC_GetTypeName(piccType));
  */

  if(mode == 0) {
    rfid_reader(key);
  } else {
    rfid_writer(key, data);
  }

  Serial.println();
  mfrc522.PICC_HaltA();
  mfrc522.PCD_StopCrypto1();
}

void process_message() {
  StaticJsonDocument<64> doc;
  String buffer;

  while(Serial.available()) {
    char data = Serial.read();
    if(data == '\n') { // read just 1 line
      break;
    } else {
      buffer = buffer + data;
    }
    delay(10);
  }
  Serial.println();
  Serial.println("Got from SerialPort");
  Serial.println(buffer);

  deserializeJson(doc, buffer);

  // mode and data CHK'
  if(doc["mode"] == "w") {
    String location = doc["location"];
    data = location;
    mode = 1; // 쓰기 모드로 변경
  }
}