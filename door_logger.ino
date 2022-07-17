#include <ESP8266WiFi.h>
#include <ESPAsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include <AsyncElegantOTA.h>
#include <BlynkSimpleEsp8266.h>
#include "Credentials.h"

#define sensor_gpio     3
#define green_led_gpio  1
#define white_led_gpio  0
#define buzzer_gpio     2

#define BLYNK_TEMPLATE_ID   DOOR_LOGGER_BLYNK_TEMPLATE_ID
#define BLYNK_DEVICE_NAME   DOOR_LOGGER_BLYNK_DEVICE_NAME
#define BLYNK_AUTH_TOKEN    DOOR_LOGGER_BLYNK_AUTH_TOKEN
char auth[] = BLYNK_AUTH_TOKEN;
const char* ssid = DOOR_LOGGER_SSID;
const char* password = DOOR_LOGGER_PASS;


AsyncWebServer server(80);

// reconnect wifi if connection drop
#define WIFI_RECONNECT_INTERVAL 20000
#define GREEN_LED_BREATH_INTERVAL 5
#define GREEN_LED_BLINK_INTERVAL 1000
unsigned long last_reconnection = 0;
unsigned long last_led_update = 0;
byte green_led_brightness = 0;

void wifi_agent_loop(){
  if(WiFi.status() != WL_CONNECTED){
    if(millis() - last_reconnection >= WIFI_RECONNECT_INTERVAL){
      last_reconnection = millis();
      WiFi.disconnect();
      WiFi.reconnect();
    }
    //green led blink
    if(millis() - last_led_update>= GREEN_LED_BLINK_INTERVAL){
      last_led_update = millis();
      if(green_led_brightness != 0) green_led_brightness = 0;
      else                          green_led_brightness = 255;
      analogWrite(green_led_gpio, green_led_brightness);
    }
  }else{
    // green led breath
    if(millis() - last_led_update>= GREEN_LED_BREATH_INTERVAL){
      last_led_update = millis();
      analogWrite(green_led_gpio, green_led_brightness++);
    }
  } 
}

void setup(void) {
  pinMode(sensor_gpio, INPUT);
  pinMode(green_led_gpio, OUTPUT); digitalWrite(green_led_gpio, HIGH); //turn led off
  pinMode(white_led_gpio, OUTPUT); digitalWrite(white_led_gpio, HIGH); //turn led off
  pinMode(buzzer_gpio, OUTPUT);    digitalWrite(buzzer_gpio, HIGH); //turn buzzer off
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  WiFi.hostname("door_notifier");
  // Wait for connection
  while (WiFi.status() != WL_CONNECTED) {
    delay(200);
    digitalWrite(green_led_gpio, !digitalRead(green_led_gpio));
  }
  

  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request) {
    request->send(200, "text/plain", "Hi! This is a sample response from door_notifier.");
  });

  AsyncElegantOTA.begin(&server);    // Start AsyncElegantOTA
  server.begin();
  Blynk.config(auth, DOOR_LOGGER_BLYNK_ADDRESS, 8080);
  //Blynk.begin(auth, ssid, password, DOOR_LOGGER_BLYNK_ADDRESS, 8080);
}

void loop(void) {
  Blynk.run();
  wifi_agent_loop();
}
