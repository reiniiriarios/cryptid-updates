#include "gfx.h"
#include "temperature.h"

uint8_t degF_mask[7 * 6] = {
  1, 1, 1, 0, 1, 1, 1,
  1, 0, 1, 0, 1, 0, 0,
  1, 1, 1, 0, 1, 1, 1,
  0, 0, 0, 0, 1, 0, 0,
  0, 0, 0, 0, 1, 0, 0
};
pixel_mask_t degF_pixel_mask = {degF_mask, 7, 6};

uint8_t deg_mask[5 * 5] = {
  0, 1, 1, 1, 0,
  1, 1, 0, 1, 1,
  1, 0, 0, 0, 1,
  1, 1, 0, 1, 1,
  0, 1, 1, 1, 0
};
pixel_mask_t deg_pixel_mask = {deg_mask, 5, 5};

TemperatureDisplay::TemperatureDisplay(Gfx *graphics_object) {
  xStart = 2;
  yStart = 2;
  gfx = graphics_object;

  gradient_config_t *cfg = new gradient_config_t(10, 32, 16, 150, 250, false);
  gradient_config = *cfg;
}

void TemperatureDisplay::update(float newTemperature, bool small) {
  temperature = (uint8_t)round(newTemperature);

  // really cold
  if (temperature < 65) {
    gradient_config.setStart(170);
    gradient_config.setEnd(240);
  }
  // still cold
  else if (temperature < 70) {
    gradient_config.setStart(gfx->value2hue(temperature, 65, 69, 175, 150));
    gradient_config.setEnd(gfx->value2hue(temperature, 65, 69, 255, 230));
  }
  // okay warm now
  else if (temperature < 90) {
    gradient_config.setStart(gfx->value2hue(temperature, 70, 89, 315, 335));
    gradient_config.setEnd(gfx->value2hue(temperature, 70, 89, 340, 360));
  }
  // and we're hot af
  else {
    gradient_config.setStart(310);
    gradient_config.setEnd(335);
  }

  if (small) {
    uint8_t width = gfx->drawCircularGradientMask(temperature, xStart, yStart, &gradient_config);
    gfx->drawCircularGradientMask(&degF_pixel_mask, xStart + width, yStart, &gradient_config);
  }
  else {
    uint8_t width = gfx->drawCircularGradientFont(temperature, xStart, yStart, &gradient_config);
    gfx->drawCircularGradientMask(&deg_pixel_mask, xStart + width, yStart, &gradient_config);
    width += deg_pixel_mask.width + 1;
    gfx->drawCircularGradientFont("F", xStart + width, yStart, &gradient_config);
  }
}
