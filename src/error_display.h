#ifndef CRYPTID_ERROR_DISPLAY_H
#define CRYPTID_ERROR_DISPLAY_H

#include "gfx.h"

class ErrorDisplay {
  public:
    /**
     * @brief Construct a new ErrorDisplay object
     * 
     * @param gfx_p A pointer to the graphics object.
     */
    ErrorDisplay(Gfx *gfx_p);

    /**
     * @brief Update the error image (run each frame).
     */
    void update(int errorCode);

  private:
    /**
     * @brief A pointer to the graphics object.
     */
    Gfx *gfx;

    /**
     * @brief The gradient config struct.
     */
    gradient_config_t gradient_config;

    /**
     * @brief The pixel mask for the error prefix.
     * 
     */
    pixel_mask_t pixel_mask;

    /**
     * @brief The mask array.
     */
    uint8_t mask[11 * 5] = {
      1, 1, 1, 0, 1, 1, 0, 0, 1, 1, 0,
      1, 0, 0, 0, 1, 0, 1, 0, 1, 0, 1,
      1, 1, 1, 0, 1, 1, 0, 0, 1, 1, 0,
      1, 0, 0, 0, 1, 0, 1, 0, 1, 0, 1,
      1, 1, 1, 0, 1, 0, 1, 0, 1, 0, 1,
    };

    /**
     * @brief X position
     */
    const uint8_t x = 20;

    /**
     * @brief Y position
     */
    const uint8_t y = 13;
};

#endif
