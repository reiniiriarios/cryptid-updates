#include "aaahhh.h"

Aaahhh::Aaahhh(Gfx *gfx_p) {
  gfx = gfx_p;
}

void Aaahhh::display(void) {
  gfx->drawCircularGradientFont(displayString, gfx->getCenterFontX(&displayString), 9, &gcfg);
}

bool Aaahhh::isMovingFast(void) {
  return moving_fast;
}

void Aaahhh::setMovement(float x, float y, float z) {
  setMovement(double(x * 1000), double(y * 1000), double(z * 1000));
}

void Aaahhh::setMovement(double x, double y, double z) {
  double movex = accel_x - x;
  double movey = accel_y - y;
  double movez = accel_z - z;
  if (movex > 1000 || movex < -1000 || movey > 1000 || movey < -1000 || movez > 1000 || movez < -1000) {
    moving_fast = true;
    started_moving_fast = millis();
  }
  else if (moving_fast && millis() - started_moving_fast > 1000) {
    moving_fast = false;
  }
  accel_x = x;
  accel_y = y;
  accel_z = z;
}
