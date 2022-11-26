#include <Adafruit_MLX90614.h>

namespace InfraredThermometer {

enum class Code { OK, FAIL };

Adafruit_MLX90614 mlx = Adafruit_MLX90614();

Code setup(double emissivity) {
  // init sensor
  if (!mlx.begin()) return Code::FAIL;
  mlx.writeEmissivity(emissivity);
  return Code::OK;
}

double readTempC() { return mlx.readObjectTempC(); }
};  // namespace InfraredThermometer