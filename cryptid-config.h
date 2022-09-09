#ifndef H_CRYPTID_CONFIG
#define H_CRYPTID_CONFIG

  // ---- MatrixPortal M4 pin configuration -----

  #define UP_BUTTON 2
  #define DOWN_BUTTON 3
  #define STATUS_LED 4
  #define D13_LED 13

  // ---- RGB LED Matrix configuration and setup ----

  #define MATRIX_HEIGHT 32 // Matrix height (pixels) - SET TO 64 FOR 64x64 MATRIX!
  #define MATRIX_WIDTH  64 // Matrix width (pixels)
  #define MAX_FPS       60 // Maximum redraw rate, frames/second
                           // 14 for animation, 24 or 30 for video, 60 is smooth

  #define CENTER_X 0.5f * MATRIX_WIDTH   // The center x-coord on the LED matrix.
  #define CENTER_Y 0.5f * MATRIX_HEIGHT  // The center y-coord on the LED matrix.

  // ---- LIS3DH Triple-Axis Accelerometer ----

  #define ACCEL_PIN 0x19

#endif
