/* --------------------------------------------------------------------------
CRYPTID UPDATES - LED Matrix Display
----------------------------------------------------------------------------- */

// INCLUDES --------------------------------------------------------------------

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

// CONFIG ----------------------------------------------------------------------

// MatrixPortal M4 ------

#define UP_BUTTON 2
#define DOWN_BUTTON 3
#define STATUS_LED 4
#define D13_LED 13

uint8_t rgbPins[]  = {7, 8, 9, 10, 11, 12};
uint8_t addrPins[] = {17, 18, 19, 20}; // add 21 if 64-pixel tall
uint8_t clockPin   = 14;
uint8_t latchPin   = 15;
uint8_t oePin      = 16;

// RGB LED Matrix ------

#define HEIGHT  32 // Matrix height (pixels) - SET TO 64 FOR 64x64 MATRIX!
#define WIDTH   64 // Matrix width (pixels)
#define MAX_FPS 45 // Maximum redraw rate, frames/second
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

// LIS3DH Triple-Axis Accelerometer ------

// #define ACCEL_PIN 0x19
// Adafruit_LIS3DH accel = Adafruit_LIS3DH();

// Colors & Pixels ------

uint16_t pixels[HEIGHT][WIDTH] = {};
float gradient_width = 32.0f;
float shape_width = 16.0f;

// IMAGES ----------------------------------------------------------------------

void generateColors(void) {
  for(int y=0; y<HEIGHT; y++) {
    for(int x=0; x<WIDTH; x++) {
      float tick = 100 + (millis() * 0.001);
      float v = (
          cos(((float)x - centerX) / (0.5f * shape_width))
          + sin(((float)y - centerY) / (0.5f * shape_width)) + tick
        ) * shape_width;

      float interval = fmod(v, gradient_width);
      uint16_t hue = round((interval / gradient_width) * 65535.0f);

      // (*p)[x][y] = 0;
      pixels[y][x] = hue;
    }
  }
}

void drawPixels(void) {
  for(int y=0; y<HEIGHT; y++) {
    for(int x=0; x<WIDTH; x++) {
      uint16_t color = matrix.colorHSV(pixels[y][x]);
      matrix.drawPixel(x, y, color);
    }
  }
}

// SETUP -----------------------------------------------------------------------

void err(int x) {
  uint8_t i;
  pinMode(LED_BUILTIN, OUTPUT);       // Using onboard LED
  for(i=1;;i++) {                     // Loop forever...
    digitalWrite(LED_BUILTIN, i & 1); // LED on/off blink to alert user
    delay(x);
  }
}

void setup(void) {
  Serial.begin(9600);
  while (!Serial) delay(10);

  // uint16_t pixels[HEIGHT][WIDTH] = {};
  // uint16_t (*p_pixels)[HEIGHT][WIDTH]= &pixels;
  // generateColors(p_pixels);

  ProtomatterStatus status = matrix.begin();
  Serial.printf("Protomatter begin() status: %d\n", status);
}

// LOOP ------------------------------------------------------------------------

void loop() {
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
