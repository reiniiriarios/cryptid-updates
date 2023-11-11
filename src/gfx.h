#ifndef H_CRYPTID_GFX
#define H_CRYPTID_GFX

#include <Adafruit_Protomatter.h>
#include "types.h"
#include "config.h"

class Gfx {
  public:
    /**
     * @brief A pointer to the Protomatter matrix object.
     */
    Adafruit_Protomatter *matrix;

    /**
     * @brief A representation of the pixel grid, abstracted away from Protomatter.
     */
    pixel_t pixels[MATRIX_HEIGHT][MATRIX_WIDTH] = {};

    /**
     * @brief Construct a new Gfx object.
     * 
     * @param matrix_p Pointer to a Protomatter matrix object.
     */
    Gfx(Adafruit_Protomatter *matrix_p);

    /**
     * @brief Write the pixels[] data to the Protomatter matrix.
     */
    void toBuffer(void);

    /**
     * @brief Build one pixel in a circular gradient.
     * 
     * @param x    The x-coord on pixels[].
     * @param y    The y-coord on pixels[].
     * @param cfg  The gradient config.
     */
    void drawCircularGradientPixel(uint8_t x, uint8_t y, gradient_config_t *cfg);
  
    /**
     * @brief Draw a pixel mask on pixels[] with a gradient.
     * 
     * @param mask  The pixel mask to draw.
     * @param x     The x-coord on pixels[].
     * @param y     The y-coord on pixels[].
     * @param cfg   The gradient config.
     * @return      Width of image drawn in pixels.
     */
    uint8_t drawCircularGradientMask(pixel_mask_t *mask, uint8_t x, uint8_t y, gradient_config_t *cfg);

    /**
     * @brief Draw a float (rounded) on pixels[] with a gradient, using masks.
     * 
     * @param n    The number to draw.
     * @param x    The x-coord on pixels[].
     * @param y    The y-coord on pixels[].
     * @param cfg  The gradient config.
     * @return     Width of image drawn in pixels.
     */
    uint8_t drawCircularGradientMask(float n, uint8_t x, uint8_t y, gradient_config_t *cfg);

    /**
     * @brief Draw an integer on pixels[] with a gradient, using masks.
     * 
     * @param n    The number to draw.
     * @param x    The x-coord on pixels[].
     * @param y    The y-coord on pixels[].
     * @param cfg  The gradient config.
     * @return     Width of image drawn in pixels.
     */
    uint8_t drawCircularGradientMask(uint8_t n, uint8_t x, uint8_t y, gradient_config_t *cfg);

    /**
     * @brief Draw a float (rounded) on pixels[] with a gradient, using a font.
     * 
     * @param n    The number to draw.
     * @param x    The x-coord on pixels[].
     * @param y    The y-coord on pixels[].
     * @param cfg  The gradient config.
     * @return     Width of image drawn in pixels.
     */
    uint8_t drawCircularGradientFont(float n, uint8_t x, uint8_t y, gradient_config_t *cfg);

    /**
     * @brief Draw an integer on pixels[] with a gradient, using a font.
     * 
     * @param n    The number to draw.
     * @param x    The x-coord on pixels[].
     * @param y    The y-coord on pixels[].
     * @param cfg  The gradient config.
     * @return    Width of image drawn in pixels.
     */
    uint8_t drawCircularGradientFont(uint8_t n, uint8_t x, uint8_t y, gradient_config_t *cfg);

    /**
     * @brief Draw a string on pixels[] with a gradient, using a font.
     * 
     * @param s    The string to draw.
     * @param x    The x-coord on pixels[].
     * @param y    The y-coord on pixels[].
     * @param cfg  The gradient config.
     * @return     Width of image drawn in pixels.
     */
    uint8_t drawCircularGradientFont(String s, uint8_t x, uint8_t y, gradient_config_t *cfg);

    /**
     * @brief Draw a character on pixels[] with a gradient.
     * 
     * @param c    The character to draw.
     * @param x    The x-coord on pixels[].
     * @param y    The y-coord on pixels[].
     * @param cfg  The gradient config.
     * @return     Width of character drawn in pixels.
     */
    uint8_t drawCircularGradientFont(char c, uint8_t x, uint8_t y, gradient_config_t *cfg);

    /**
     * @brief Convert value to hue by scaling the value to a different range.
     *
     * @param value    The value.
     * @param minFrom  Bottom range of values.
     * @param maxFrom  Upper range of values.
     * @param minTo    Bottom range of hues.
     * @param maxTo    Upper range of hues.
     * @return         Hue.
     */
    uint16_t value2hue(int value, int minFrom, int maxFrom, int minTo, int maxTo);

    /**
     * @brief Draw error pixel(s) to denote WiFi is down.
     */
    void drawErrorWiFi(void);

    /**
     * @brief Draw error pixel(s) to denote MQTT is down.
     */
    void drawErrorMqtt(void);

  private:

    uint16_t getHueBounce(int start, int end);
};

#endif
