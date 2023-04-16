void rfid_writer(MFRC522::MIFARE_Key key, String data) {
  MFRC522::StatusCode status; // 상태 코드 입력 변수

  byte buffer[data.length() + 1];
  data.getBytes(buffer, data.length() + 1);
  int size = sizeof(buffer);

  for(byte block = 1; block < 3; block++) {
    // PCD 인증
    status = mfrc522.PCD_Authenticate(MFRC522::PICC_CMD_MF_AUTH_KEY_B, block, &key, &(mfrc522.uid));
    if (status != MFRC522::STATUS_OK) {
      // Serial.print("PCD_Authenticate() failed: ");
      // Serial.println(mfrc522.GetStatusCodeName(status));
      return;
    } else {
      // Serial.println("PCD_Authenticate() success: ");
    }

    // Write block
    status = mfrc522.MIFARE_Write(block, &buffer[block * 16 - 16], 16);
    if (status != MFRC522::STATUS_OK) {
      // Serial.print("MIFARE_Write() failed: ");
      // Serial.println(mfrc522.GetStatusCodeName(status));
      return;
    } else {
      // Serial.println("MIFARE_Write() success: ");
    }
  }

  mode = 0; // 읽기 모드로 변경
}