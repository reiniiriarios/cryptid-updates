
#include "cryptid-updates.h"
#include "gfx.cpp"

class TemperatureDisplay {
  public:
    uint8_t temperature;
    uint8_t font_width = 9;
    uint8_t font_height = 9;
    uint8_t xStart;
    uint8_t yStart;
    gradient_config_t gradient_config;
    Gfx* gfx;

    TemperatureDisplay(uint8_t x, uint8_t y, Gfx* graphics_object) {
      xStart = x;
      yStart = y;
      gfx = graphics_object;
      //@todo: adjustable font size here
      drawCharSize = font_width * font_height;

      temp_mask_0.width = font_width;
      temp_mask_1.width = font_width;
      temp_mask_2.width = font_width;
      temp_mask_0.height = font_height;
      temp_mask_1.height = font_height;
      temp_mask_2.height = font_height;
      uint8_t* temp_mask_0_m = new uint8_t[drawCharSize]();
      uint8_t* temp_mask_1_m = new uint8_t[drawCharSize]();
      uint8_t* temp_mask_2_m = new uint8_t[drawCharSize]();
      temp_mask_0.mask = temp_mask_0_m;
      temp_mask_1.mask = temp_mask_1_m;
      temp_mask_2.mask = temp_mask_2_m;

      gradient_config.animation_speed  = 10;
      gradient_config.gradient_width   = 32;
      gradient_config.shape_width      = 16;
      gradient_config.gradient_start   = 150;
      gradient_config.gradient_end     = 250;
    }

    //@todo: change hue based on temperature
    void update(float newTemperature) {
      uint8_t newTemp = (uint8_t)round(newTemperature);
      if (newTemp == temperature) {
        return;
      }

      String charTemp = String(newTemp);
      uint8_t drawX = xStart;

      if (newTemp >= 100) {
        // draw first digit
        drawX += (*gfx).fillMaskFromChar(charTemp[0], &temp_mask_0);
        (*gfx).buildCircularGradientFromMask(&temp_mask_0, drawX, yStart, &gradient_config);
        // draw second digit
        drawX += (*gfx).fillMaskFromChar(charTemp[1], &temp_mask_1);
        (*gfx).buildCircularGradientFromMask(&temp_mask_1, drawX, yStart, &gradient_config);
        // draw third digit
        (*gfx).fillMaskFromChar(charTemp[2], &temp_mask_2);
        (*gfx).buildCircularGradientFromMask(&temp_mask_2, drawX, yStart, &gradient_config);
      }
      else {
        // draw first digit
        drawX += (*gfx).fillMaskFromChar(charTemp[0], &temp_mask_1);
        (*gfx).buildCircularGradientFromMask(&temp_mask_1, drawX, yStart, &gradient_config);
        // draw second digit
        (*gfx).fillMaskFromChar(charTemp[1], &temp_mask_2);
        (*gfx).buildCircularGradientFromMask(&temp_mask_2, drawX, yStart, &gradient_config);
      }
    }

  private:
    pixel_mask_t temp_mask_0;
    pixel_mask_t temp_mask_1;
    pixel_mask_t temp_mask_2;
    int drawCharSize;
};
