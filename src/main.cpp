#include <Arduino.h>
#include <Arduino_JSON.h>
#include <SPI.h>
#include <WiFi.h>

#include "Display.h"
#include "HeartRateSensor.h"
#include "InfraredThermometer.h"
#include "SPIFFS.h"
#include "credentials.h"
#include "webserver.h"

JSONVar readings;
// Get Sensor Readings and return JSON object
String getSensorReadings() {
  readings["bpm"] = String(HeartRateSensor::getBeatAvg());
  readings["temp"] = String(InfraredThermometer::readTempC());
  return JSON.stringify(readings);
}

void setup() {
  Serial.begin(115200);
  Serial.println("Initializing...");

  if (InfraredThermometer::setup(0.5) != InfraredThermometer::Code::OK) {
    Serial.println("Error connecting to MLX sensor. Check wiring.");
    while (1)
      ;
  }

  if (HeartRateSensor::setup() != HeartRateSensor::Code::OK) {
    Serial.println("MAX30105 was not found. Please check wiring/power.");
    while (1)
      ;
  }

  if (Display::setup() != Display::Code::OK) {
    Serial.println(F("SSD1306 allocation failed"));
    while (1)
      ;
  }

  Serial.println("Connecting to WiFi...");
  if (WebServer::setup(WIFI_SSID, WIFI_PASSWD, getSensorReadings) !=
      WebServer::Code::OK) {
    Serial.println(F("WebServer init failed"));
    while (1)
      ;
  }
  Serial.printf("IP Address: %s\n", WebServer::getIPAddress().toString());

  Serial.println("Place your index finger on the sensor with steady pressure.");
}

void loop() {
  HeartRateSensor::loop();
  if (!HeartRateSensor::isFingerDetected()) {
    Display::println("---");
  } else {
    int beatAvg = HeartRateSensor::getBeatAvg();
    Display::printHeartBeat(beatAvg);
  }
  WebServer::loop();
}
