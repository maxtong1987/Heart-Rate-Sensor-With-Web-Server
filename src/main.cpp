#include <Arduino.h>

#include <Wire.h>
#include "MAX30105.h"
#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#include <vector>
#include <string>

#include "heartRate.h"
#include "ParticleSensor.h"

// Display settings
#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 32 // OLED display height, in pixels

// Declaration for an SSD1306 display connected to I2C (SDA, SCL pins)
// The pins for I2C are defined by the Wire-library.
// On an arduino UNO:       A4(SDA), A5(SCL)
// On an arduino MEGA 2560: 20(SDA), 21(SCL)
// On an arduino LEONARDO:   2(SDA),  3(SCL), ...
#define OLED_RESET -1       // Reset pin # (or -1 if sharing Arduino reset pin)
#define SCREEN_ADDRESS 0x3C ///< See datasheet for Address; 0x3D for 128x64, 0x3C for 128x32
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

// 16x16px
#define LOGO_HEIGHT 16
#define LOGO_WIDTH 16
static const unsigned char PROGMEM logo_bmp[] = {
    0x00, 0x00, 0x3e, 0x7c, 0x7f, 0xfe, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
    0x7f, 0xfe, 0x7f, 0xfe, 0x3f, 0xfc, 0x1f, 0xf8, 0x0f, 0xf0, 0x03, 0xc0, 0x01, 0x80, 0x00, 0x00};

void drawheart(void)
{
  display.drawBitmap(1, 0, logo_bmp, LOGO_WIDTH, LOGO_HEIGHT, 1);
}

void setup()
{
  Serial.begin(115200);
  Serial.println("Initializing...");

  ParticleSensorCode code = ParticleSensor::setup();
  if (code == ParticleSensorCode::DEVICE_NOT_FOUND) {
    Serial.println("MAX30105 was not found. Please check wiring/power. ");
    while (1);
  }

  Serial.println("Place your index finger on the sensor with steady pressure.");
  // Initialize display
  // SSD1306_SWITCHCAPVCC = generate display voltage from 3.3V internally
  if (!display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS))
  {
    Serial.println(F("SSD1306 allocation failed"));
    for (;;)
      ; // Don't proceed, loop forever
  }

  display.display();
  delay(2000); // Pause for 2 seconds

  // Clear the buffer
  display.clearDisplay();

  display.setTextSize(2);              // Normal 1:1 pixel scale
  display.setTextColor(SSD1306_WHITE); // Draw white text
  display.setCursor(0, 0);             // Start at top-left corner
  display.cp437(true);                 // Use full 256 char 'Code Page 437' font
}

void loop()
{
  ParticleSensor::loop();

  display.clearDisplay();
  if (!ParticleSensor::isFingerDetected())
  {
    Serial.print(" No finger????");
    display.setCursor(0, 0);
    display.println("---");
  }
  else
  {
    display.setCursor(0, 0);
    drawheart();
    display.setCursor(20, 0);
    display.printf("%d", ParticleSensor::getBeatAvg());
  }
  display.display();

  Serial.println();
}
