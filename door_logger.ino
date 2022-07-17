#include <ESP8266WiFi.h>
#include <ESPAsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include <AsyncElegantOTA.h>
#include <BlynkSimpleEsp8266.h>
#include "Credentials.h"

#define BLYNK_TEMPLATE_ID   DOOR_LOGGER_BLYNK_TEMPLATE_ID
#define BLYNK_DEVICE_NAME   DOOR_LOGGER_BLYNK_DEVICE_NAME
#define BLYNK_AUTH_TOKEN    DOOR_LOGGER_BLYNK_AUTH_TOKEN
char auth[] = BLYNK_AUTH_TOKEN;
const char* ssid = DOOR_LOGGER_SSID;
const char* password = DOOR_LOGGER_PASS;


AsyncWebServer server(80);


void setup(void) {
  delay(2000);
  Blynk.begin(auth, ssid, password, DOOR_LOGGER_BLYNK_ADDRESS, 8080);
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);

  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request) {
    request->send(200, "text/plain", "Hi! This is a sample response.");
  });

  AsyncElegantOTA.begin(&server);    // Start AsyncElegantOTA
  server.begin();
}

byte count = 0;
void loop(void) {
  Blynk.run();
  Blynk.virtualWrite(V0, count++);
  delay(1000);
}
