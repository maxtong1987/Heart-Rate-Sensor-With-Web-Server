#include "HeartRateSensor.h"

#include <vector>

#include "MAX30105.h"
#include "heartRate.h"

namespace HeartRateSensor {

class Calculator {
 private:
  std::vector<byte> rates;
  byte rateSize;
  byte rateSpot;
  long lastBeat;  // Time at which the last beat occurred
  int beatAvg;

 public:
  Calculator(byte rateSize = 4) {
    this->rateSize = rateSize;
    this->rates = std::vector<byte>(rateSize);
    this->rateSpot = 0;
    this->lastBeat = 0;
    this->beatAvg = 0;
  }

  int getBeatAvg() { return beatAvg; }

  void calculate() {
    long delta = millis() - lastBeat;
    lastBeat = millis();

    long beatsPerMinute = 60 / (delta / 1000.0);

    if (beatsPerMinute >= 255 || beatsPerMinute <= 20) return;

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
};

// The heart rate sensor we are using.
MAX30105 max30105;

// To calculate average heart beat.
Calculator calculator;

Code setup() {
  // Initialize heart rate sensor
  if (!max30105.begin(Wire,
                      I2C_SPEED_FAST))  // Use default I2C port, 400kHz speed
  {
    return Code::DEVICE_NOT_FOUND;
  }

  max30105.setup();  // Configure sensor with default settings
  max30105.setPulseAmplitudeRed(
      0x0A);  // Turn Red LED to low to indicate sensor is running
  max30105.setPulseAmplitudeGreen(0);  // Turn off Green LED

  return Code::OK;
}

int getBeatAvg() { return calculator.getBeatAvg(); }

bool isFingerDetected() {
  long irValue = max30105.getIR();
  return irValue > 50000;
}

void loop() {
  long irValue = max30105.getIR();

  if (!checkForBeat(irValue)) return;  // Return if no heart beat.

  // We sensed a beat!
  calculator.calculate();
}

};  // namespace HeartRateSensor