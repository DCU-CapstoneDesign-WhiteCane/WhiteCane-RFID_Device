// RFID TAG 읽기
void rfid_reader() {
  StaticJsonDocument<64> doc;   // JSON 객체 생성

  // 키 준비 (초기 세팅은 FFFFFFFFFFFFh로 되어 있음)
  MFRC522::MIFARE_Key key;
  for (byte i = 0; i < 6; i++) {
    key.keyByte[i] = 0xFF;
  }

  byte block;
  byte len;
  MFRC522::StatusCode status;

  if (! mfrc522.PICC_IsNewCardPresent()) { // 새 TAG 찾기
    return;
  }

  if (! mfrc522.PICC_ReadCardSerial()) {   // TAG 중 하나 선택
    return;
  }

  // mfrc522.PICC_DumpDetailsToSerial(&(mfrc522.uid)); // TAG에 대한 세부 정보 출력

  byte buffer[18];
  block = 1;
  len = 18;

  status = mfrc522.PCD_Authenticate(MFRC522::PICC_CMD_MF_AUTH_KEY_A, 1, &key, &(mfrc522.uid));
  if (status != MFRC522::STATUS_OK) {
    Serial.print("Authentication failed: ");
    Serial.println(mfrc522.GetStatusCodeName(status));
    return;
  }

  status = mfrc522.MIFARE_Read(block, buffer, &len);
  if (status != MFRC522::STATUS_OK) {
    Serial.print("Reading failed: ");
    Serial.println(mfrc522.GetStatusCodeName(status));
    return;
  }

  // TAG에 저장된 장소 확인
  String buffer_str;
  for (uint8_t i = 0; i < 16; i++) {
    if (buffer[i] != 32)  { // 32가 공백을 의미
      continue;
    } else {
      buffer[i] = '\0';
      buffer_str = String((char*)buffer);

      // Serial.print("Location : ");
      // Serial.println(buffer_str);

      // JSON 데이터 관리자 앱으로 전송
      doc["mode"] = "r";
      doc["location"] = buffer_str;
      serializeJson(doc, Serial);
      Serial.write("#");
      break;
    }
  }
  Serial.println();

  delay(1000); // 다음 TAG를 읽기 위해 필요한 대기 시간

  mfrc522.PICC_HaltA();
  mfrc522.PCD_StopCrypto1();
}