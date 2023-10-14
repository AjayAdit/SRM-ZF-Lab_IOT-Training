#include <ArduinoJson.h>
#include <ArduinoJson.hpp>
#include <WiFi.h>
#include <PubSubClient.h>
#include <Wire.h>

#include "EmonLib.h"             // Include Emon Library
#define VOLT_CAL 125
EnergyMonitor emon1;             // Create an instance



const char* ssid = "K5000A23";
const char* password = "12345678";
const char* mqtt_server = "192.168.225.245";

WiFiClient espClient;
PubSubClient client(espClient);
unsigned long lastMsg = 0;
#define MSG_BUFFER_SIZE	(50)
char msg[MSG_BUFFER_SIZE];
int value = 0;

void setup_wifi() {

  delay(100);
  // We start by connecting to a WiFi network
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);

  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  randomSeed(micros());

  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
}

void callback(char* topic, byte* payload, unsigned int length) {

}

void reconnect() {
  // Loop until we're reconnected
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    // Create a random client ID
    String clientId = "ESP32Client-";
    clientId += String(random(0xffff), HEX);
    // Attempt to connect
    if (client.connect(clientId.c_str())) {
      Serial.println("connected");
      // Once connected, publish an announcement...
      client.publish("outTopic", "hello world");
      // ... and resubscribe
      client.subscribe("inTopic");
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      // Wait 5 seconds before retrying
      delay(5000);
    }
  }
}

void setup() {
  Serial.begin(115200);
  
  

  // Set up oversampling and filter initialization
  emon1.voltage(35, VOLT_CAL, 1.7);  // Voltage: input pin, calibration, phase_shift

  setup_wifi();
  client.setServer(mqtt_server, 1883);
  client.setCallback(callback);
}

void Voltage_Measurement()
{
 emon1.calcVI(25,1000);         // Calculate all. No.of half wavelengths (crossings), time-out
  
  float supplyVoltage   = emon1.Vrms;             //extract Vrms into Variable
  if(supplyVoltage >= 100)
  {
 Serial.print("Voltage : ");  
 Serial.println(supplyVoltage);
  }
  else
 {
   Serial.println("Power OFF");
  }


}

void loop() {
Voltage_Measurement();
float supplyVoltage   = emon1.Vrms;  
  if (!client.connected()) {
    reconnect();
  }
  client.loop();
  StaticJsonDocument<32> doc;
  char output[55];

  long now = millis();
  if (now - lastMsg > 5000) {
    lastMsg = now;
    float Volts = supplyVoltage;
    
    doc["V"] = Volts;
    Serial.println("Read");

    serializeJson(doc, output);
    Serial.println(output);
    client.publish("/home/sensors", output);
    Serial.println("Sent");
  }
    
}

