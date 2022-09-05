#ifndef CRYPTID_TEMP
#define CRYPTID_TEMP

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
       */
      void update(float newTemperature);

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
       * @brief Pixel masks for each digit.
       */
      pixel_mask_t temp_mask[3];

      /**
       * @brief The image structs for each digit.
       */
      gradient_image_t gradient_image[3];

      /**
       * @brief The current temperature.
       */
      uint8_t temperature;

      /**
       * @brief The current temperature (in characters).
       */
      String temperatureChars = "000";

      /**
       * @brief Font width.
       */
      uint8_t font_width = 9;

      /**
       * @brief Font height.
       */
      uint8_t font_height = 9;

      /**
       * @brief Font width * font height.
       */
      int drawCharSize;

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
