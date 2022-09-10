#ifndef H_CRYPTID_TEMP
#define H_CRYPTID_TEMP

#include "cryptid-types.h"
#include "cryptid-gfx.h"

class TemperatureDisplay {
  public:
    /**
     * @brief Construct a new TemperatureDisplay object
     * 
     * @param gfx_p A pointer to the graphics object.
     */
    TemperatureDisplay(Gfx *gfx_p);

    /**
     * @brief Update the temperature image (run each frame).
     *
     * @param newTemperature The temperature in degrees fahrenheit.
     * @param small          Whether to draw it small or large.
     */
    void update(float newTemperature, bool small = false);

  private:
    /**
     * @brief A pointer to the graphics object.
     */
    Gfx* gfx;

    /**
     * @brief The gradient config struct.
     */
    gradient_config_t gradient_config;

    /**
     * @brief The current temperature.
     */
    uint8_t temperature;

    /**
     * @brief x-coord to draw on pixels[] grid.
     */
    uint8_t xStart;

    /**
     * @brief y-coord to draw on pixels[] grid.
     */
    uint8_t yStart;
};

#endif
