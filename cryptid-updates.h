#ifndef H_CRYPTID_UPDATES
#define H_CRYPTID_UPDATES

// Watchdog Timer
#include <Adafruit_SleepyDog.h>

// Status LED
#include <Adafruit_NeoPixel.h>

// Accelerometer
#include <Adafruit_LIS3DH.h>

// Temperature & Humidity Sensor
#include <Adafruit_SHT4x.h>

// SGB30 TVOC/eCO2 Gas Sensor
#include <Adafruit_SGP30.h>

// RGB Matrix
#include <Adafruit_Protomatter.h>

// Interwebs
#include <WiFiNINA.h>
#include <MQTT_Looped.h>

// Filesystem
// #include <Adafruit_FlashCache.h>
// #include <Adafruit_FlashTransport.h>
// #include <Adafruit_SPIFlash.h>
// #include <Adafruit_SPIFlashBase.h>
// #include <flash_devices.h>

// Read bitmaps from SD
// #include <Adafruit_ImageReader_EPD.h>
// #include <Adafruit_ImageReader.h>

#include "wifi-config.h"
#include "src/types.h"
#include "src/config.h"
#include "src/utilities.h"
#include "src/error_display.h"
#include "src/gfx.h"
#include "src/loading.h"
#include "src/heart.h"
#include "src/temperature.h"
#include "src/humidity.h"
#include "src/weather.h"
#include "src/aqi.h"
#include "src/time.h"
#include "src/aaahhh.h"

// Instead of using a timer, these approximations ensure the
// same delay between actions. This means things don't execute
// as precicely on schedule, but the animation is, in theory,
// smoother.
#define every_n_seconds(n, offset) if (loopCounter % (MAX_FPS * n) == offset)
#define every_n_loops(n, offset) if (loopCounter % n == offset)

/**
 * @brief Current display.
 */
typedef enum {
  CURRENT_DISPLAY_NONE = 0,
  CURRENT_DISPLAY_TEST = 1,
  CURRENT_DISPLAY_RAINBOW = 2,
  CURRENT_DISPLAY_INT_TEMP_HUMID = 10,
  CURRENT_DISPLAY_EXT_TEMP_HUMID = 20,
  CURRENT_DISPLAY_DATE_TIME = 30,
} current_display_t;

/**
 * @brief Blink onboard LED and print message over serial.
 * 
 * @param milliseconds How fast to blink the LED.
 * @param message      Error message.
 */
void err(int milliseconds, String message);

/**
 * @brief Run once on start.
 */
void setup(void);

/**
 * @brief Set up interwebs things.
 */
void interwebsSetup(void);

/**
 * @brief Main loop.
 */
void loop(void);

/**
 * @brief Draw things.
 */
void updateDisplay(void);

/**
 * @brief Do things every n seconds.
 */
void everyN(void);

#endif
