// RFID TAG 읽기
void rfid_reader(MFRC522::MIFARE_Key key) {
  StaticJsonDocument<64> doc;   // JSON 객체 생성
  MFRC522::StatusCode status; // 상태코드 입력 변수
  // mfrc522.PICC_DumpDetailsToSerial(&(mfrc522.uid)); // TAG에 대한 세부 정보 출력

  byte len = 18;
  byte buffer[32];
  byte buffer1[18];
  byte buffer2[18];

  // 첫 번째 블럭 읽기
  byte block = 1;

  // 인증
  status = mfrc522.PCD_Authenticate(MFRC522::PICC_CMD_MF_AUTH_KEY_A, block, &key, &(mfrc522.uid));
  if (status != MFRC522::STATUS_OK) {
    Serial.print("Authentication failed: ");
    Serial.println(mfrc522.GetStatusCodeName(status));
    return;
  }

  // 데이터 읽기
  status = mfrc522.MIFARE_Read(block, buffer1, &len);
  if (status != MFRC522::STATUS_OK) {
    Serial.print("Reading failed: ");
    Serial.println(mfrc522.GetStatusCodeName(status));
    return;
  }

  // 두 번째 블럭 읽기
  block = 2;

  // 인증
  status = mfrc522.PCD_Authenticate(MFRC522::PICC_CMD_MF_AUTH_KEY_A, block, &key, &(mfrc522.uid));
  if (status != MFRC522::STATUS_OK) {
    Serial.print("Authentication failed: ");
    Serial.println(mfrc522.GetStatusCodeName(status));
    return;
  }

  // 데이터 읽기
  status = mfrc522.MIFARE_Read(block, buffer2, &len);
  if (status != MFRC522::STATUS_OK) {
    Serial.print("Reading failed: ");
    Serial.println(mfrc522.GetStatusCodeName(status));
    return;
  }
  
  // 버퍼 1, 2 합치기
  for(byte i = 0; i <= 31; i++) {
    if(i < 16) {
      buffer[i] = buffer1[i];
    } else {
      buffer[i] = buffer2[i-16];
    }
  }

  // TAG에 저장된 장소 확인
  String buffer_str;
  for (uint8_t i = 0; i < 31; i++) {
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
}