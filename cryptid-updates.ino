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
#include "src/error_display.h"
#include "src/interwebs.h"
#include "src/gfx.h"
#include "src/loading.h"
#include "src/heart.h"
#include "src/temperature.h"
#include "src/humidity.h"
#include "src/weather.h"
#include "src/aqi.h"
#include "src/time.h"
#include "src/aaahhh.h"

bool DISPLAY_ON = true;

// HARDWARE CONFIG ---------------------------------------------------------------------------------

#define BACK_BUTTON 2
#define NEXT_BUTTON 3

// MatrixPortal M4 pin configuration
uint8_t rgbPins[] = { 7, 8, 9, 10, 11, 12 };
uint8_t addrPins[] = { 17, 18, 19, 20 };  // add 21 if 64-pixel tall
uint8_t clockPin = 14;
uint8_t latchPin = 15;
uint8_t oePin = 16;

// LIS3DH Triple-Axis Accelerometer
Adafruit_LIS3DH accelerometer = Adafruit_LIS3DH();

// SHT4X Temperature and Humidity Sensor
Adafruit_SHT4x sht4 = Adafruit_SHT4x();

sensors_event_t accel, humidity, temp;  // % rH, °C

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

// Graphics display objects.
TemperatureDisplay tempDisplay(&gfx);
HumidityDisplay humidityDisplay(&gfx);
WeatherSymbol weatherSymbol(&gfx);
AQI aqi(&gfx);
TimeDisplay timeDisplay(&gfx);
ErrorDisplay errorDisplay(&gfx);
Heart heart(&gfx);
Aaahhh aaahhh(&gfx);

// OTHER CONTROL OBJECTS ---------------------------------------------------------------------------

Interwebs interwebs(&gfx, &errorDisplay);

weather_t weatherInterior;
weather_t weatherExterior;

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
  // Wait for serial port to open.
  // while (!Serial) delay(10);

  // Button setup.
  pinMode(BACK_BUTTON, INPUT_PULLUP);
  pinMode(NEXT_BUTTON, INPUT_PULLUP);

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

  // Accelerometer
  if (!accelerometer.begin(0x19)) {
    err(250, "couldn't find accelerometer");
  }
  accelerometer.setRange(LIS3DH_RANGE_4_G); // 2, 4, 8 or 16 G
  accelerometer.getEvent(&accel);
  aaahhh.setMovement(accel.acceleration.x, accel.acceleration.y, accel.acceleration.z);

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
  mqttSubscriptions();
  if (interwebs.connect()) {
    mqttCurrentStatus();
  }
}

void mqttCurrentStatus(void) {
  String on = "ON";
  if (!DISPLAY_ON) on = "OFF";
  interwebs.mqttSendMessage("cryptid/display/status", on);
  interwebs.mqttSendMessage("cryptid/display/temperature", String(temp.temperature));
  interwebs.mqttSendMessage("cryptid/display/humidity", String(humidity.relative_humidity));
}

void mqttSubscriptions(void) {
  // Turn display on and off.
  interwebs.onMqtt("cryptid/display/set", [](String &payload){
    if (payload == "on" || payload == "ON" || payload.toInt() == 1) {
      DISPLAY_ON = true;
      interwebs.mqttSendMessage("cryptid/display/status", "ON");
    }
    else if (payload == "off" || payload == "OFF" || payload.toInt() == 0) {
      DISPLAY_ON = false;
      interwebs.mqttSendMessage("cryptid/display/status", "OFF");
    }
  });

  // Update current time.
  interwebs.onMqtt("current_time", [](String &payload){
    timeDisplay.setTime(payload);
  });

  // Update weather.
  interwebs.onMqtt("weather/temperature", [](String &payload){
    weatherExterior.temp_c = payload.toFloat();
    weatherExterior.temp_f = celsius2fahrenheit(weatherExterior.temp_c);
    weatherExterior.temp_last = millis();
  });
  interwebs.onMqtt("weather/feelslike", [](String &payload){
    weatherExterior.feelslike_c = payload.toFloat();
    weatherExterior.feelslike_f = celsius2fahrenheit(weatherExterior.feelslike_c);
    weatherExterior.feelslike_last = millis();
    return;
  });
  interwebs.onMqtt("weather/humidity", [](String &payload){
    weatherExterior.humidity = payload.toInt();
    weatherExterior.humidity_last = millis();
    return;
  });
  interwebs.onMqtt("weather/code", [](String &payload){
    weatherExterior.code = static_cast<weather_code_t>(payload.toInt());
    weatherExterior.code_last = millis();
    return;
  });
  interwebs.onMqtt("weather/isday", [](String &payload){
    // 1 = day, 0 = night
    weatherExterior.is_day = payload.toInt() == 1;
    weatherExterior.is_day_last = millis();
    return;
  });
  interwebs.onMqtt("weather/aqi", [](String &payload){
    weatherExterior.aqi = payload.toInt();
    weatherExterior.aqi_last = millis();
    return;
  });

  // Send discovery when Home Assistant notifies it's online.
  interwebs.onMqtt("homeassistant/status", [](String &payload){
    if (payload == "online") {
      interwebs.mqttSendDiscovery();
      mqttCurrentStatus();
    }
  });
}

// LOOP --------------------------------------------------------------------------------------------

uint32_t prevTime = 0;      // Used for frames-per-second throttle

current_display_t currentDisplay = CURRENT_DISPLAY_NONE;

void loop(void) {
  // Limit FPS
  uint32_t t;
  while (((t = micros()) - prevTime) < (1000000L / MAX_FPS));
  prevTime = t;

  // Buttons.
  if (digitalRead(BACK_BUTTON) == LOW) {
    Serial.println("BACK");
    DISPLAY_ON = !DISPLAY_ON;
    while(digitalRead(BACK_BUTTON) == LOW); // wait for release
  }
  if (digitalRead(NEXT_BUTTON) == LOW) {
    Serial.println("NEXT");
    DISPLAY_ON = !DISPLAY_ON;
    while(digitalRead(NEXT_BUTTON) == LOW); // wait for release
  }

  // Read and log accelerometer data.
  accelerometer.getEvent(&accel);
  aaahhh.setMovement(accel.acceleration.x, accel.acceleration.y, accel.acceleration.z);

  // Do things every n seconds.
  everyN();

  // Run main MQTT loop every loop.
  interwebs.mqttLoop();

  // Draw things.
  if (DISPLAY_ON) {
    updateDisplay();
  }

  // Check and repair interwebs connections.
  if (!interwebs.wifiIsConnected()) {
    gfx.drawErrorWiFi();
    interwebs.wifiReconnect();
  }
  else if (!interwebs.mqttIsConnected()) {
    gfx.drawErrorMqtt();
    if (interwebs.mqttReconnect()) {
      interwebs.mqttSendDiscovery();
      mqttCurrentStatus();
    }
  }

  // Done.
  gfx.toBuffer();  // Move pixels[] to matrix
  matrix.show();   // Copy data to matrix buffers
}

void updateDisplay(void) {
  // AAAHHH
  if (aaahhh.isMovingFast()) {
    aaahhh.display();
    return;
  }

  // INTERIOR
  if (currentDisplay == CURRENT_DISPLAY_INT_TEMP_HUMID) {
    tempDisplay.update(weatherInterior.temp_f);
    humidityDisplay.update(weatherInterior.humidity);
    weatherSymbol.drawSymbolInterior();
    return;
  }

  // CURRENT WEATHER
  if (currentDisplay == CURRENT_DISPLAY_EXT_TEMP_HUMID) {
    if (weatherExterior.code == WEATHER_CODE_UNKNOWN) {
      errorDisplay.update(201);
      return;
    }
    if (
      millis() - weatherExterior.temp_last > 600000 ||
      millis() - weatherExterior.humidity_last > 600000 ||
      millis() - weatherExterior.code_last > 600000
    ) { // 1 min = 60000 ms
      // Weather more than 10 minutes out of date.
      errorDisplay.update(202);
      return;
    }
    bool is_day = true;
    if (millis() - weatherExterior.is_day_last < 600000) {
      is_day = weatherExterior.is_day;
    }
    tempDisplay.update(weatherExterior.temp_f);
    humidityDisplay.update(weatherExterior.humidity);
    weatherSymbol.drawSymbol(weatherExterior.code, is_day);
    // air quality index separate expiry
    if (millis() - weatherExterior.aqi_last < 1800000) { // 30min
      aqi.update(weatherExterior.aqi);
    }
    return;
  }

  // CURRENT TIME
  if (currentDisplay == CURRENT_DISPLAY_DATE_TIME) {
    if (timeDisplay.getDay() == 0) {
      errorDisplay.update(401);
      return;
    }
    if (millis() - timeDisplay.last_updated > 1200000) { // 20min
      errorDisplay.update(402);
      return;
    }
    timeDisplay.updateScreen();
    return;
  }

  // Oops.
  errorDisplay.update(101);
}

uint16_t frameCounter = 0;  // Counts up every frame based on MAX_FPS.

void everyN(void) {
  // Once per second.
  if (frameCounter % MAX_FPS == 0) {
    sht4.getEvent(&humidity, &temp);
    weatherInterior.temp_c = temp.temperature;
    weatherInterior.temp_f = celsius2fahrenheit(temp.temperature);
    weatherInterior.humidity = humidity.relative_humidity;
  }

  // Every 5 seconds.
  if (frameCounter % (MAX_FPS * 5) == 0) {
    Serial.print("Temperature: ");
    Serial.print(weatherInterior.temp_f);
    Serial.print("°F, Humidity: ");
    Serial.print(weatherInterior.humidity);
    Serial.println("% rH");

    // Loop current display.
    if (currentDisplay == CURRENT_DISPLAY_INT_TEMP_HUMID) {
      currentDisplay = CURRENT_DISPLAY_EXT_TEMP_HUMID;
    } else if (currentDisplay == CURRENT_DISPLAY_EXT_TEMP_HUMID) {
      currentDisplay = CURRENT_DISPLAY_DATE_TIME;
    } else {
      currentDisplay = CURRENT_DISPLAY_INT_TEMP_HUMID;
    }
  }

  // Every 10 seconds.
  if (frameCounter % (MAX_FPS * 10) == 0) {
    Serial.print("Free Memory: ");
    Serial.println(freeMemory());
  }

  // Every 20 seconds.
  if (frameCounter % (MAX_FPS * 20) == 0) {
    mqttCurrentStatus();

    // reset at final "every" block:
    frameCounter = 0;
  }
  frameCounter++;
}
