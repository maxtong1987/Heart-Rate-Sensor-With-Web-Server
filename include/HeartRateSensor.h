namespace HeartRateSensor {

enum class Code { OK, DEVICE_NOT_FOUND };

Code setup();

int getBeatAvg();

bool isFingerDetected();

void loop();

};  // namespace HeartRateSensor