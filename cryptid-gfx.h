#ifndef CRYPTID_GFX
#define CRYPTID_GFX

#include <Adafruit_Protomatter.h>
#include "cryptid-types.h"
#include "cryptid-config.h"

class Gfx {
  public:
    /**
     * @brief A pointer to the Protomatter matrix object.
     */
    Adafruit_Protomatter* matrix;

    /**
     * @brief A representation of the pixel grid, abstracted away from Protomatter.
     */
    pixel_t pixels[MATRIX_HEIGHT][MATRIX_WIDTH] = {};

    /**
     * @brief Construct a new Gfx object.
     * 
     * @param matrix_p Pointer to a Protomatter matrix object.
     */
    Gfx(Adafruit_Protomatter* matrix_p);

    /**
     * @brief Write the pixels[] data to the Protomatter matrix
     */
    void drawPixels(void);
  
    /**
     * @brief Build one frame of a gradient based on an image mask + config.
     * 
     * @param image The gradient image mask + config to build the gradient on.
     */
    void buildCircularGradient(gradient_image_t* image);

    /**
     * @brief Build one pixel in a gradient.
     * 
     * @param x    The x-coord on pixels[].
     * @param y    The y-coord on pixels[].
     * @param cfg  The gradient config.
     */
    void buildCircularGradientPixel(uint8_t x, uint8_t y, gradient_config_t* cfg);

    /**
     * @brief Draw a float as characters on pixels[] with a gradient.
     * 
     * @param number           The number to draw.
     * @param x                The x-coord on pixels[].
     * @param y                The y-coord on pixels[].
     * @param gradient_config  The gradient config.
     */
    void drawNumber(float number, uint8_t x, uint8_t y, gradient_config_t* gradient_config);

    /**
     * @brief Draw an integer as characters on pixels[] with a gradient.
     * 
     * @param number           The number to draw.
     * @param x                The x-coord on pixels[].
     * @param y                The y-coord on pixels[].
     * @param gradient_config  The gradient config.
     */
    void drawNumber(uint8_t number, uint8_t x, uint8_t y, gradient_config_t* gradient_config);

    /**
     * @brief Draw a single character as characters on pixels[] with a gradient.
     * 
     * @param c                The character to draw.
     * @param x                The x-coord on pixels[].
     * @param y                The y-coord on pixels[].
     * @param gradient_config  The gradient config.
     */
    uint8_t drawChar(char c, uint8_t x, uint8_t y, gradient_config_t* gradient_config);

    /**
     * @brief Build a mask from a single character.
     * 
     * @param c  The character to build a mask from.
     * @return pixel_mask_t 
     */
    pixel_mask_t buildMaskFromChar(unsigned char c);

    /**
     * @brief Fill a mask based on a single character.
     * 
     * @param c           The character to build the mask from.
     * @param pixel_mask  The mask to use.
     * @return uint8_t    The width of the character (used to position additional characters).
     */
    uint8_t fillMaskFromChar(unsigned char c, pixel_mask_t* pixel_mask);
};


#endif
