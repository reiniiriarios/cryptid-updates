#include "cryptid-gfx.h"
#include "cryptid-heart.h"

Heart::Heart(Gfx* gfx_p) {
  gfx = gfx_p;

  pixel_mask.width = 11;
  pixel_mask.height = 11;
  pixel_mask.mask = mask;

  gradient_config.animation_speed  = 10;
  gradient_config.gradient_width   = 32;
  gradient_config.shape_width      = 16;
  gradient_config.gradient_start   = 260;
  gradient_config.gradient_end     = 350;

  gradient_image.config = gradient_config;
  gradient_image.mask = pixel_mask;

  gradient_image.x = 40;
  gradient_image.y = 4;
}

void Heart::update(void) {
  gfx->buildCircularGradient(&gradient_image);
}
