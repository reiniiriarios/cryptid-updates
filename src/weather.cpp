#include "weather.h"
#include "gfx.h"

const int SYMBOL_W = 24;
const int SYMBOL_H = 24;

uint8_t WEATHER_MASK_UNKNOWN[SYMBOL_W * SYMBOL_H] = {
  0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
  0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,
  0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,
  0,0,0,0,0,0,1,1,1,1,1,0,0,1,1,1,1,1,0,0,0,0,0,0,
  0,0,0,0,0,1,1,1,1,0,0,0,0,0,0,1,1,1,1,0,0,0,0,0,
  0,0,0,0,1,1,1,1,0,0,0,0,0,0,0,1,1,1,1,0,0,0,0,0,
  0,0,0,0,1,1,1,1,0,0,0,0,0,0,0,0,1,1,1,1,0,0,0,0,
  0,0,0,0,1,1,1,1,0,0,0,0,0,0,0,0,1,1,1,1,0,0,0,0,
  0,0,0,0,1,1,1,1,0,0,0,0,0,0,0,0,1,1,1,1,0,0,0,0,
  0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,0,0,0,0,
  0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,0,0,0,0,0,
  0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,0,0,0,0,0,0,
  0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,0,0,0,0,0,0,0,
  0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,0,0,0,0,0,0,0,0,
  0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,0,0,0,0,0,0,0,0,0,
  0,0,0,0,0,0,0,0,0,0,1,1,1,1,0,0,0,0,0,0,0,0,0,0,
  0,0,0,0,0,0,0,0,0,0,1,1,1,1,0,0,0,0,0,0,0,0,0,0,
  0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
  0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
  0,0,0,0,0,0,0,0,0,0,1,1,1,1,0,0,0,0,0,0,0,0,0,0,
  0,0,0,0,0,0,0,0,0,0,1,1,1,1,0,0,0,0,0,0,0,0,0,0,
  0,0,0,0,0,0,0,0,0,0,1,1,1,1,0,0,0,0,0,0,0,0,0,0,
  0,0,0,0,0,0,0,0,0,0,1,1,1,1,0,0,0,0,0,0,0,0,0,0,
  0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
};
pixel_mask_t WEATHER_PMASK_UNKNOWN = {WEATHER_MASK_UNKNOWN, SYMBOL_W, SYMBOL_H};
gradient_config_t WEATHER_GCFG_UNKNOWN = { 10, 32, 16, 0, 10, false };

uint8_t WEATHER_MASK_INT[SYMBOL_W * SYMBOL_H] = {
  0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
  0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
  0,0,0,0,0,0,0,0,0,0,0,1,1,0,0,0,0,0,0,0,0,0,0,0,
  0,0,0,0,0,0,0,0,0,1,1,0,0,1,1,0,0,0,0,0,0,0,0,0,
  0,0,0,0,0,0,0,1,1,0,0,0,0,0,0,1,1,0,0,0,0,0,0,0,
  0,0,0,0,0,1,1,0,0,0,0,0,0,0,0,0,0,1,1,0,0,0,0,0,
  0,0,0,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,0,0,0,
  0,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,0,
  0,0,0,1,0,0,1,1,1,1,1,1,0,0,0,0,0,0,0,0,1,0,0,0,
  0,0,0,1,0,0,1,0,0,0,0,1,0,0,0,0,0,0,0,0,1,0,0,0,
  0,0,0,1,0,0,1,0,0,0,0,1,0,0,0,0,0,0,0,0,1,0,0,0,
  0,0,0,1,0,0,1,0,0,0,0,1,0,0,0,0,0,0,0,0,1,0,0,0,
  0,0,0,1,0,0,1,0,0,0,0,1,0,0,0,0,0,0,0,0,1,0,0,0,
  0,0,0,1,0,0,1,1,1,1,1,1,0,0,0,0,0,0,0,0,1,0,0,0,
  0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,
  0,0,0,1,0,0,0,0,0,0,0,0,0,1,1,1,1,0,0,0,1,0,0,0,
  0,0,0,1,0,0,0,0,0,0,0,0,0,1,0,0,1,0,0,0,1,0,0,0,
  0,0,0,1,0,0,0,0,0,0,0,0,0,1,0,0,1,0,0,0,1,0,0,0,
  0,0,0,1,0,0,0,0,0,0,0,0,0,1,0,0,1,0,0,0,1,0,0,0,
  0,0,0,1,0,0,0,0,0,0,0,0,0,1,0,1,1,0,0,0,1,0,0,0,
  0,0,0,1,0,0,0,0,0,0,0,0,0,1,0,0,1,0,0,0,1,0,0,0,
  0,0,0,1,0,0,0,0,0,0,0,0,0,1,0,0,1,0,0,0,1,0,0,0,
  0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0,
  0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
};
pixel_mask_t WEATHER_PMASK_INT = {WEATHER_MASK_INT, SYMBOL_W, SYMBOL_H};
gradient_config_t WEATHER_GCFG_INT = { 10, 32, 16, 290, 310, false };

uint8_t WEATHER_MASK_SUN[SYMBOL_W * SYMBOL_H] = {
  0,0,0,0,0,0,0,0,0,0,0,1,1,0,0,0,0,0,0,0,0,0,0,0,
  0,0,0,0,0,0,0,0,0,0,0,1,1,0,0,0,0,0,0,0,0,0,0,0,
  0,0,0,1,0,0,0,0,0,0,0,1,1,0,0,0,0,0,0,0,1,0,0,0,
  0,0,1,1,1,0,0,0,0,0,0,1,1,0,0,0,0,0,0,1,1,1,0,0,
  0,0,0,1,1,1,0,0,0,0,0,1,1,0,0,0,0,0,1,1,1,0,0,0,
  0,0,0,0,1,1,1,0,0,0,0,0,0,0,0,0,0,1,1,1,0,0,0,0,
  0,0,0,0,0,1,1,0,0,0,0,0,0,0,0,0,0,1,1,0,0,0,0,0,
  0,0,0,0,0,0,0,0,0,0,1,1,1,1,0,0,0,0,0,0,0,0,0,0,
  0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,
  0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,
  0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,
  1,1,1,1,1,0,0,1,1,1,1,1,1,1,1,1,1,0,0,1,1,1,1,1,
  1,1,1,1,1,0,0,1,1,1,1,1,1,1,1,1,1,0,0,1,1,1,1,1,
  0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,
  0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,
  0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,
  0,0,0,0,0,0,0,0,0,0,1,1,1,1,0,0,0,0,0,0,0,0,0,0,
  0,0,0,0,0,1,1,0,0,0,0,0,0,0,0,0,0,1,1,0,0,0,0,0,
  0,0,0,0,1,1,1,0,0,0,0,0,0,0,0,0,0,1,1,1,0,0,0,0,
  0,0,0,1,1,1,0,0,0,0,0,1,1,0,0,0,0,0,1,1,1,0,0,0,
  0,0,1,1,1,0,0,0,0,0,0,1,1,0,0,0,0,0,0,1,1,1,0,0,
  0,0,0,1,0,0,0,0,0,0,0,1,1,0,0,0,0,0,0,0,1,0,0,0,
  0,0,0,0,0,0,0,0,0,0,0,1,1,0,0,0,0,0,0,0,0,0,0,0,
  0,0,0,0,0,0,0,0,0,0,0,1,1,0,0,0,0,0,0,0,0,0,0,0,
};
pixel_mask_t WEATHER_PMASK_SUN = {WEATHER_MASK_SUN, SYMBOL_W, SYMBOL_H};
gradient_config_t WEATHER_GCFG_SUN = { 10, 32, 16, 40, 70, false };

uint8_t WEATHER_MASK_CLOUD[SYMBOL_W * 15] = {
  0,0,0,0,0,0,0,0,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,
  0,0,0,0,0,0,1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,
  0,0,0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0,0,0,0,
  0,0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0,0,
  0,0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0,
  0,0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,
  0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,
  0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,
  1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
  1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
  1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
  1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
  1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
  0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,
  0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,
};
pixel_mask_t WEATHER_PMASK_CLOUD = {WEATHER_MASK_CLOUD, SYMBOL_W, 15};
gradient_config_t WEATHER_GCFG_CLOUD = { 10, 32, 16, 170, 190, false };

uint8_t WEATHER_MASK_CLOUD_S[16 * 10] = {
  0,0,0,0,0,0,0,1,1,1,1,1,0,0,0,0,
  0,0,0,1,1,1,1,1,1,1,1,1,1,0,0,0,
  0,0,1,1,1,1,1,1,1,1,1,1,1,1,0,0,
  0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,
  0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
  1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
  1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
  1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
  0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
  0,0,1,1,1,1,1,1,1,1,1,1,1,1,0,0,
};
pixel_mask_t WEATHER_PMASK_CLOUD_S = {WEATHER_MASK_CLOUD_S, 16, 10};
gradient_config_t WEATHER_GCFG_CLOUD_S = { 10, 32, 16, 170, 190, false };

uint8_t WEATHER_MASK_SUN_S[14 * 13] = {
  0,0,0,0,0,0,0,1,0,0,0,0,0,0,
  0,0,0,0,0,0,0,1,0,0,0,0,0,0,
  0,1,0,0,0,0,0,1,0,0,0,0,0,1,
  0,0,1,0,0,0,0,0,0,0,0,0,1,0,
  0,0,0,1,0,0,1,1,1,0,0,1,0,0,
  0,0,0,0,0,1,1,1,1,1,0,0,0,0,
  0,0,0,0,1,1,1,1,1,1,1,0,0,0,
  1,1,1,0,1,1,1,1,0,0,0,0,0,0,
  0,0,0,0,1,1,1,0,0,0,0,0,0,0,
  0,0,0,0,0,1,0,0,0,0,0,0,0,0,
  0,0,0,1,0,0,0,0,0,0,0,0,0,0,
  0,0,1,0,0,0,0,0,0,0,0,0,0,0,
  0,1,0,0,0,0,0,0,0,0,0,0,0,0,
};
pixel_mask_t WEATHER_PMASK_SUN_S = {WEATHER_MASK_SUN_S, 14, 15};
gradient_config_t WEATHER_GCFG_SUN_S = { 10, 32, 16, 40, 70, false };

uint8_t WEATHER_MASK_RAIN_L[SYMBOL_W * 4] = {
  0,0,0,0,1,0,0,0,0,0,0,1,0,0,0,0,0,0,1,0,0,0,0,0,
  0,0,0,0,1,0,0,0,0,0,0,1,0,0,0,0,0,0,1,0,0,0,0,0,
  0,0,0,0,0,1,0,0,0,0,0,0,1,0,0,0,0,0,0,1,0,0,0,0,
  0,0,0,0,0,1,0,0,0,0,0,0,1,0,0,0,0,0,0,1,0,0,0,0,
};
pixel_mask_t WEATHER_PMASK_RAIN_L = {WEATHER_MASK_RAIN_L, SYMBOL_W, 4};
gradient_config_t WEATHER_GCFG_RAIN_L = { 10, 32, 16, 220, 240, false };

uint8_t WEATHER_MASK_RAIN_H[SYMBOL_W * 7] = {
  0,0,1,0,0,0,0,0,0,1,0,0,0,0,0,0,1,0,0,0,0,0,0,0,
  0,0,1,0,0,0,0,0,0,1,0,0,0,0,0,0,1,0,0,0,0,0,0,0,
  0,0,0,1,0,0,0,0,0,0,1,0,0,0,0,0,0,1,0,0,0,0,0,0,
  0,0,0,1,0,0,1,0,0,0,1,0,0,1,0,0,0,1,0,0,1,0,0,0,
  0,0,0,0,0,0,1,0,0,0,0,0,0,1,0,0,0,0,0,0,1,0,0,0,
  0,0,0,0,0,0,0,1,0,0,0,0,0,0,1,0,0,0,0,0,0,1,0,0,
  0,0,0,0,0,0,0,1,0,0,0,0,0,0,1,0,0,0,0,0,0,1,0,0,
};
pixel_mask_t WEATHER_PMASK_RAIN_H = {WEATHER_MASK_RAIN_H, SYMBOL_W, 7};
gradient_config_t WEATHER_GCFG_RAIN_H = { 10, 32, 16, 220, 240, false };

uint8_t WEATHER_MASK_SNOW_L[SYMBOL_W * 5] = {
  0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
  0,0,0,0,1,0,0,0,1,1,1,0,0,0,0,0,0,0,0,1,0,0,0,0,
  0,0,0,1,1,1,0,0,0,1,0,0,0,0,1,0,0,0,1,1,1,0,0,0,
  0,0,0,0,1,0,0,0,0,0,0,0,0,1,1,1,0,0,0,1,0,0,0,0,
  0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,
};
pixel_mask_t WEATHER_PMASK_SNOW_L = {WEATHER_MASK_SNOW_L, SYMBOL_W, 5};
gradient_config_t WEATHER_GCFG_SNOW_L = { 10, 32, 16, 180, 200, false };

uint8_t WEATHER_MASK_SNOW_H[SYMBOL_W * 7] = {
  0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,
  0,1,1,1,0,0,0,0,0,1,0,0,0,0,0,0,1,1,1,0,0,0,0,0,
  0,0,1,0,0,0,0,0,1,1,1,0,0,0,0,0,0,1,0,0,0,0,0,0,
  0,0,0,0,0,1,0,0,0,1,0,0,0,0,0,0,0,0,0,0,1,0,0,0,
  0,0,0,0,1,1,1,0,0,0,0,0,0,1,0,0,0,0,0,1,1,1,0,0,
  0,0,0,0,0,1,0,0,0,0,0,0,1,1,1,0,0,0,0,0,1,0,0,0,
  0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,
};
pixel_mask_t WEATHER_PMASK_SNOW_H = {WEATHER_MASK_SNOW_H, SYMBOL_W, 7};
gradient_config_t WEATHER_GCFG_SNOW_H = { 10, 32, 16, 180, 200, false };

uint8_t WEATHER_MASK_SLEET_L[SYMBOL_W * 4] = {
  0,0,0,0,0,0,0,0,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
  0,0,0,1,1,0,0,0,1,1,0,0,0,0,0,0,0,0,1,1,0,0,0,0,
  0,0,0,1,1,0,0,0,0,0,0,0,0,1,1,0,0,0,1,1,0,0,0,0,
  0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,0,0,0,0,0,0,0,0,0,
};
pixel_mask_t WEATHER_PMASK_SLEET_L = {WEATHER_MASK_SLEET_L, SYMBOL_W, 4};
gradient_config_t WEATHER_GCFG_SLEET_L = { 10, 32, 16, 210, 230, false };

uint8_t WEATHER_MASK_SLEET_H[SYMBOL_W * 6] = {
  0,0,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,0,0,0,0,0,
  0,0,1,1,0,0,0,0,0,1,1,0,0,0,0,0,0,1,1,0,0,0,0,0,
  0,0,0,0,0,0,0,0,0,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,
  0,0,0,0,0,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,0,0,
  0,0,0,0,0,1,1,0,0,0,0,0,0,1,1,0,0,0,0,0,1,1,0,0,
  0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,0,0,0,0,0,0,0,0,0,
};
pixel_mask_t WEATHER_PMASK_SLEET_H = {WEATHER_MASK_SLEET_H, SYMBOL_W, 6};
gradient_config_t WEATHER_GCFG_SLEET_H = { 10, 32, 16, 210, 230, false };

uint8_t WEATHER_MASK_LIGHTNING[8 * 14] = {
  0,0,0,1,1,1,0,0,
  0,0,0,1,1,1,0,0,
  0,0,1,1,1,1,0,0,
  0,0,1,1,1,0,0,0,
  0,0,1,1,1,0,0,0,
  0,1,1,1,1,0,0,0,
  0,1,1,1,1,1,1,1,
  1,1,1,1,1,1,1,1,
  1,1,1,1,1,1,1,0,
  0,0,0,1,1,1,0,0,
  0,0,0,1,1,0,0,0,
  0,0,0,1,0,0,0,0,
  0,0,1,1,0,0,0,0,
  0,0,1,0,0,0,0,0,
};
pixel_mask_t WEATHER_PMASK_LIGHTNING = {WEATHER_MASK_LIGHTNING, 8, 14};
gradient_config_t WEATHER_GCFG_LIGHTNING = { 10, 32, 16, 40, 70, false };

uint8_t WEATHER_MASK_FOG[SYMBOL_W * 13] = {
  0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0,
  0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,
  0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0,
  0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
  0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
  0,0,0,0,0,0,1,1,1,1,1,1,0,0,0,0,1,1,1,1,1,1,1,0,
  0,0,0,0,0,1,1,1,1,1,1,1,1,0,0,1,1,1,1,1,1,1,1,1,
  0,0,0,0,0,0,1,1,1,1,1,1,0,0,0,0,1,1,1,1,1,1,1,0,
  0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
  0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
  0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0,0,
  0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0,
  0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0,0,
};
pixel_mask_t WEATHER_PMASK_FOG = {WEATHER_MASK_FOG, SYMBOL_W, 13};
gradient_config_t WEATHER_GCFG_FOG = { 10, 32, 16, 200, 220, false };

WeatherSymbol::WeatherSymbol(Gfx *gfx_p) {
  gfx = gfx_p;
}

void WeatherSymbol::drawSymbolInterior(void) {
  gfx->drawCircularGradientMask(&WEATHER_PMASK_INT, x, y, &WEATHER_GCFG_INT);
}

void WeatherSymbol::drawSymbol(weather_code_t code) {
  switch (code) {
    case WEATHER_CODE_CLEAR:
      //todo: moon by time of day
      gfx->drawCircularGradientMask(&WEATHER_PMASK_SUN, x, y, &WEATHER_GCFG_SUN);
      return;
    case WEATHER_CODE_PARTLY_CLOUDY:
      //todo: moon by time of day
      gfx->drawCircularGradientMask(&WEATHER_PMASK_SUN_S, x + 3, y, &WEATHER_GCFG_SUN_S);
      gfx->drawCircularGradientMask(&WEATHER_PMASK_CLOUD_S, x + 8, y + 7, &WEATHER_GCFG_CLOUD_S);
      return;
    case WEATHER_CODE_CLOUDY:
    case WEATHER_CODE_OVERCAST:
      gfx->drawCircularGradientMask(&WEATHER_PMASK_CLOUD, x, y + 4, &WEATHER_GCFG_CLOUD);
      return;
    case WEATHER_CODE_FOG:
    case WEATHER_CODE_MIST:
    case WEATHER_CODE_FREEZING_FOG:
      gfx->drawCircularGradientMask(&WEATHER_PMASK_FOG, x, y + 8, &WEATHER_GCFG_FOG);
      return;
    case WEATHER_CODE_LIGHT_RAIN:
    case WEATHER_CODE_PATCHY_RAIN_POSSIBLE:
    case WEATHER_CODE_PATCHY_LIGHT_RAIN:
    case WEATHER_CODE_PATCHY_LIGHT_DRIZZLE:
    case WEATHER_CODE_LIGHT_RAIN_SHOWER:
    case WEATHER_CODE_MODERATE_RAIN_AT_TIMES:
      gfx->drawCircularGradientMask(&WEATHER_PMASK_CLOUD, x, y + 2, &WEATHER_GCFG_CLOUD);
      gfx->drawCircularGradientMask(&WEATHER_PMASK_RAIN_L, x, y + 18, &WEATHER_GCFG_RAIN_L);
      return;
    case WEATHER_CODE_HEAVY_RAIN:
    case WEATHER_CODE_MODERATE_OR_HEAVY_RAIN_SHOWER:
    case WEATHER_CODE_TORRENTIAL_RAIN_SHOWER:
    case WEATHER_CODE_HEAVY_RAIN_AT_TIMES:
      gfx->drawCircularGradientMask(&WEATHER_PMASK_CLOUD, x, y + 2, &WEATHER_GCFG_CLOUD);
      gfx->drawCircularGradientMask(&WEATHER_PMASK_RAIN_H, x, y + 17, &WEATHER_GCFG_RAIN_H);
      return;
    case WEATHER_CODE_LIGHT_SNOW:
    case WEATHER_CODE_LIGHT_SNOW_SHOWERS:
    case WEATHER_CODE_PATCHY_SNOW_POSSIBLE:
    case WEATHER_CODE_PATCHY_LIGHT_SNOW:
    case WEATHER_CODE_PATCHY_MODERATE_SNOW:
      gfx->drawCircularGradientMask(&WEATHER_PMASK_CLOUD, x, y + 2, &WEATHER_GCFG_CLOUD);
      gfx->drawCircularGradientMask(&WEATHER_PMASK_SNOW_L, x, y + 18, &WEATHER_GCFG_SNOW_L);
      return;
    case WEATHER_CODE_HEAVY_SNOW:
    case WEATHER_CODE_MODERATE_SNOW:
    case WEATHER_CODE_MODERATE_OR_HEAVY_SNOW_SHOWERS:
    case WEATHER_CODE_PATCHY_HEAVY_SNOW:
    case WEATHER_CODE_BLOWING_SNOW:
    case WEATHER_CODE_BLIZZARD:
      gfx->drawCircularGradientMask(&WEATHER_PMASK_CLOUD, x, y + 2, &WEATHER_GCFG_CLOUD);
      gfx->drawCircularGradientMask(&WEATHER_PMASK_SNOW_H, x, y + 17, &WEATHER_GCFG_SNOW_H);
      return;
    case WEATHER_CODE_LIGHT_SLEET:
    case WEATHER_CODE_LIGHT_SLEET_SHOWERS:
    case WEATHER_CODE_PATCHY_SLEET_POSSIBLE:
    case WEATHER_CODE_PATCHY_FREEZING_DRIZZLE_POSSIBLE:
    case WEATHER_CODE_LIGHT_FREEZING_RAIN:
    case WEATHER_CODE_ICE_PELLETS:
    case WEATHER_CODE_LIGHT_SHOWERS_OF_ICE_PELLETS:
      gfx->drawCircularGradientMask(&WEATHER_PMASK_CLOUD, x, y + 2, &WEATHER_GCFG_CLOUD);
      gfx->drawCircularGradientMask(&WEATHER_PMASK_SLEET_L, x, y + 18, &WEATHER_GCFG_SLEET_L);
      return;
    case WEATHER_CODE_MODERATE_OR_HEAVY_SLEET:
    case WEATHER_CODE_MODERATE_OR_HEAVY_SLEET_SHOWERS:
    case WEATHER_CODE_FREEZING_DRIZZLE:
    case WEATHER_CODE_HEAVY_FREEZING_DRIZZLE:
    case WEATHER_CODE_MODERATE_OR_HEAVY_FREEZING_RAIN:
    case WEATHER_CODE_MODERATE_OR_HEAVY_SHOWERS_OF_ICE_PELLETS:
      gfx->drawCircularGradientMask(&WEATHER_PMASK_CLOUD, x, y + 2, &WEATHER_GCFG_CLOUD);
      gfx->drawCircularGradientMask(&WEATHER_PMASK_SLEET_H, x, y + 17, &WEATHER_GCFG_SLEET_H);
      return;
    case WEATHER_CODE_THUNDERY_OUTBREAKS_POSSIBLE:
    case WEATHER_CODE_PATCHY_LIGHT_RAIN_WITH_THUNDER:
    case WEATHER_CODE_MODERATE_OR_HEAVY_RAIN_WITH_THUNDER:
    case WEATHER_CODE_PATCHY_LIGHT_SNOW_WITH_THUNDER:
    case WEATHER_CODE_MODERATE_OR_HEAVY_SNOW_WITH_THUNDER:
      gfx->drawCircularGradientMask(&WEATHER_PMASK_CLOUD, x, y + 2, &WEATHER_GCFG_CLOUD);
      gfx->drawCircularGradientMask(&WEATHER_PMASK_LIGHTNING, x + 7, y + 6, &WEATHER_GCFG_LIGHTNING);
      return;
  }
  gfx->drawCircularGradientMask(&WEATHER_PMASK_UNKNOWN, x, y, &WEATHER_GCFG_UNKNOWN);
}
