#include <Arduino.h>
#include <SPI.h>

#include "HeartRateSensor.h"
#include "SSD1306Display.h"

void setup() {
  Serial.begin(115200);
  Serial.println("Initializing...");

  if (HeartRateSensor::setup() != HeartRateSensor::Code::OK) {
    Serial.println("MAX30105 was not found. Please check wiring/power. ");
    while (1)
      ;
  }

  Serial.println("Place your index finger on the sensor with steady pressure.");
  // Initialize display
  // SSD1306_SWITCHCAPVCC = generate display voltage from 3.3V internally
  if (SSD1306Display::setup() != SSD1306Display::Code::OK) {
    Serial.println(F("SSD1306 allocation failed"));
    while (1)
      ;
  }
}

void loop() {
  HeartRateSensor::loop();
  if (!HeartRateSensor::isFingerDetected()) {
    SSD1306Display::showNoFinger();
  } else {
    int beatAvg = HeartRateSensor::getBeatAvg();
    SSD1306Display::showHeartBeat(beatAvg);
    Serial.printf("Average heart beat: %d", beatAvg);
  }
  Serial.println();
}
