#include <Adafruit_MLX90614.h>

namespace InfraredThermometer {

enum class Code { OK, FAIL };

Code setup(double emissivity);

double readTempC();
};  // namespace InfraredThermometer