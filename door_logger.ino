#include <ESP8266WiFi.h>
#include <ESPAsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include <AsyncElegantOTA.h>
#include "Credentials.h"

const char* ssid = DOOR_LOGGER_SSID;
const char* password = DOOR_LOGGER_PASS;


AsyncWebServer server(80);


void setup(void) {
  delay(2000);
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);

  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request) {
    request->send(200, "text/plain", "Hi! This is a sample response.");
  });

  AsyncElegantOTA.begin(&server);    // Start AsyncElegantOTA
  server.begin();
}

void loop(void) {
  
}
