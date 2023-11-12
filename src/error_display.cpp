#include "error_display.h"
#include "gfx.h"

ErrorDisplay::ErrorDisplay(Gfx *gfx_p) {
  gfx = gfx_p;

  pixel_mask.width = 11;
  pixel_mask.height = 5;
  pixel_mask.mask = mask;

  gradient_config_t *cfg = new gradient_config_t(10, 32, 16, 0, 15, false);
  gradient_config = *cfg;
}

void ErrorDisplay::update(uint8_t errorCode) {
  gfx->drawCircularGradientMask(&pixel_mask, x, y, &gradient_config);
  gfx->drawCircularGradientMask(errorCode, x + pixel_mask.width + 1, y, &gradient_config);
}
