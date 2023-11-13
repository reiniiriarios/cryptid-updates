#include "loading.h"
#include "gfx.h"

void displayLoadingScreen(Gfx* gfx) {
  pixel_mask_t mask = {};
  mask.width = 27;
  mask.height = 5;
  uint8_t loadingMask[27 * 5] = {
    1, 0, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 1, 1, 0, 0, 1, 0, 1, 0, 0, 1, 0, 0, 1, 1, 0,
    1, 0, 0, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 1, 0, 1, 0, 1, 0, 0, 0,
    1, 0, 0, 0, 1, 0, 1, 0, 1, 1, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 1, 0, 1, 0, 1, 1,
    1, 0, 0, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 0, 1, 0, 1, 0, 0, 1,
    1, 1, 1, 0, 0, 1, 0, 0, 1, 0, 1, 0, 1, 1, 0, 0, 1, 0, 1, 0, 0, 1, 0, 0, 1, 1, 0,
  };
  mask.mask = loadingMask;
  gradient_config_t *cfg = new gradient_config_t(10, 32, 16, 225, 240, false);
  gfx->drawCircularGradientMask(&mask, 18, 13, cfg);
}
