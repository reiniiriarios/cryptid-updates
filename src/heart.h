#ifndef H_CRYPTID_HEART
#define H_CRYPTID_HEART

#include "types.h"
#include "gfx.h"

class Heart {
  public:
    /**
     * @brief Construct a new Heart object
     * 
     * @param gfx_p A pointer to the graphics object.
     */
    Heart(Gfx *gfx_p);

    /**
     * @brief Update the heart gradient image (run each frame).
     */
    void update(void);

  private:
    /**
     * @brief A pointer to the graphics object.
     */
    Gfx *gfx;

    /**
     * @brief The mask struct.
     */
    pixel_mask_t pixel_mask;

    /**
     * @brief The gradient config struct.
     */
    gradient_config_t gradient_config;

    /**
     * @brief The mask array.
     */
    uint8_t mask[11 * 11] = {
      0, 0, 1, 1, 0, 0, 0, 1, 1, 0, 0,
      0, 1, 1, 1, 1, 0, 1, 1, 1, 1, 0,
      1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
      1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
      1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
      1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
      0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0,
      0, 0, 1, 1, 1, 1, 1, 1, 1, 0, 0,
      0, 0, 0, 1, 1, 1, 1, 1, 0, 0, 0,
      0, 0, 0, 0, 1, 1, 1, 0, 0, 0, 0,
      0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0,
    };
};

#endif
