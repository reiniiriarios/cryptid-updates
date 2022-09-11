#include "cryptid-gfx.h"
#include "cryptid-heart.h"

Heart::Heart(Gfx *gfx_p) {
  gfx = gfx_p;

  pixel_mask.width = 11;
  pixel_mask.height = 11;
  pixel_mask.mask = mask;

  gradient_config_t *cfg = new gradient_config_t(10, 32, 16, 260, 350, false);
  gradient_config = *cfg;
}

void Heart::update(void) {
  gfx->drawCircularGradientMask(&pixel_mask, 45, 4, &gradient_config);
}
