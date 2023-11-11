//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//~ CRYPTID UPDATES ~ LED Matrix Display ~
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

// INCLUDES ----------------------------------------------------------------------------------------

// Status LED
#include <Adafruit_NeoPixel.h>

// Accelerometer
#include <Adafruit_LIS3DH.h>

// Temperature & Humidity Sensor
#include <Adafruit_SHT4x.h>

// RGB Matrix
#include <Adafruit_Protomatter.h>

// Filesystem
// #include <Adafruit_FlashCache.h>
// #include <Adafruit_FlashTransport.h>
// #include <Adafruit_SPIFlash.h>
// #include <Adafruit_SPIFlashBase.h>
// #include <flash_devices.h>

// Read bitmaps from SD
// #include <Adafruit_ImageReader_EPD.h>
// #include <Adafruit_ImageReader.h>

// Cryptid Updates
#include "cryptid-updates.h"
#include "src/types.h"
#include "src/config.h"
#include "src/utilities.h"
#include "src/interwebs.h"
#include "src/gfx.h"
#include "src/loading.h"
#include "src/heart.h"
#include "src/temperature.h"
#include "src/humidity.h"

// HARDWARE CONFIG ---------------------------------------------------------------------------------

// MatrixPortal M4 pin configuration
uint8_t rgbPins[] = { 7, 8, 9, 10, 11, 12 };
uint8_t addrPins[] = { 17, 18, 19, 20 };  // add 21 if 64-pixel tall
uint8_t clockPin = 14;
uint8_t latchPin = 15;
uint8_t oePin = 16;

// LIS3DH Triple-Axis Accelerometer
Adafruit_LIS3DH accel = Adafruit_LIS3DH();

// SHT4X Temperature and Humidity Sensor
Adafruit_SHT4x sht4 = Adafruit_SHT4x();
sensors_event_t humidity, temp;  // % rH, °C

// THE SCREEN & GRAPHICS OBJECTS -------------------------------------------------------------------

// The LED Matrix.
Adafruit_Protomatter matrix(
  MATRIX_WIDTH,                // Width of matrix (or matrix chain) in pixels
  4,                           // Bit depth, 1-6, only green uses 6, avoid
  1, rgbPins,                  // # of matrix chains, array of 6 RGB pins for each
  sizeof(addrPins), addrPins,  // # of address pins (height is inferred), array of pins
  clockPin, latchPin, oePin,   // Other matrix control pins
  true);                       // Double-buffering

// The graphics object responsible for all drawing operations.
Gfx gfx(&matrix);

// A heart <3.
Heart heart(&gfx);

// The temperature display.
TemperatureDisplay tempDisplay(&gfx);

// The humidity display.
HumidityDisplay humidityDisplay(&gfx);

// OTHER CONTROL OBJECTS ---------------------------------------------------------------------------

Interwebs interwebs;

// ERROR HANDLING ----------------------------------------------------------------------------------

void err(int milliseconds, String message = "") {
  Serial.write("ERROR ");
  Serial.write(milliseconds);
  Serial.write("\n");
  if (message.length() > 0) {
    Serial.println(message);
  }

  uint8_t i;
  pinMode(LED_BUILTIN, OUTPUT);        // Using onboard LED
  for (i = 1;; i++) {                  // Loop forever...
    digitalWrite(LED_BUILTIN, i & 1);  // LED on/off blink to alert user
    delay(milliseconds);
  }
}

// SETUP -------------------------------------------------------------------------------------------

void setup(void) {
  Serial.begin(9600);
  // if serial is important, include this so we don't miss messages
  // if code not commented out, the display will not function until serial port opens
  while (!Serial) delay(10);

  // The display
  ProtomatterStatus status = matrix.begin();
  Serial.printf("Protomatter begin() status: %d\n", status);
  if (status != 0) {
    err(200, "protomatter failed to start");
  }

  // Loading screen
  displayLoadingScreen(&gfx);
  gfx.toBuffer();
  matrix.show();

  // Temperature & Humidity
  if (!sht4.begin()) {
    err(400, "SHT4x failed to start");
  }
  Serial.print("SHT4x Serial 0x");
  Serial.println(sht4.readSerial(), HEX);  // 0xF5D9FCC

  sht4.setPrecision(SHT4X_MED_PRECISION);  // SHT4X_HIGH_PRECISION SHT4X_LOW_PRECISION

  sht4.getEvent(&humidity, &temp);

  Serial.print("Humidity: ");
  Serial.print(humidity.relative_humidity);
  Serial.println("% rH");

  Serial.print("Temperature: ");
  Serial.print(temp.temperature);
  Serial.println("°C");

  // Interwebs
  interwebs.connect();
}

// LOOP --------------------------------------------------------------------------------------------

uint32_t prevTime = 0;      // Used for frames-per-second throttle
uint16_t frameCounter = 0;  // Counts up every frame based on MAX_FPS.

float temp_f = 0;  // Temperature in degrees fahrenheit.

void loop(void) {
  // Limit FPS
  uint32_t t;
  while (((t = micros()) - prevTime) < (1000000L / MAX_FPS));
  prevTime = t;

  // Do something every second.
  if (frameCounter % MAX_FPS == 0) {
    sht4.getEvent(&humidity, &temp);
    temp_f = celsius2fahrenheit(temp.temperature);
  }
  // Do something every n seconds.
  if (frameCounter % (MAX_FPS * 5) == 0) {
    Serial.print("Temperature: ");
    Serial.print(temp_f);
    Serial.print("°F, Humidity: ");
    Serial.print(humidity.relative_humidity);
    Serial.println("% rH");
  }
  // Do something every n seconds.
  if (frameCounter % (MAX_FPS * 10) == 0) {
    Serial.print("Free Memory: ");
    Serial.println(freeMemory());
  }
  // Do something every n seconds.
  if (frameCounter % (MAX_FPS * 20) == 0) {
    interwebs.mqttSendMessage("display/temperature", String(temp.temperature));
    interwebs.mqttSendMessage("display/humidity", String(humidity.relative_humidity));
    // reset at final repeat block:
    frameCounter = 0;
  }
  frameCounter++;

  // Run main MQTT loop every loop.
  interwebs.mqttLoop();

  // Update pixel data
  heart.update();
  tempDisplay.update(temp_f);
  humidityDisplay.update(humidity.relative_humidity);

  if (!interwebs.wifiIsConnected()) {
    gfx.drawErrorWiFi();
  }
  else if (!interwebs.mqttIsConnected()) {
    gfx.drawErrorMqtt();
  }

  // Done
  gfx.toBuffer();  // Move pixels[] to matrix
  matrix.show();   // Copy data to matrix buffers
}
