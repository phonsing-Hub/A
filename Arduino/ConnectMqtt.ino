void ConnectMqtt(){
    Serial.println("");
  while (!client.connected()){
    Serial.print("Connecting to MQTT: ");
    Serial.println(mqtt_server);
  if (client.connect("mqttx_5c5c2f6c", mqtt_username, mqtt_password)) {
      Serial.println("Connected to mqtt successfully");
      client.subscribe("64028780/Msg"); 
    } else {
      Serial.print("Failed with state ");
      Serial.print(client.state());
      Serial.println("");
      delay(2000);
    }
  }
}

void reConnectMqtt(){
  if (!client.connected()) {
      Serial.print("Connecting to MQTT: ");
      Serial.println(mqtt_server);
  if (client.connect("mqttx_5c5c2f6c", mqtt_username, mqtt_password)) {
      Serial.println("Connected to mqtt successfully");
      client.subscribe("64028780/Msg"); 
    } else {
      Serial.print("Failed with state ");
      Serial.print(client.state());
      Serial.println("");
      delay(2000);
    }
  }
}