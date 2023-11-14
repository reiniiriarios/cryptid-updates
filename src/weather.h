#ifndef CRYPTID_WEATHER_SYMBOL_H
#define CRYPTID_WEATHER_SYMBOL_H

#include <map>

#include "types.h"
#include "gfx.h"

/**
 * @brief Weather symbols.
 */
typedef enum {
  WEATHER_SYMBOL_INT = -1,
  WEATHER_SYMBOL_UNKNOWN = 0,
  WEATHER_SYMBOL_SUN = 1,
  WEATHER_SYMBOL_MOON = 2,
  WEATHER_SYMBOL_CLOUDY = 3,
  WEATHER_SYMBOL_CLOUDY_SUN = 4,
  WEATHER_SYMBOL_CLOUDY_MOON = 5,
  WEATHER_SYMBOL_RAIN = 8,
  WEATHER_SYMBOL_SNOW = 9,
  WEATHER_SYMBOL_SLEET = 10,
  WEATHER_SYMBOL_LIGHTNING = 11,
  WEATHER_SYMBOL_FOG = 12,
} weather_symbol_t;

/**
 * @brief Weather condition codes from Weather API.
 *
 * @see https://www.weatherapi.com/docs/weather_conditions.json
 */
typedef enum {
  WEATHER_CODE_UNKNOWN = 0,
  WEATHER_CODE_CLEAR = 1000,
  WEATHER_CODE_PARTLY_CLOUDY = 1003,
  WEATHER_CODE_CLOUDY = 1006,
  WEATHER_CODE_OVERCAST = 1009,
  WEATHER_CODE_MIST = 1030,
  WEATHER_CODE_PATCHY_RAIN_POSSIBLE = 1063,
  WEATHER_CODE_PATCHY_SNOW_POSSIBLE = 1066,
  WEATHER_CODE_PATCHY_SLEET_POSSIBLE = 1069,
  WEATHER_CODE_PATCHY_FREEZING_DRIZZLE_POSSIBLE = 1072,
  WEATHER_CODE_THUNDERY_OUTBREAKS_POSSIBLE = 1087,
  WEATHER_CODE_BLOWING_SNOW = 1114,
  WEATHER_CODE_BLIZZARD = 1117,
  WEATHER_CODE_FOG = 1135,
  WEATHER_CODE_FREEZING_FOG = 1147,
  WEATHER_CODE_PATCHY_LIGHT_DRIZZLE = 1150,
  WEATHER_CODE_LIGHT_DRIZZLE = 1153,
  WEATHER_CODE_FREEZING_DRIZZLE = 1168,
  WEATHER_CODE_HEAVY_FREEZING_DRIZZLE = 1171,
  WEATHER_CODE_PATCHY_LIGHT_RAIN = 1180,
  WEATHER_CODE_LIGHT_RAIN = 1183,
  WEATHER_CODE_MODERATE_RAIN_AT_TIMES = 1186,
  WEATHER_CODE_MODERATE_RAIN = 1189,
  WEATHER_CODE_HEAVY_RAIN_AT_TIMES = 1192,
  WEATHER_CODE_HEAVY_RAIN = 1195,
  WEATHER_CODE_LIGHT_FREEZING_RAIN = 1198,
  WEATHER_CODE_MODERATE_OR_HEAVY_FREEZING_RAIN = 1201,
  WEATHER_CODE_LIGHT_SLEET = 1204,
  WEATHER_CODE_MODERATE_OR_HEAVY_SLEET = 1207,
  WEATHER_CODE_PATCHY_LIGHT_SNOW = 1210,
  WEATHER_CODE_LIGHT_SNOW = 1213,
  WEATHER_CODE_PATCHY_MODERATE_SNOW = 1216,
  WEATHER_CODE_MODERATE_SNOW = 1219,
  WEATHER_CODE_PATCHY_HEAVY_SNOW = 1222,
  WEATHER_CODE_HEAVY_SNOW = 1225,
  WEATHER_CODE_ICE_PELLETS = 1237,
  WEATHER_CODE_LIGHT_RAIN_SHOWER = 1240,
  WEATHER_CODE_MODERATE_OR_HEAVY_RAIN_SHOWER = 1243,
  WEATHER_CODE_TORRENTIAL_RAIN_SHOWER = 1246,
  WEATHER_CODE_LIGHT_SLEET_SHOWERS = 1249,
  WEATHER_CODE_MODERATE_OR_HEAVY_SLEET_SHOWERS = 1252,
  WEATHER_CODE_LIGHT_SNOW_SHOWERS = 1255,
  WEATHER_CODE_MODERATE_OR_HEAVY_SNOW_SHOWERS = 1258,
  WEATHER_CODE_LIGHT_SHOWERS_OF_ICE_PELLETS = 1261,
  WEATHER_CODE_MODERATE_OR_HEAVY_SHOWERS_OF_ICE_PELLETS = 1264,
  WEATHER_CODE_PATCHY_LIGHT_RAIN_WITH_THUNDER = 1273,
  WEATHER_CODE_MODERATE_OR_HEAVY_RAIN_WITH_THUNDER = 1276,
  WEATHER_CODE_PATCHY_LIGHT_SNOW_WITH_THUNDER = 1279,
  WEATHER_CODE_MODERATE_OR_HEAVY_SNOW_WITH_THUNDER = 1282,
} weather_code_t;

/**
 * @brief The current weather.
 */
typedef struct weather_t {
  float temp_c = 0;
  float temp_f = 0;
  float feelslike_c = 0;
  float feelslike_f = 0;
  uint8_t humidity = 0;
  weather_code_t code = WEATHER_CODE_UNKNOWN;
  int temp_last = 0;
  int feelslike_last = 0;
  int humidity_last = 0;
  int code_last = 0;
} weather_t;

class WeatherSymbol {
  public:
    /**
     * @brief Construct a new WeatherSymbol object
     * 
     * @param gfx_p A pointer to the graphics object.
     */
    WeatherSymbol(Gfx *gfx_p);

    /**
     * @brief Update the gradient image (run each frame).
     */
    void update(weather_code_t code);

    /**
     * @brief Update the gradient image (run each frame).
     */
    void update(weather_symbol_t symbol);

    /**
     * @brief Update the gradient image (run each frame).
     */
    void updateInterior(void);

    /**
     * @brief Get symbol from condition code.
     */
    weather_symbol_t getSymbol(weather_code_t code);

  private:
    /**
     * @brief A pointer to the graphics object.
     */
    Gfx *gfx;
};

#endif
