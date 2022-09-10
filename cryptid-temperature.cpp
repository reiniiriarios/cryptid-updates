#include <algorithm>
using namespace std;

#include "cryptid-gfx.h"
#include "cryptid-temperature.h"

uint8_t degF_mask[7 * 6] = {
  1, 1, 1, 0, 1, 1, 1,
  1, 0, 1, 0, 1, 0, 0,
  1, 1, 1, 0, 1, 1, 1,
  0, 0, 0, 0, 1, 0, 0,
  0, 0, 0, 0, 1, 0, 0
};
pixel_mask_t degF_pixel_mask = {degF_mask, 7, 6};

TemperatureDisplay::TemperatureDisplay(Gfx *graphics_object) {
  xStart = 4;
  yStart = 4;
  gfx = graphics_object;

  gradient_config_t *cfg = new gradient_config_t(10, 32, 16, 150, 250, false);
  gradient_config = *cfg;
}

void TemperatureDisplay::update(float newTemperature) {
  temperature = (uint8_t)round(newTemperature);

  // really cold
  if (temperature < 65) {
    gradient_config.setStart(170);
    gradient_config.setEnd(240);
  }
  // still cold
  else if (temperature < 70) {
    gradient_config.setStart(temperature2hue(temperature, 65, 69, 175, 150));
    gradient_config.setEnd(temperature2hue(temperature, 65, 69, 245, 220));
  }
  // okay warm now
  else if (temperature < 90) {
    gradient_config.setStart(temperature2hue(temperature, 70, 89, 410, 315));
    gradient_config.setEnd(temperature2hue(temperature, 70, 89, 435, 340));
  }
  // and we're hot af
  else {
    gradient_config.setStart(310);
    gradient_config.setEnd(335);
  }

  // Draw temperature.
  uint8_t width = gfx->buildCircularGradientFromNumberMask(temperature, xStart, yStart, &gradient_config);
  // Draw °F.
  gfx->buildCircularGradient(xStart + width, yStart, &degF_pixel_mask, &gradient_config);
}

uint16_t TemperatureDisplay::temperature2hue(int value, int minFrom, int maxFrom, int minTo, int maxTo) {
  int hue = round((float)(std::min(maxFrom, std::max(minFrom, value)) - minFrom) * ((float)(maxTo - minTo) / (float)(maxFrom - minFrom)) + minTo);
  while (hue < 0) hue += 360;
  while (hue > 360) hue -= 360;
  return (uint16_t)hue;
}
