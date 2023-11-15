#include "aaahhh.h"

Aaahhh::Aaahhh(Gfx *gfx_p) {
  gfx = gfx_p;
}

void Aaahhh::display(void) {
  gfx->drawCircularGradientFont(displayString, gfx->getCenterFontX(&displayString), 9, &gcfg);
}
