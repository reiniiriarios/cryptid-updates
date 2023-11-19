#ifndef CRYPTID_AQI_H
#define CRYPTID_AQI_H

#include "gfx.h"

class AQI {
  public:
    /**
     * @brief Construct a new Air Quality Index object
     * 
     * @param gfx_p A pointer to the graphics object.
     */
    AQI(Gfx *gfx_p);

    /**
     * @brief Update the image (run each frame).
     *
     * @param aqiValue
     */
    void update(uint16_t aqiValue);

  private:
    /**
     * @brief A pointer to the graphics object.
     */
    Gfx *gfx;

    /**
     * @brief The gradient config struct.
     */
    gradient_config_t gradient_config = gradient_config_t(10, 32, 16, 0, 15, false);

    /**
     * @brief Set gradient based on index.
     * 
     * @param aqiValue
     */
    void setGradient(uint16_t aqiValue);

    /**
     * @brief X position
     */
    const uint8_t x = 41;

    /**
     * @brief Y position
     */
    const uint8_t y = 25;

    /**
     * @brief The pixel mask for the prefix.
     * 
     */
    pixel_mask_t pixel_mask = {
      new uint8_t[6 * 6] {
        0,0,0,0,1,0,
        0,1,1,0,0,1,
        1,0,0,1,0,1,
        1,0,1,0,0,1,
        1,0,0,0,1,0,
        0,1,1,1,0,0,
      },
      6, 6
    };
};

#endif
