#ifdef ESP8266
#include <ESP8266WiFi.h>
#elif defined(ESP32)
#include <WiFi.h>

#include <DHT.h>

#else
#error "Board not found"
#endif

#include <PubSubClient.h>

//Relays for switching appliances
#define Relay1            15
#define DHTPIN            26

#define DHTTYPE DHT11
DHT dht(DHTPIN, DHTTYPE);


char str_hum_data[10];
char str_temp_data[10];

// Update these with values suitable for your network.

const char* ssid = "K5000A23";
const char* password = "12345678";
const char* mqtt_server = "192.168.149.245"; // Local IP address of Raspberry Pi

const char* username = "goodboy";
const char* pass = "badboy";


// Subscribed Topics

#define sub1 "device1/relay1"

WiFiClient espClient;
PubSubClient client(espClient);
unsigned long lastMsg = 0;
#define MSG_BUFFER_SIZE	(50)
char msg[MSG_BUFFER_SIZE];
int value = 0;


// Connecting to WiFi Router

void setup_wifi()
{

  delay(10);
  // We start by connecting to a WiFi network
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);

  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(50);
    Serial.print(".");
  }

  randomSeed(micros());

  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
}

void callback(char* topic, byte* payload, unsigned int length)
{
  Serial.print("Message arrived [");
  Serial.print(topic);
  Serial.print("] ");


  if (strstr(topic, sub1))
  {
    for (int i = 0; i < length; i++)
    {
      Serial.print((char)payload[i]);
    }
    Serial.println();
    // Switch on the LED if an 1 was received as first character
    if ((char)payload[0] == '1')
    {
      digitalWrite(Relay1, HIGH);   // Turn the LED on (Note that LOW is the voltage level
      // but actually the LED is on; this is because
      // it is active low on the ESP-01)
    } else {
      digitalWrite(Relay1, LOW);  // Turn the LED off by making the voltage HIGH
    }
  }

  else
  {
    Serial.println("unsubscribed topic");
  }

}


// Connecting to MQTT broker

void reconnect()
{
  // Loop until we're reconnected
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    // Create a random client ID
    String clientId = "ESP8266Client-";
    clientId += String(random(0xffff), HEX);
    // Attempt to connect
    if (client.connect(clientId.c_str() , username, pass)) {
      Serial.println("connected");
      // Once connected, publish an announcement...
      client.publish("outTopic", "hello world");
      // ... and resubscribe
      client.subscribe(sub1);
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      // Wait 5 seconds before retrying
      delay(500);
    }
  }
}



void setup()
{

  pinMode(Relay1, OUTPUT);

  Serial.begin(115200);

  setup_wifi();

  client.setServer(mqtt_server, 1883);
  client.setCallback(callback);
}

void loop()
{

  if (!client.connected()) 
  {
    reconnect();
  }
  client.loop();

    unsigned long now = millis();
  if (now - lastMsg > 200) {
    float hum_data = dht.readHumidity();
    Serial.println(hum_data);
    /* 4 is mininum width, 2 is precision; float value is copied onto str_sensor*/
    dtostrf(hum_data, 4, 2, str_hum_data);
    float temp_data = dht.readTemperature(); // or dht.readTemperature(true) for Fahrenheit
    dtostrf(temp_data, 4, 2, str_temp_data);
    lastMsg = now;
    Serial.print("Publish message: ");
    Serial.print("Temperature - "); Serial.println(str_temp_data);
    client.publish("device1/temp", str_temp_data);
    Serial.print("Humidity - "); Serial.println(str_hum_data);
    client.publish("device1/hum", str_hum_data);
  }


}
