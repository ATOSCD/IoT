#include <WiFi.h>
#include <ArduinoOTA.h>
#include <WebSocketsClient.h>
#include <ArduinoJson.h>
#include <key.h>
#include <ESP32Servo.h>

#define SERVO_PIN 13
// key.h 에서 wifi, address 바꾸기

WebSocketsClient webSocket;
Servo light_servo;

void setup() {
  light_servo.attach(SERVO_PIN);
  light_servo.write(90); 
  Serial.begin(115200);
  setupWiFi();  
  setupOTA();  
  setupWebSocket();
}

void loop() {
  ArduinoOTA.handle(); 
  webSocket.loop(); 
}

void setupWiFi() {
  Serial.print("Connecting to WiFi: ");
  Serial.println(SSID);

  WiFi.begin(SSID, PASSWORD);

  int retries = 0;
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
    if (++retries > 30) {
      Serial.println("\nWiFi connection failed!");
      return;
    }
  }

  Serial.println("\nWiFi connected!");
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());
}

void setupOTA() {
  ArduinoOTA.setHostname("ATOSCD-ESP32");
  ArduinoOTA.setPassword(OTAPASS);
  ArduinoOTA
    .onStart([]() {
      Serial.println("OTA Start");
    })
    .onEnd([]() {
      Serial.println("\nOTA End");
    })
    .onProgress([](unsigned int progress, unsigned int total) {
      Serial.printf("OTA Progress: %u%%\r", (progress / (total / 100)));
    })
    .onError([](ota_error_t error) {
      Serial.printf("OTA Error[%u]: ", error);
    });

  ArduinoOTA.begin();
  Serial.println("OTA ready");
}

void webSocketEvent(WStype_t type, uint8_t* payload, size_t length);

void setupWebSocket() {
  webSocket.begin(ADDRESS, PORT, "/ws/iot-light");
  webSocket.onEvent(webSocketEvent);
}

void webSocketEvent(WStype_t type, uint8_t* payload, size_t length) {
  switch (type) {
    case WStype_CONNECTED:
      Serial.println("Connected to WebSocket server");
      //webSocket.sendTXT("{\"iot_id\":\"arduino\", \"message\":\"Hello from Arduino\"}");
      break;

    case WStype_DISCONNECTED:
      Serial.println("Disconnected from WebSocket server");
      break;

    case WStype_TEXT: {
      Serial.printf("Received message: %s\n", payload);

      // JSON 파싱
      StaticJsonDocument<512> doc;
      DeserializationError error = deserializeJson(doc, payload);

      if (error) {
        Serial.print("JSON parse error: ");
        Serial.println(error.c_str());
        return;
      }

      const char* iot_id = doc["iot_id"];
      const char* message = doc["message"];

      if(strcmp(iot_id, "light") == 0){
        if(strcmp(message, "on") == 0){
          light_servo.write(60); // 나중에 각도 바꾸기
        }
        else if(strcmp(message, "off") == 0){
          light_servo.write(120);
        }
      }
      break;
    }
    default:
      break;
  }
}
