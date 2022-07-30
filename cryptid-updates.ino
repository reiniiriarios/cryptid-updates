//
// CRYPTID UPDATES - LED Matrix Display
//

// INCLUDES ----------------------------------------------------------------------------------------

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
// #include <Adafruit_LIS3DH.h>

// Temperature & Humidity Sensor
#include <Adafruit_SHT4x.h>

// RGB Matrix
#include <Adafruit_Protomatter.h>

// Pixel Dust Simulator
// #include <Adafruit_PixelDust.h>

// CONFIG ------------------------------------------------------------------------------------------

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

#define HEIGHT  32 // Matrix height (pixels) - SET TO 64 FOR 64x64 MATRIX!
#define WIDTH   64 // Matrix width (pixels)
#define MAX_FPS 60 // Maximum redraw rate, frames/second
                   // 14 for animation, 24 or 30 for video, 60 is smooth

float centerX = 0.5f * WIDTH;
float centerY = 0.5f * HEIGHT;

Adafruit_Protomatter matrix(
  WIDTH,       // Width of matrix (or matrix chain) in pixels
  4,           // Bit depth, 1-6, only green uses 6, avoid
  1, rgbPins,  // # of matrix chains, array of 6 RGB pins for each
  sizeof(addrPins), addrPins, // # of address pins (height is inferred), array of pins
  clockPin, latchPin, oePin,  // Other matrix control pins
  false);       // Double-buffering

uint32_t prevTime = 0; // Used for frames-per-second throttle

// ---- LIS3DH Triple-Axis Accelerometer ----

// #define ACCEL_PIN 0x19
// Adafruit_LIS3DH accel = Adafruit_LIS3DH();

// ---- Colors & Pixels settings ----

uint16_t pixels[HEIGHT][WIDTH] = {};
uint8_t  animation_speed  = 10;    // how fast the gradient animates
                                   // 1 = very slow, 10 = steady, 40 = uncomfortably fast
                                   // set this high at your own risk (seizures, etc)
uint8_t  gradient_width   = 32;    // size of gradient within shapes, no direct correlation to pixels
uint8_t  shape_width      = 16;    // size of the shapes, no direct correlation to pixels
uint16_t gradient_start   = 260;   // in degrees, 0-360
uint16_t gradient_end     = 350;   // in degrees, 0-360
boolean  gradient_reverse = false; // clockwise or counter

// GENERATE IMAGES ---------------------------------------------------------------------------------

// scale the above values to a useful value to computer hue for Protomatter (0-65535)
float gradient_start_scaled = gradient_start / 360.0f * 65535.0f;
float gradient_end_scaled = gradient_end / 360.0f * 65535.0f;

/**
 * @brief Generate pretty colors to the pixels[] array
 * 
 * @todo This should be refactored to accept a mask
 * 
 */
void generateColors(void) {
  for(int y=0; y<HEIGHT; y++) {
    for(int x=0; x<WIDTH; x++) {
      float tick = millis() * 0.0001f * animation_speed;
      float v = (
          cos(((float)x - centerX) / (0.5f * shape_width))
          + sin(((float)y - centerY) / (0.5f * shape_width)) + tick
        ) * shape_width;

      // interval is a number between 0 and gradient_width
      float interval = fmod(v, gradient_width);
      uint16_t hue;

      // if the gradient is less than 360deg total, then
      // we must stop at the end and loop back and forth
      if (gradient_start > 0 || gradient_end < 360) {

        // find how far across the gradient we are
        float distance_across_gradient;
        if (interval < 0.5f * gradient_width) {
          // for the first half of the gradient, move twice as fast
          distance_across_gradient = interval * 2;
        }
        else {
          // for the second half, move backwards, still twice as fast
          distance_across_gradient = (gradient_width - interval) * 2.0f;
        }
        // between 0 and 1
        float percent_across_gradient = distance_across_gradient / gradient_width;

        // counterclockwise
        if (gradient_reverse) {
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
        hue = round((interval / gradient_width) * 65535.0f);
      }

      // normalize within bounds
      while (hue < 0) hue += 65535;
      while (hue > 65535) hue -= 65535;

      // (*p)[x][y] = 0;
      pixels[y][x] = hue;
    }
  }
}

/**
 * @brief Write the pixels[] data to the Protomatter matrix
 * 
 */
void drawPixels(void) {
  for(int y=0; y<HEIGHT; y++) {
    for(int x=0; x<WIDTH; x++) {
      uint16_t color = matrix.colorHSV(pixels[y][x]);
      matrix.drawPixel(x, y, color);
    }
  }
}

// ERROR HANDLING ----------------------------------------------------------------------------------

/**
 * @brief Blink onboard LED every <milliseconds> and print message over serial
 * 
 * @param milliseconds 
 * @param message 
 */
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

/**
 * @brief Run once on start
 * 
 */
void setup(void) {
  Serial.begin(9600);
  // if serial is important, include this so we don't miss messages
  // if code not commented out, the display will not function until serial port opens
  while (!Serial) delay(10);

  // normalize hues
  while (gradient_start < 0) gradient_start += 360;
  while (gradient_start > 360) gradient_start -= 360;
  while (gradient_end < 0) gradient_end += 360;
  while (gradient_end > 360) gradient_end -= 360;

  // if the start and end are reversed, gradient_reverse is backwards
  if (gradient_start > gradient_end) gradient_reverse = !gradient_reverse;

  // uint16_t pixels[HEIGHT][WIDTH] = {};
  // uint16_t (*p_pixels)[HEIGHT][WIDTH]= &pixels;
  // generateColors(p_pixels);

  ProtomatterStatus status = matrix.begin();
  Serial.printf("Protomatter begin() status: %d\n", status);
  if (status != 0) {
    err(200, "protomatter failed to start");
  }
}

// LOOP --------------------------------------------------------------------------------------------

/**
 * @brief Main loop
 * 
 */
void loop(void) {
  // Limit the animation frame rate to MAX_FPS.  Because the subsequent sand
  // calculations are non-deterministic (don't always take the same amount
  // of time, depending on their current states), this helps ensure that
  // things like gravity appear constant in the simulation.
  uint32_t t;
  while(((t = micros()) - prevTime) < (1000000L / MAX_FPS));
  prevTime = t;

  // Update pixel data
  generateColors();
  drawPixels();

  matrix.show(); // Copy data to matrix buffers
}
