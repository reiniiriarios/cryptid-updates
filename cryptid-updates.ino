//
// CRYPTID UPDATES - LED Matrix Display
//

// INCLUDES ----------------------------------------------------------------------------------------

#include "cryptid-updates.h"

// Filesystem
// #include <Adafruit_FlashCache.h>
// #include <Adafruit_FlashTransport.h>
// #include <Adafruit_SPIFlash.h>
// #include <Adafruit_SPIFlashBase.h>
// #include <flash_devices.h>

// Read bitmaps from SD
// #include <Adafruit_ImageReader_EPD.h>
// #include <Adafruit_ImageReader.h>

// Status LED
#include <Adafruit_NeoPixel.h>

// Accelerometer
#include <Adafruit_LIS3DH.h>

// Temperature & Humidity Sensor
#include <Adafruit_SHT4x.h>

// RGB Matrix
#include <Adafruit_Protomatter.h>

// Pixel Dust Simulator
// #include <Adafruit_PixelDust.h>

// HARDWARE CONFIG ---------------------------------------------------------------------------------

// ---- MatrixPortal M4 pin configuration -----

#define UP_BUTTON 2
#define DOWN_BUTTON 3
#define STATUS_LED 4
#define D13_LED 13

uint8_t rgbPins[]  = {7, 8, 9, 10, 11, 12};
uint8_t addrPins[] = {17, 18, 19, 20}; // add 21 if 64-pixel tall
uint8_t clockPin   = 14;
uint8_t latchPin   = 15;
uint8_t oePin      = 16;

// ---- RGB LED Matrix configuration and setup ----

#define MATRIX_HEIGHT 32 // Matrix height (pixels) - SET TO 64 FOR 64x64 MATRIX!
#define MATRIX_WIDTH  64 // Matrix width (pixels)
#define MAX_FPS       60 // Maximum redraw rate, frames/second
                         // 14 for animation, 24 or 30 for video, 60 is smooth

float centerX = 0.5f * MATRIX_WIDTH;
float centerY = 0.5f * MATRIX_HEIGHT;

// ---- LIS3DH Triple-Axis Accelerometer ----

#define ACCEL_PIN 0x19
Adafruit_LIS3DH accel = Adafruit_LIS3DH();

// ---- SHT4X Temperature and Humidity Sensor ----

Adafruit_SHT4x sht4 = Adafruit_SHT4x();
sensors_event_t humidity, temp; // % rH, °C

// THE SCREEN --------------------------------------------------------------------------------------

/**
 * @brief The LED Matrix.
 */
Adafruit_Protomatter matrix(
  MATRIX_WIDTH,               // Width of matrix (or matrix chain) in pixels
  4,                          // Bit depth, 1-6, only green uses 6, avoid
  1, rgbPins,                 // # of matrix chains, array of 6 RGB pins for each
  sizeof(addrPins), addrPins, // # of address pins (height is inferred), array of pins
  clockPin, latchPin, oePin,  // Other matrix control pins
  false);                     // Double-buffering

/**
 * @brief A representation of the pixel grid, abstracted away from Protomatter.
 */
pixel_t pixels[MATRIX_HEIGHT][MATRIX_WIDTH] = {};

// ERROR HANDLING ----------------------------------------------------------------------------------

void err(int milliseconds, String message = "") {
  Serial.write("ERROR ");
  Serial.write(milliseconds);
  Serial.write("\n");
  if (message.length() > 0) {
    Serial.println(message);
  }

  uint8_t i;
  pinMode(LED_BUILTIN, OUTPUT);       // Using onboard LED
  for(i=1;;i++) {                     // Loop forever...
    digitalWrite(LED_BUILTIN, i & 1); // LED on/off blink to alert user
    delay(milliseconds);
  }
}

// SETUP -------------------------------------------------------------------------------------------

void setup(void) {
  Serial.begin(9600);
  // if serial is important, include this so we don't miss messages
  // if code not commented out, the display will not function until serial port opens
  while (!Serial) delay(10);

  ProtomatterStatus status = matrix.begin();
  Serial.printf("Protomatter begin() status: %d\n", status);
  if (status != 0) {
    err(200, "protomatter failed to start");
  }

  if (!sht4.begin()) {
    err(400, "SHT4x failed to start");
  }
  Serial.print("SHT4x Serial 0x");
  Serial.println(sht4.readSerial(), HEX); // 0xF5D9FCC

  // SHT4X_HIGH_PRECISION
  // SHT4X_LOW_PRECISION
  sht4.setPrecision(SHT4X_MED_PRECISION);

  sht4.getEvent(&humidity, &temp);

  Serial.print("Humidity: ");
  Serial.print(humidity.relative_humidity);
  Serial.println("% rH");

  Serial.print("Temperature: ");
  Serial.print(temp.temperature);
  Serial.println("°C");
}

// LOOP --------------------------------------------------------------------------------------------

uint32_t prevTime = 0; // Used for frames-per-second throttle

void loop(void) {
  // --- Limit FPS ---
  // Limit the animation frame rate to MAX_FPS.  Because the subsequent sand
  // calculations are non-deterministic (don't always take the same amount
  // of time, depending on their current states), this helps ensure that
  // things like gravity appear constant in the simulation.
  uint32_t t;
  while(((t = micros()) - prevTime) < (1000000L / MAX_FPS));
  prevTime = t;

  // --- Update pixel data ---
  drawHeart(10, 4);

  // --- Done ---
  drawPixels();  // Move pixels[] to matrix
  matrix.show(); // Copy data to matrix buffers
}

// DRAW IMAGES -------------------------------------------------------------------------------------

void drawPixels(void) {
  for(int y = 0; y < MATRIX_HEIGHT; y++) {
    for(int x = 0; x < MATRIX_WIDTH; x++) {
      if (pixels[y][x].on == true) {
        uint16_t color = matrix.colorHSV(pixels[y][x].hue);
        matrix.drawPixel(x, y, color);
      }
    }
  }
}

void buildCircularGradientFromMask(
  pixel_mask_t      mask,
  uint8_t           xStart,
  uint8_t           yStart,
  gradient_config_t cfg
) {
  // scale the start/end values to a useful value to compute hue for Protomatter (0-65535)
  float gradient_start_scaled = cfg.gradient_start / 360.0f * 65535.0f;
  float gradient_end_scaled = cfg.gradient_end / 360.0f * 65535.0f;

  // normalize hues
  while (cfg.gradient_start < 0) cfg.gradient_start += 360;
  while (cfg.gradient_start > 360) cfg.gradient_start -= 360;
  while (cfg.gradient_end < 0) cfg.gradient_end += 360;
  while (cfg.gradient_end > 360) cfg.gradient_end -= 360;

  // if the start and end are reversed, gradient_reverse is backwards
  if (cfg.gradient_start > cfg.gradient_end) cfg.gradient_reverse = !cfg.gradient_reverse;

  // loop through the mask
  uint8_t *pixel = mask.mask;
  for(int y = 0; y < mask.height && y < MATRIX_HEIGHT; y++) {

    // get y on pixels[] grid
    uint8_t yDraw = yStart + y;
    if (yDraw > MATRIX_HEIGHT) continue;

    for(int x = 0; x < mask.width && x < MATRIX_WIDTH; x++, pixel++) {
      // if mask is empty here, we're not drawing anything
      if (*pixel == 0) continue;

      // get x on pixels[] grid
      uint8_t xDraw = xStart + x;
      if (xDraw > MATRIX_WIDTH) continue;

      float tick = millis() * 0.0001f * cfg.animation_speed;
      float v = (
          cos(((float)x - centerX) / (0.5f * cfg.shape_width))
          + sin(((float)y - centerY) / (0.5f * cfg.shape_width)) + tick
        ) * cfg.shape_width;

      // interval is a number between 0 and gradient_width
      float interval = fmod(v, cfg.gradient_width);
      uint16_t hue;

      // if the gradient is less than 360deg total, then
      // we must stop at the end and loop back and forth
      if (cfg.gradient_start > 0 || cfg.gradient_end < 360) {

        // find how far across the gradient we are
        float distance_across_gradient;
        if (interval < 0.5f * cfg.gradient_width) {
          // for the first half of the gradient, move twice as fast
          distance_across_gradient = interval * 2;
        }
        else {
          // for the second half, move backwards, still twice as fast
          distance_across_gradient = (cfg.gradient_width - interval) * 2.0f;
        }
        // between 0 and 1
        float percent_across_gradient = distance_across_gradient / cfg.gradient_width;

        // counterclockwise
        if (cfg.gradient_reverse) {
          // how much to scale the percent_across by, rotating counterclockwise
          float scaling_factor = 360 - gradient_end_scaled - gradient_start_scaled;
          // percent_across_gradient * scaling_factor = degrees backwards from start
          hue = round(gradient_start_scaled - percent_across_gradient * scaling_factor);
        }
        // clockwise
        else {
          // how much to scale the percent_across by
          float scaling_factor = gradient_end_scaled - gradient_start_scaled;
          // percent_across_gradient * scaling_factor = degrees across after the start
          hue = round(gradient_start_scaled + percent_across_gradient * scaling_factor);
        }
      }
      else {
        // 0 <= interval / gradient_width <= 1
        hue = round((interval / cfg.gradient_width) * 65535.0f);
      }

      // normalize within bounds
      while (hue < 0) hue += 65535;
      while (hue > 65535) hue -= 65535;

      pixels[yDraw][xDraw].on = true;
      pixels[yDraw][xDraw].hue = hue;
    }
  }
}

void drawHeart(uint8_t xStart, uint8_t yStart) {
  pixel_mask_t heart_mask;
  heart_mask.width = 11;
  heart_mask.height = 11;

  uint8_t mask[] = {
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
  heart_mask.mask = mask;

  gradient_config_t heart_config;
  heart_config.animation_speed  = 10;
  heart_config.gradient_width   = 32;
  heart_config.shape_width      = 16;
  heart_config.gradient_start   = 260;
  heart_config.gradient_end     = 350;

  buildCircularGradientFromMask(heart_mask, xStart, yStart, heart_config);
}

// UTILITY -----------------------------------------------------------------------------------------

float celsius2fahrenheit(float celsius) {
  return ((celsius * 9)/5) + 32;
}
