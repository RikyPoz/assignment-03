#include <Arduino.h>
#include <WiFi.h>
#include <PubSubClient.h>
#include <ArduinoJson.h>
#include "Led.h"
#include <Termo.h>
#define MSG_BUFFER_SIZE 50
#define TERMO_PIN 5
#define REDLED_PIN 9
#define GREENLED_PIN 10

#define IDLE 2
#define START_STATE 1
// wifi network info

const char *ssid = "Home&Life SuperWiFi-42A5";
const char *password = "GKK3PJNLHJ8FHHFX";

// MQTT server address 
const char *mqtt_server = "broker.mqtt-dashboard.com";

// MQTT topic 
const char *topic = "temperatura";



WiFiClient espClient;
PubSubClient client(espClient);
TaskHandle_t termoTask;
TaskHandle_t wifiTask;
TaskHandle_t sendTask;
TaskHandle_t mqttTask;
volatile TickType_t sendPeriod;
volatile int temperature;
volatile int state;
char msg[MSG_BUFFER_SIZE];
// MQTT subscribing callback

void callback(char *topic, byte *payload, unsigned int length) {
  char buffer[MSG_BUFFER_SIZE];
  unsigned int copyLen = (length < (MSG_BUFFER_SIZE - 1)) ? length : (MSG_BUFFER_SIZE - 1);
  memcpy(buffer, payload, copyLen);
  buffer[copyLen] = '\0';

  StaticJsonDocument<128> doc;
  DeserializationError error = deserializeJson(doc, buffer);
  if (error) {
    Serial.print("Errore nel parsing del JSON: ");
    Serial.println(error.f_str());
    return;
  }
  if (doc.containsKey("sendPeriod")) {
    long period_ms = doc["sendPeriod"];
    sendPeriod = pdMS_TO_TICKS(period_ms);
    Serial.print("sendPeriod aggiornato a: ");
    Serial.print(period_ms);
  }
}

void sendTaskcode(void *parameter)
{
  Led redLed(REDLED_PIN);
  Led greenLed(GREENLED_PIN);
  TickType_t xLastWakeTime = xTaskGetTickCount();
  bool connected = false;
  const TickType_t retryPeriod = pdMS_TO_TICKS(5000);

  Serial.print("sendTask is running on core ");
  Serial.println(xPortGetCoreID());
  for (;;) {
    if (connected) {
      if (state == IDLE) {
        Serial.print("Periodo: ");
        Serial.println(sendPeriod);
        xTaskDelayUntil( &xLastWakeTime, sendPeriod);
        snprintf(msg, MSG_BUFFER_SIZE, "{\"temperatura\":%d}", temperature);
        Serial.println(String("Publishing message: ") + msg);
        client.publish(topic, msg);
      } else {
        greenLed.switchOff();
        redLed.switchOn();
        connected = false;
      }
    } else {
      if (state == IDLE) {
        redLed.switchOff();
        greenLed.switchOn();
        connected = true;
      } else {
        xTaskDelayUntil( &xLastWakeTime, retryPeriod);
      }
    }
    
  }
}


void mqttTaskcode(void *parameter) {
  bool connected = false;
  TickType_t xLastWakeTime = xTaskGetTickCount();
  const TickType_t loopPeriod = pdMS_TO_TICKS(100);
  const TickType_t retryPeriod = pdMS_TO_TICKS(5000);

  for (;;) {
    if (connected) {
      if (client.connected()) {
        client.loop();
        xTaskDelayUntil(&xLastWakeTime, loopPeriod);
      } else {
        state--;
        connected = false;
      }
    } else {
      Serial.print("Attempting MQTT connection...");
      // Create a random client ID
      String clientId = String("temp-client") + String(random(0xffff), HEX);
      // Attempt to connect
      if (client.connect(clientId.c_str())) {
        Serial.println("connected");
        client.subscribe(topic);
        state++;
        connected = true;
      } else {
        Serial.print("failed, rc=");
        Serial.print(client.state());
        Serial.println(" try again in 5 seconds");
        xTaskDelayUntil(&xLastWakeTime, retryPeriod);
      }
    }
  }
  
}

void wifiTaskcode(void *parameter) {
  bool connected = true;
  TickType_t xLastWakeTime = xTaskGetTickCount();
  const TickType_t loopPeriod = pdMS_TO_TICKS(100);
  const TickType_t retryPeriod = pdMS_TO_TICKS(500);
  for (;;) {
    if (connected) {
      if (WiFi.status() != WL_CONNECTED) {
        Serial.println("WiFi Disconnected... Reconnect.");
        WiFi.begin(ssid, password);
        Serial.print("Connecting");
        state--;
        connected = false;
      } else {
        xTaskDelayUntil(&xLastWakeTime, loopPeriod);
      }
    } else {
      if (WiFi.status() != WL_CONNECTED) {
        Serial.print(" .");
        xTaskDelayUntil(&xLastWakeTime, retryPeriod);
      } else {
        Serial.println("");
        Serial.print("Connected to WiFi network with IP Address: ");
        Serial.println(WiFi.localIP());
        state++;
        connected = true;
      }
    }
  }
}

int simulaTemperatura(void) {
  static int t = 20, d = 1;
  t += d;
  if(t >= 25) d = -1;
  else if(t <= 15) d = 1;
  return t;
}

void termoTaskcode(void *parameter) {
  Termo termo(TERMO_PIN);
  TickType_t xLastWakeTime = xTaskGetTickCount();
  const TickType_t period = pdMS_TO_TICKS(100);
  for (;;) {
    temperature = simulaTemperatura();
    xTaskDelayUntil(&xLastWakeTime, period);
  }
}

void setup()
{
  Serial.begin(115200);
  WiFi.mode(WIFI_STA);
  randomSeed(micros());
  client.setServer(mqtt_server, 1883);
  client.setCallback(callback);
  sendPeriod = pdMS_TO_TICKS(3000);
  state = START_STATE;
  xTaskCreatePinnedToCore(termoTaskcode, "termo", 2048, NULL, 1, &termoTask, 0);
  xTaskCreatePinnedToCore(wifiTaskcode, "wifi", 4096, NULL, 1, &wifiTask, 1);
  xTaskCreatePinnedToCore(mqttTaskcode, "mqtt", 4096, NULL, 1, &mqttTask, 1);
  xTaskCreatePinnedToCore(sendTaskcode, "send", 4096, NULL, 1, &sendTask, 1);
}

void loop()
{

}
