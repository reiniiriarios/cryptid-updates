
#include "cryptid-gfx.h"
#include "cryptid-temperature.h"
#include "cryptid-numbers.h"

TemperatureDisplay::TemperatureDisplay(Gfx *graphics_object) {
  xStart = 4;
  yStart = 4;
  gfx = graphics_object;

  gradient_config_t *cfg = new gradient_config_t(10, 32, 16, 150, 250, false);
  gradient_config = *cfg;
}

//@todo: change hue based on temperature
void TemperatureDisplay::update(float newTemperature) {
  temperature = (uint8_t)round(newTemperature);
  gfx->buildCircularGradientFromNumber(temperature, xStart, yStart, &gradient_config);
}
