#define BLYNK_PRINT Serial
#define BLYNK_TEMPLATE_ID "TMPL662ZlFUAX"
#define BLYNK_TEMPLATE_NAME "CPE345x64028780"
#define BLYNK_AUTH_TOKEN "zYP6hSPpy7XgRA5y8hd7Z2m3kpsgsQeN"

#include <WiFi.h>
#include "time.h"
#include <PubSubClient.h>
#include <ArduinoJson.h> 
#include "Adafruit_SHT31.h"
#include <WiFiClient.h>
#include <BlynkSimpleEsp32.h>

const char* ntpServer = "pool.ntp.org";
const long  gmtOffset_sec = 7 * 3600;
const int   daylightOffset_sec = 0;
const char* ssid = "Polsing";
const char* password = "Polsing2002";

const char* mqtt_server = "broker.emqx.io";
const int mqtt_port = 1883;
const char* mqtt_username = "polsing";
String pss = "APL_" + String(random(1000, 2000));
const char* mqtt_password = pss.c_str();

unsigned long lastAlertTime = 0; // เก็บเวลาที่ทำการเตือนล่าสุด
const unsigned long alertInterval = 7 * 24 * 60 * 60 * 1000;

WiFiClient espClient;
PubSubClient client(espClient);
  
bool enableHeater = false;
uint8_t loopCnt = 0;
Adafruit_SHT31 sht31 = Adafruit_SHT31();

int PWM_FREQUENCY = 5000;
int PWM_CHANNEL0 = 0;
int PWM_RESOUTION = 8;
int BUZZER_PIN = 33;

void setup() {
  Serial.begin(115200);
  while (!Serial)
  delay(10);

  pinMode(26,OUTPUT);
  pinMode(32,OUTPUT);
  pinMode(14,OUTPUT);
  pinMode(BUZZER_PIN,OUTPUT);
  digitalWrite(26,HIGH);
  digitalWrite(32, HIGH);
  digitalWrite(14, HIGH);

  ledcSetup(PWM_CHANNEL0, PWM_FREQUENCY, PWM_RESOUTION);
  ledcAttachPin(BUZZER_PIN, PWM_CHANNEL0);

  if (!sht31.begin(0x44)) {   // Set to 0x45 for alternate i2c addr
    Serial.println("Couldn't find SHT31");
    while (1) delay(1);
  }
  Serial.print("Heater Enabled State: ");
  if (sht31.isHeaterEnabled())Serial.println("ENABLED");
  else Serial.println("DISABLED");
  // Connect to Wi-Fi
  ConnectWifi();
  // Set the MQTT broker and credentials
  client.setServer(mqtt_server, mqtt_port);
  client.setCallback(callback);
  // Connect to MQTT
  ConnectMqtt();
  configTime(gmtOffset_sec, daylightOffset_sec, ntpServer);
  printLocalTime();
  WiFi.disconnect(true);
  WiFi.mode(WIFI_OFF);
  Blynk.begin(BLYNK_AUTH_TOKEN, ssid, password);
  }
float h ,t;
void loop() {
  printLocalTime();
  reConnectMqtt();
  bme();
  //showtime();
  Blynk.virtualWrite(V1, t);  // V1 เป็นหมายเลขของวิดเจ็ตที่คุณต้องการให้แสดงค่าอุณหภูมิ
  Blynk.virtualWrite(V2, h); 
  if(h<=80) digitalWrite(32, LOW);
  else digitalWrite(32, HIGH);

  client.loop();
  Blynk.run();
  delay(1000);
}

// BLYNK_WRITE(V0){
// //h
// }
// BLYNK_WRITE(V1){
//   //t
// }