#include <WiFi.h>
#include <ArduinoOTA.h>
#include <key.h>

const char* ssid = SSID;
const char* password = PASSWORD;

void setup() {
  Serial.begin(115200);
  setupWiFi();  
  setupOTA();  
  pinMode(2, OUTPUT);
}

void loop() {
  ArduinoOTA.handle();  
  
}

void setupWiFi() {
  Serial.print("Connecting to WiFi: ");
  Serial.println(ssid);

  WiFi.begin(ssid, password);

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
  ArduinoOTA.setPassword("atos593");
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
