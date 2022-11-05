#include <Arduino.h>
#include <SPI.h>
#include <Wire.h>

#include <vector>

#include "MAX30105.h"
#include "heartRate.h"

enum class ParticleSensorCode { OK, DEVICE_NOT_FOUND };

MAX30105 max30105;

class ParticleSensor {
 public:
  static ParticleSensor &getInstance() {
    static ParticleSensor instance;
    return instance;
  }

  static ParticleSensorCode setup() {
    // Initialize heart rate sensor
    if (!max30105.begin(Wire,
                        I2C_SPEED_FAST))  // Use default I2C port, 400kHz speed
    {
      return ParticleSensorCode::DEVICE_NOT_FOUND;
    }

    max30105.setup();  // Configure sensor with default settings
    max30105.setPulseAmplitudeRed(
        0x0A);  // Turn Red LED to low to indicate sensor is running
    max30105.setPulseAmplitudeGreen(0);  // Turn off Green LED

    return ParticleSensorCode::OK;
  }

  static void loop() { getInstance()._loop(); }

  static int getBeatAvg() { return getInstance().beatAvg; }

  static bool isFingerDetected() {
    long irValue = max30105.getIR();
    return irValue > 50000;
  }

 private:
  ParticleSensor(byte rateSize = 4) {
    this->rateSize = rateSize;
    this->rates = std::vector<byte>(rateSize);
  }

  void _loop() {
    long irValue = max30105.getIR();

    if (checkForBeat(irValue)) {
      // We sensed a beat!
      long delta = millis() - lastBeat;
      lastBeat = millis();

      beatsPerMinute = 60 / (delta / 1000.0);

      if (beatsPerMinute < 255 && beatsPerMinute > 20) {
        rates[rateSpot++] =
            (byte)beatsPerMinute;      // Store this reading in the array
        rateSpot %= rates.capacity();  // Wrap variable

        // Take average of readings
        beatAvg = 0;
        for (byte x = 0; x < rateSize; x++) {
          beatAvg += rates[x];
        }
        beatAvg /= rateSize;
      }
    }
  }

  std::vector<byte> rates;
  byte rateSize;
  byte rateSpot;
  long lastBeat;  // Time at which the last beat occurred
  float beatsPerMinute;
  int beatAvg;
};
