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
 * *mask must point to the first element of a 1d array of values, the size matching width * height.
 */
typedef struct pixel_mask_t {
  uint8_t *mask;   /**< A pointer to the first element in a mask array. */
  uint8_t width;   /**< Width of the mask. */
  uint8_t height;  /**< Height of the mask. */
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

} gradient_config_t;

/**
 * Blink onboard LED and print message over serial.
 * 
 * @param milliseconds How fast to blink the LED.
 * @param message      Error message.
 */
void err(int milliseconds, String message);

/**
 * Run once on start.
 */
void setup(void);

/**
 * @brief Main loop.
 */
void loop(void);

/**
 * @brief Write the pixels[] data to the Protomatter matrix.
 */
void drawPixels(void);

/**
 * @brief Generate pretty colors to the pixels[] array.
 * 
 * @param mask   The mask to draw within.
 * @param xStart Where to start drawing on the pixels[] grid.
 * @param yStart Where to start drawing on the pixels[] grid.
 * @param cfg    Config for this gradient.
 */
void buildCircularGradientFromMask(
  pixel_mask_t      mask,
  uint8_t           xStart,
  uint8_t           yStart,
  gradient_config_t cfg
);

/**
 * @brief Draw a heart.
 * 
 * @param xStart x-coord of pixels[] grid.
 * @param yStart y-coord of pixels[] grid.
 */
void drawHeart(uint8_t xStart, uint8_t yStart);

/**
 * @brief Build a pixel_mask from a single character.
 * 
 * Adapted from Adafruit_GFX::drawChar()
 * 
 * @param c The character to create a mask from.
 * @return pixel_mask_t 
 */
pixel_mask_t buildMaskFromChar(unsigned char c);

/**
 * @brief Draw the temperature.
 * 
 * @param temp Temperature to display.
 * @param x    x-coord to draw at on pixels[] grid.
 * @param y    y-coord to draw at on pixels[] grid.
 */
void drawTemperature(float temp, uint8_t x, uint8_t y);

/**
 * @brief Draw a number based on gradient_config. Number will be rounded.
 * 
 * @param number           The number to draw.
 * @param x                x-coord to draw at on pixels[] grid.
 * @param y                y-coord to draw at on pixels[] grid.
 * @param gradient_config  Config for gradient.
 */
void drawNumber(float number, uint8_t x, uint8_t y, gradient_config_t gradient_config);

/**
 * @brief Draw a number based on gradient_config.
 * 
 * @param number           The number to draw.
 * @param x                x-coord to draw at on pixels[] grid.
 * @param y                y-coord to draw at on pixels[] grid.
 * @param gradient_config  Config for gradient.
 */
void drawNumber(uint8_t number, uint8_t x, uint8_t y, gradient_config_t gradient_config);

/**
 * @brief Convert celcius to fahrenheit.
 * 
 * @param celsius degrees
 * @return float fahrenheit
 */
float celsius2fahrenheit(float celsius);
