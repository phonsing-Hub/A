void ConnectWifi(){
  Serial.println("Connecting to WiFi ");
  WiFi.begin(ssid, password,6);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("Connected successfully");
}
