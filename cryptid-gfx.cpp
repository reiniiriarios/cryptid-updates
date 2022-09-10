// RGB Matrix
#include <Adafruit_Protomatter.h>
// Font
#include <Fonts/FreeSans9pt7b.h>

#include "cryptid-types.h"
#include "cryptid-config.h"
#include "cryptid-gfx.h"
#include "cryptid-numbers.h"

const GFXfont *gfxFont = &FreeSans9pt7b;

inline GFXglyph *pgm_read_glyph_ptr(const GFXfont *gfxFont, uint8_t c) {
#ifdef __AVR__
  return &(((GFXglyph *)pgm_read_pointer(&gfxFont->glyph))[c]);
#else
  // expression in __AVR__ section may generate "dereferencing type-punned
  // pointer will break strict-aliasing rules" warning In fact, on other
  // platforms (such as STM32) there is no need to do this pointer magic as
  // program memory may be read in a usual way So expression may be simplified
  return gfxFont->glyph + c;
#endif //__AVR__
}

inline uint8_t *pgm_read_bitmap_ptr(const GFXfont *gfxFont) {
#ifdef __AVR__
  return (uint8_t *)pgm_read_pointer(&gfxFont->bitmap);
#else
  // expression in __AVR__ section generates "dereferencing type-punned pointer
  // will break strict-aliasing rules" warning In fact, on other platforms (such
  // as STM32) there is no need to do this pointer magic as program memory may
  // be read in a usual way So expression may be simplified
  return gfxFont->bitmap;
#endif //__AVR__
}


Gfx::Gfx(Adafruit_Protomatter *matrix_p) {
  matrix = matrix_p;
}

void Gfx::drawPixels(void) {
  for(int y = 0; y < MATRIX_HEIGHT; y++) {
    for(int x = 0; x < MATRIX_WIDTH; x++) {
      if (pixels[y][x].on == true) {
        uint16_t color = matrix->colorHSV(pixels[y][x].hue);
        matrix->drawPixel(x, y, color);
        // turn pixel back off after we move to buffer
        pixels[y][x].on = false;
      }
      else {
        matrix->drawPixel(x, y, 0);
      }
    }
  }
}

void Gfx::buildCircularGradient(uint8_t drawX, uint8_t drawY, pixel_mask_t *mask, gradient_config_t *cfg) {
  // loop through the mask
  uint8_t *pixel = mask->mask;
  for(int y = 0; y < mask->height && y < MATRIX_HEIGHT; y++) {

    // get y on pixels[] grid
    uint8_t yDraw = drawY + y;
    if (yDraw > MATRIX_HEIGHT) continue;

    for(int x = 0; x < mask->width && x < MATRIX_WIDTH; x++, pixel++) {
      // if mask is empty here, we're not drawing anything
      if (*pixel == 0) continue;

      // get x on pixels[] grid
      uint8_t xDraw = drawX + x;
      if (xDraw > MATRIX_WIDTH) continue;

      // add this pixel
      buildCircularGradientPixel(xDraw, yDraw, cfg);
    }
  }
}

void Gfx::buildCircularGradientPixel(uint8_t x, uint8_t y, gradient_config_t *cfg) {
  float tick = millis() * 0.0001f * cfg->animation_speed;
  float v = (
      cos(((float)x - CENTER_X) / (0.5f * cfg->shape_width))
      + sin(((float)y - CENTER_Y) / (0.5f * cfg->shape_width)) + tick
    ) * cfg->shape_width;

  // interval is a number between 0 and gradient_width
  float interval = fmod(v, cfg->gradient_width);
  uint16_t hue;

  // if the gradient is less than 360deg total, then
  // we must stop at the end and loop back and forth
  if (cfg->gradient_start > 0 || cfg->gradient_end < 360) {

    // find how far across the gradient we are
    float distance_across_gradient;
    if (interval < 0.5f * cfg->gradient_width) {
      // for the first half of the gradient, move twice as fast
      distance_across_gradient = interval * 2;
    }
    else {
      // for the second half, move backwards, still twice as fast
      distance_across_gradient = (cfg->gradient_width - interval) * 2.0f;
    }
    // between 0 and 1
    float percent_across_gradient = distance_across_gradient / cfg->gradient_width;

    // counterclockwise
    if (cfg->gradient_reverse) {
      // how much to scale the percent_across by, rotating counterclockwise
      float scaling_factor = 360 - cfg->gradient_end_scaled - cfg->gradient_start_scaled;
      // percent_across_gradient * scaling_factor = degrees backwards from start
      hue = round(cfg->gradient_start_scaled - percent_across_gradient * scaling_factor);
    }
    // clockwise
    else {
      // how much to scale the percent_across by
      float scaling_factor = cfg->gradient_end_scaled - cfg->gradient_start_scaled;
      // percent_across_gradient * scaling_factor = degrees across after the start
      hue = round(cfg->gradient_start_scaled + percent_across_gradient * scaling_factor);
    }
  }
  else {
    // 0 <= interval / gradient_width <= 1
    hue = round((interval / cfg->gradient_width) * 65535.0f);
  }

  // normalize within bounds
  while (hue < 0) hue += 65535;
  while (hue > 65535) hue -= 65535;

  pixels[y][x].on = true;
  pixels[y][x].hue = hue;
}

uint8_t Gfx::buildCircularGradientFromNumberFont(float number, uint8_t x, uint8_t y, gradient_config_t *cfg) {
  return buildCircularGradientFromNumberFont((uint8_t)round(number), x, y, cfg);
}

uint8_t Gfx::buildCircularGradientFromNumberFont(uint8_t number, uint8_t x, uint8_t y, gradient_config_t *cfg) {
  return buildCircularGradientFromString(String(number), x, y, cfg);
}

uint8_t Gfx::buildCircularGradientFromNumberMask(float number, uint8_t x, uint8_t y, gradient_config_t *cfg) {
  return buildCircularGradientFromNumberMask((uint8_t)round(number), x, y, cfg);
}

uint8_t Gfx::buildCircularGradientFromNumberMask(uint8_t number, uint8_t x, uint8_t y, gradient_config_t *cfg) {
  // Calculate the number of digits in the number.
  uint8_t num_digits = 0;
  for (uint8_t n = number; n > 0; n /= 10) {
    num_digits++;
  }
  // Make an array of digits, small to large (right to left).
  uint8_t digits[num_digits];
  for (uint8_t n = number, i = 0; n > 0; n /= 10, i++) {
    digits[i] = n % 10;
  }
  // Draw each digit in reverse order.
  uint8_t width = 0;
  for (int8_t i = num_digits - 1; i >= 0; i--) {
    buildCircularGradient(x + width, y, &numberMasks[digits[i]], cfg);
    width += numberMasks[digits[i]].width + 1;
  }

  return width;
}

uint8_t Gfx::buildCircularGradientFromString(String s, uint8_t x, uint8_t y, gradient_config_t *cfg) {
  uint8_t width = 0;
  for (uint8_t i = 0; i < s.length(); i++) {
    width += buildCircularGradientFromChar(s[i], x + width, y, cfg) + 1;
  }

  return width;
}

uint8_t Gfx::buildCircularGradientFromChar(char c, uint8_t x, uint8_t y, gradient_config_t *cfg) {
  c -= (uint8_t)pgm_read_byte(&gfxFont->first);
  GFXglyph *glyph = pgm_read_glyph_ptr(gfxFont, c);
  uint8_t *bitmap = pgm_read_bitmap_ptr(gfxFont);

  uint16_t bo = pgm_read_word(&glyph->bitmapOffset);
  uint8_t w = pgm_read_byte(&glyph->width), h = pgm_read_byte(&glyph->height);
  int8_t xo = pgm_read_byte(&glyph->xOffset), yo = pgm_read_byte(&glyph->yOffset);
  uint8_t xx, yy, bits = 0, bit = 0;
  int16_t xo16 = 0, yo16 = 0;

  uint8_t i = 0;
  for (yy = 0; yy < h; yy++) {
    for (xx = 0; xx < w; xx++) {
      if (!(bit++ & 7)) {
        bits = pgm_read_byte(&bitmap[bo++]);
      }
      if (bits & 0x80) {
        buildCircularGradientPixel(x + xx, y + yy, cfg);
      }
      bits <<= 1;
      i++;
    }
  }

  return w;
}
