#include "cryptid-gfx.h"
#include "cryptid-humidity.h"

uint8_t percentS_mask[7 * 6] = {
  1, 0, 1,
  0, 0, 1,
  0, 1, 0,
  1, 0, 0,
  1, 0, 1,
};
pixel_mask_t percentS_pixel_mask = {percentS_mask, 7, 6};

HumidityDisplay::HumidityDisplay(Gfx *graphics_object) {
  xStart = 2;
  yStart = 17;
  gfx = graphics_object;

  gradient_config_t *cfg = new gradient_config_t(10, 32, 16, 150, 250, false);
  gradient_config = *cfg;
}

void HumidityDisplay::update(float newHumidity, bool small) {
  humidity = (uint8_t)round(newHumidity);

  gradient_config.setStart(gfx->value2hue(humidity, 0, 100, 150, 185));
  gradient_config.setEnd(gfx->value2hue(humidity, 0, 100, 220, 255));

  if (small) {
    uint8_t width = gfx->drawCircularGradientMask(humidity, xStart, yStart, &gradient_config);
    gfx->drawCircularGradientMask(&percentS_pixel_mask, xStart + width, yStart, &gradient_config);
  }
  else {
    uint8_t width = gfx->drawCircularGradientFont(humidity, xStart, yStart, &gradient_config);
    gfx->drawCircularGradientFont("%", xStart + width, yStart, &gradient_config);
  }
}
