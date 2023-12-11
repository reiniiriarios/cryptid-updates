#include "aqi.h"

AQI::AQI(Gfx *gfx_p) {
  gfx = gfx_p;
}

void AQI::update(uint16_t aqiValue) {
  // Move left half a digit if 3 or 1 digits instead of 2
  uint8_t xx = x;
  if (aqiValue < 10) {
    xx += 2;
  } else if (aqiValue >= 100) {
    xx -= 2;
  }
  setGradient(aqiValue);
  gfx->drawCircularGradientMask(&pixel_mask, xx, y, &gradient_config);
  gfx->drawCircularGradientMask(aqiValue, xx + pixel_mask.width + 1, y + 1, &gradient_config);
}

void AQI::setGradient(uint16_t aqiValue) {
  // good
  if (aqiValue < 50) {
    gradient_config.setStart(135);
    gradient_config.setEnd(165);
  }
  // moderate
  else if (aqiValue < 100) {
    gradient_config.setStart(50);
    gradient_config.setEnd(75);
  }
  // unhealthy for sensitive groups
  else if (aqiValue < 150) {
    gradient_config.setStart(15);
    gradient_config.setEnd(45);
  }
  // unhealthy
  else if (aqiValue < 200) {
    gradient_config.setStart(0);
    gradient_config.setEnd(30);
  }
  // very unhealthy
  else if (aqiValue < 300) {
    gradient_config.setStart(340);
    gradient_config.setEnd(370); // 10
  }
  // hazardous
  else {
    gradient_config.setStart(340);
    gradient_config.setEnd(420); // 60
  }
}
