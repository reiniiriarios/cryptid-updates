
#include "cryptid-gfx.h"
#include "cryptid-temperature.h"

TemperatureDisplay::TemperatureDisplay(Gfx* graphics_object) {
  xStart = 4;
  yStart = 4;
  gfx = graphics_object;
  //@todo: adjustable font size here
  drawCharSize = font_width * font_height;

  // temp_mask[0].width = font_width;
  // temp_mask[2].width = font_width;
  // temp_mask[2].width = font_width;
  // temp_mask[0].height = font_height;
  // temp_mask[1].height = font_height;
  // temp_mask[2].height = font_height;
  // uint8_t* temp_mask_0_m = new uint8_t[drawCharSize]();
  // uint8_t* temp_mask_1_m = new uint8_t[drawCharSize]();
  // uint8_t* temp_mask_2_m = new uint8_t[drawCharSize]();
  // temp_mask[0].mask = temp_mask_0_m;
  // temp_mask[1].mask = temp_mask_1_m;
  // temp_mask[2].mask = temp_mask_2_m;

  // gradient_image[0].mask = temp_mask[0];
  // gradient_image[1].mask = temp_mask[1];
  // gradient_image[2].mask = temp_mask[2];

  gradient_config.animation_speed  = 10;
  gradient_config.gradient_width   = 32;
  gradient_config.shape_width      = 16;
  gradient_config.gradient_start   = 150;
  gradient_config.gradient_end     = 250;

  gradient_image[0].config = gradient_config;
  gradient_image[1].config = gradient_config;
  gradient_image[2].config = gradient_config;

  gradient_image[0].x = xStart;
  gradient_image[0].y = yStart;
  gradient_image[1].y = yStart;
  gradient_image[2].y = yStart;
}

//@todo: change hue based on temperature
void TemperatureDisplay::update(float newTemperature) {
  uint8_t newTemp = (uint8_t)round(newTemperature);
  if (newTemp == temperature) {
    return;
  }

  temperatureChars = String(newTemp);
  uint8_t drawX = xStart;

  drawX += gfx->drawChar(temperatureChars[0], drawX, yStart, &gradient_config);
  drawX += gfx->drawChar(temperatureChars[1], drawX, yStart, &gradient_config);
  if (newTemp >= 100) {
    gfx->drawChar(temperatureChars[2], drawX, yStart, &gradient_config);
  }
}
