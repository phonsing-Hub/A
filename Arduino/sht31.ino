void bme() {
 h = sht31.readHumidity();
 t = sht31.readTemperature();

  unsigned long currentTime = millis(); // เวลาปัจจุบันใน millisecond
  unsigned long timeUntilAlert = alertInterval - (currentTime - lastAlertTime); // คำนวณเวลาที่เหลือจนถึงการเตือน

  // ตรวจสอบว่าเหลือเวลากี่วัน และเวลาเป็นชั่วโมงและนาที ถึงจะถึงเวลาเตือน
  int daysUntilAlert = timeUntilAlert / (24 * 60 * 60 * 1000);
  int hoursUntilAlert = (timeUntilAlert % (24 * 60 * 60 * 1000)) / (60 * 60 * 1000);
  int minutesUntilAlert = (timeUntilAlert % (60 * 60 * 1000)) / (60 * 1000);

if(timeUntilAlert <= 0){
  Buzzer();
}
  if (!isnan(t)) {
    Serial.print("Temp *C = ");
    Serial.print(t);
    Serial.print("\t\t");
  } else Serial.println("Failed to read temperature");
  

  if (!isnan(h)) {
    Serial.print("Hum. % = ");
    Serial.println(h);
  } else Serial.println("Failed to read humidity");

  delay(1000);
  if (loopCnt >= 30) {
    enableHeater = !enableHeater;
    sht31.heater(enableHeater);
    Serial.print("Heater Enabled State: ");
    if (sht31.isHeaterEnabled())
      Serial.println("ENABLED");
    else
      Serial.println("DISABLED");

    loopCnt = 0;
  }
  loopCnt++;


  // สร้าง JSON และส่งผ่านทาง MQTT
  DynamicJsonDocument jsonDoc(200);
  jsonDoc["humidity"] = h;
  jsonDoc["temperature"] = t;
  jsonDoc["daysUntilAlert"] = daysUntilAlert;
  jsonDoc["hoursUntilAlert"] = hoursUntilAlert;
  jsonDoc["minutesUntilAlert"] = minutesUntilAlert;

  String jsonString;
  serializeJson(jsonDoc, jsonString);
  client.publish("64028780/data", jsonString.c_str());
}
