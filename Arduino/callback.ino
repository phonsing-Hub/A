void callback(char* topic, byte* payload, unsigned int length) {
  String payloadStr;
  for (int i = 0; i < length; i++) 
     payloadStr += (char)payload[i];
  Serial.println(payloadStr);
  // Parse JSON
  const size_t capacity = JSON_OBJECT_SIZE(2); // Adjust the size based on your JSON structure
  DynamicJsonDocument doc(capacity);
  deserializeJson(doc, payloadStr);

  if (doc.containsKey("LED")) {
    bool led = doc["LED"];
    if(led){
    digitalWrite(26, LOW);
    Serial.println("LED: on");
    }
    else {
    digitalWrite(26, HIGH);
    Serial.println("LED: off");
    }
    } 

  if (doc.containsKey("Relay1")) {
    bool relay1 = doc["Relay1"];
    if(relay1){
    digitalWrite(32, LOW);
    Buzzer();
    Serial.println("Relay1: on");
    }
    else{
    digitalWrite(32, HIGH);
    Serial.println("Relay1: off");
    }
  } 

    if (doc.containsKey("Relay2")) {
    bool relay2 = doc["Relay2"];
    if(relay2){
    digitalWrite(14, LOW);
    Serial.println("Relay2: on");
    }
    else{
    digitalWrite(14, HIGH);
    Serial.println("Relay2: off");
    }
  } 
}