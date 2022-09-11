#ifndef CRYPTID_TYPES_H
#define CRYPTID_TYPES_H

/**
 * @brief A representation of one pixel.
 */
typedef struct pixel_t {
  bool     on      = false;  /** Whether to draw the pixel at all. */
  uint8_t  opacity = 100;    /** Opacity, [0-100] */
  uint16_t hue;              /** Hue in degrees, [0-360] */
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
  uint8_t  animation_speed;

  /** Size of gradient within shapes. No direct correlation to pixels. */
  uint8_t  gradient_width;

  /** Size of the shapes, no direct correlation to pixels */
  uint8_t  shape_width;

  /** Starting color of the gradient, in degrees, 0-360 */
  uint16_t gradient_start;

  /** End color of the gradient, in degrees, 0-360 */
  uint16_t gradient_end;

  /** Whether to draw the gradient clockwise or counterclockwise */
  bool  gradient_reverse;

  /** Hue, 0-65535. */
  float gradient_start_scaled;

  /** Hue, 0-65535. */
  float gradient_end_scaled;

  gradient_config_t(
    unsigned int animation_speed = 10,
    unsigned int gradient_width = 32,
    unsigned int shape_width = 16,
    unsigned int gradient_start = 260,
    unsigned int gradient_end = 350,
    unsigned int gradient_reverse = false
  ) :
    animation_speed((uint8_t)animation_speed),
    gradient_width((uint8_t)gradient_width),
    shape_width((uint8_t)shape_width),
    gradient_start((uint8_t)gradient_start),
    gradient_end((uint8_t)gradient_end),
    gradient_reverse((bool)gradient_reverse)
  {
    // scale the start/end values to a useful value to compute hue for Protomatter (0-65535)
    gradient_start_scaled = gradient_start / 360.0f * 65535.0f;
    gradient_end_scaled = gradient_end / 360.0f * 65535.0f;

    // normalize hues
    while (gradient_start >= 360) gradient_start -= 360;
    while (gradient_end >= 360) gradient_end -= 360;
  }

  void setStart(uint16_t hue) {
    gradient_start = hue;
    gradient_start_scaled = gradient_start / 360.0f * 65535.0f;
  }

  void setEnd(uint16_t hue) {
    gradient_end = hue;
    gradient_end_scaled = gradient_end / 360.0f * 65535.0f;
  }
} gradient_config_t;

#endif
