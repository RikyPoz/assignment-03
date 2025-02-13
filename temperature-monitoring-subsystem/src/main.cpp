#include <Arduino.h>
#include <WiFi.h>
#include <PubSubClient.h>
#include <ArduinoJson.h>
#include "Led.h"
#include <Termo.h>
#define MSG_BUFFER_SIZE 50
#define TERMO_PIN 5
#define REDLED_PIN 9
#define GREENLED_PIN 8

// wifi network info

const char *ssid = "Home&Life SuperWiFi-42A5";
const char *password = "GKK3PJNLHJ8FHHFX";

// MQTT server address 
const char *mqtt_server = "broker.mqtt-dashboard.com";

// MQTT topic 
const char *topic1 = "temperatura";
const char *topic2 = "periodo";


WiFiClient espClient;
PubSubClient client(espClient); 
TaskHandle_t termoTask, wifiTask, sendTask, mqttTask;
volatile TickType_t sendPeriod;
volatile int temperature;
bool wifiConn, mqttConn;
char msg[MSG_BUFFER_SIZE];


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
  if (((String) topic).equals(topic2) && doc.containsKey("period")) {
    long period_ms = doc["period"];
    sendPeriod = pdMS_TO_TICKS(period_ms);
    Serial.print("sendPeriod aggiornato a: ");
    Serial.print(period_ms);
  }
}

void sendTaskcode(void *parameter)
{
  enum State { WAIT, SEND, DISCONNECTED };
  const TickType_t RETRY_PERIOD = pdMS_TO_TICKS(5000);
  const TickType_t WAIT_PERIOD = pdMS_TO_TICKS(100);
  Led redLed(REDLED_PIN);
  Led greenLed(GREENLED_PIN);
  TickType_t xLastWakeTime = xTaskGetTickCount();
  State state = DISCONNECTED;
  TickType_t timeElapsed = 0;

  Serial.print("sendTask is running on core ");
  Serial.println(xPortGetCoreID());
  for (;;) {
    switch (state) {
      case DISCONNECTED :
        if (wifiConn && mqttConn) {
          redLed.switchOff();
          greenLed.switchOn();
          state = SEND;
        } else {
          xTaskDelayUntil( &xLastWakeTime, RETRY_PERIOD);
        }
      break;
      case SEND :
        if (wifiConn && mqttConn) {
          Serial.print("Periodo: ");
          Serial.println(sendPeriod);
          snprintf(msg, MSG_BUFFER_SIZE, "{\"temperatura\":%d}", temperature);
          Serial.println(String("Publishing message: ") + msg);
          client.publish(topic1, msg);
          timeElapsed = 0;
          state = WAIT;
        } else {
          greenLed.switchOff();
          redLed.switchOn();
          state = DISCONNECTED;
        }
      break;
      case WAIT :
        xTaskDelayUntil( &xLastWakeTime, WAIT_PERIOD);
        if (timeElapsed >= sendPeriod) {
          state = SEND;
        } else {
          timeElapsed += WAIT_PERIOD;
        }
    }
  }
}


void mqttTaskcode(void *parameter) {
  TickType_t xLastWakeTime = xTaskGetTickCount();
  const TickType_t LOOP_PERIOD = pdMS_TO_TICKS(100);
  const TickType_t RETRY_PERIOD = pdMS_TO_TICKS(5000);

  for (;;) {
    if (mqttConn) {
      if (client.connected()) {
        client.loop();
        xTaskDelayUntil(&xLastWakeTime, LOOP_PERIOD);
      } else {
        mqttConn = false;
      }
    } else {
      Serial.print("Attempting MQTT connection...");
      // Create a random client ID
      String clientId = String("temp-client") + String(random(0xffff), HEX);
      // Attempt to connect
      if (client.connect(clientId.c_str())) {
        Serial.println("connected");
        client.subscribe(topic1);
        client.subscribe(topic2);
        mqttConn = true;
      } else {
        Serial.print("failed, rc=");
        Serial.print(client.state());
        Serial.println(" try again in 5 seconds");
        xTaskDelayUntil(&xLastWakeTime, RETRY_PERIOD);
      }
    }
  }
  
}

void wifiTaskcode(void *parameter) {
  TickType_t xLastWakeTime = xTaskGetTickCount();
  const TickType_t LOOP_PERIOD = pdMS_TO_TICKS(100);
  const TickType_t RETRY_PERIOD = pdMS_TO_TICKS(500);
  for (;;) {
    if (wifiConn) {
      if (WiFi.status() != WL_CONNECTED) {
        Serial.println("WiFi Disconnected... Reconnect.");
        WiFi.begin(ssid, password);
        Serial.print("Connecting");
        wifiConn = false;
      } else {
        xTaskDelayUntil(&xLastWakeTime, LOOP_PERIOD);
      }
    } else {
      if (WiFi.status() != WL_CONNECTED) {
        Serial.print(" .");
        xTaskDelayUntil(&xLastWakeTime, RETRY_PERIOD);
      } else {
        Serial.println("");
        Serial.print("Connected to WiFi network with IP Address: ");
        Serial.println(WiFi.localIP());
        wifiConn = true;
      }
    }
  }
}

void termoTaskcode(void *parameter) {
  Termo termo(TERMO_PIN);
  TickType_t xLastWakeTime = xTaskGetTickCount();
  const TickType_t period = pdMS_TO_TICKS(100);
  for (;;) {
    temperature = termo.detectValue();
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
  wifiConn = true;
  mqttConn = false;
  xTaskCreatePinnedToCore(termoTaskcode, "termo", 2048, NULL, 1, &termoTask, 0);
  xTaskCreatePinnedToCore(wifiTaskcode, "wifi", 4096, NULL, 1, &wifiTask, 1);
  xTaskCreatePinnedToCore(mqttTaskcode, "mqtt", 4096, NULL, 1, &mqttTask, 1);
  xTaskCreatePinnedToCore(sendTaskcode, "send", 4096, NULL, 1, &sendTask, 1);
}

void loop() {}