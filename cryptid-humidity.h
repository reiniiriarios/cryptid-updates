#ifndef H_CRYPTID_HUMIDITY
#define H_CRYPTID_HUMIDITY

#include "cryptid-types.h"
#include "cryptid-gfx.h"

class HumidityDisplay {
  public:
    /**
     * @brief Construct a new HumidityDisplay object
     * 
     * @param gfx_p A pointer to the graphics object.
     */
    HumidityDisplay(Gfx *gfx_p);

    /**
     * @brief Update the humidity image (run each frame).
     *
     * @param newHumidity The relative humidity (percentage).
     * @param small       Whether to draw it small or large.
     */
    void update(float newHumidity, bool small = false);

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
     * @brief The current humidity.
     */
    uint8_t humidity;

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
