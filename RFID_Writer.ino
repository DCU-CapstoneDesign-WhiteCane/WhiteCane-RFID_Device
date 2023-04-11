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
  Serial.println("Got from SerialPort");
  Serial.println(buffer);
  Serial.println();

  deserializeJson(doc, buffer);

  // Mode CHK'
  if(doc["mode"] == "w") {
    String location = doc["location"];
    Serial.println("Location: " + location);
    rfid_writer(location);
  }
}

void rfid_writer(String location) {
  
}