#ifndef CRYPTID_TYPES_H
#define CRYPTID_TYPES_H

  /**
   * @brief A representation of one pixel.
   */
  typedef struct pixel_t {
    bool     on      = false; /**< Whether to draw the pixel at all. */
    uint8_t  opacity = 100;   /**< Opacity, [0-100] */
    uint16_t hue;             /**< Hue in degrees, [0-360] */
  } pixel_t;

  /**
   * @brief A mask representing where to draw pixels.
   *
   * *mask must point to the first element of a 1d array of values
   * the size matching width * height.
   */
  typedef struct pixel_mask_t {
    uint8_t *mask;   /** A pointer to the first element in a mask array. */
    uint8_t width;   /** Width of the mask. */
    uint8_t height;  /** Height of the mask. */
  } pixel_mask_t;

  /**
   * @brief Use this struct to build a config for generating a gradient.
   */
  typedef struct gradient_config_t {
    /**
     * How fast the gradient animates.
     * 1 = very slow, 10 = steady, 40 = uncomfortably fast
     * Set this high at your own risk (seizures, etc).
     */
    uint8_t  animation_speed  = 10;

    /** Size of gradient within shapes. No direct correlation to pixels. */
    uint8_t  gradient_width   = 32;

    /** Size of the shapes, no direct correlation to pixels */
    uint8_t  shape_width      = 16;

    /** Starting color of the gradient, in degrees, 0-360 */
    uint16_t gradient_start   = 260;

    /** End color of the gradient, in degrees, 0-360 */
    uint16_t gradient_end     = 350;

    /** Whether to draw the gradient clockwise or counterclockwise */
    boolean  gradient_reverse = false;

    /** Hue, 0-65535. Set within functions. */
    float gradient_start_scaled = 47330.8333;
    float gradient_end_scaled = 63714.5833;

  } gradient_config_t;

  /**
   * @brief A pixel_mask_t and gradient_config_t as a single entity with placement on screen.
   */
  typedef struct gradient_image_t {
    pixel_mask_t mask;         /** A pixel mask struct. */
    gradient_config_t config;  /** A gradient config struct. */
    uint8_t x;                 /** The x-coord on pixels[]. */
    uint8_t y;                 /** The y-coord on pixels[]. */
  } gradient_mask_t;

#endif
