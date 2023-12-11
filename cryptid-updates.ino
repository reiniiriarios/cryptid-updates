//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//~ CRYPTID UPDATES ~ LED Matrix Display ~
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
#include "cryptid-updates.h"

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

// SGB30 TVOC/eCO2 Gas Sensor
Adafruit_SGP30 sgp = Adafruit_SGP30();

sensors_event_t accel, humidity, temp;  // % rH, °C

// THE SCREEN & GRAPHICS OBJECTS -------------------------------------------------------------------

bool DISPLAY_ON = true;

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
Heart heart(&gfx);
Aaahhh aaahhh(&gfx);

// OTHER CONTROL OBJECTS ---------------------------------------------------------------------------

MQTT_Looped interwebs(new WiFiClient(), WIFI_SSID, WIFI_PASS,
  new IPAddress(MQTT_SERVER), 1883, MQTT_USER, MQTT_PASS, MQTT_CLIENT_ID);

weather_t weatherInterior;
weather_t weatherExterior;

// ERROR HANDLING ----------------------------------------------------------------------------------

void err(int milliseconds, String message = "") {
  Serial.println("ERROR " + String(milliseconds));
  if (message.length() > 0) {
    Serial.println(message);
  }
  pinMode(LED_BUILTIN, OUTPUT);        // Using onboard LED
  for (uint8_t i = 1;; i++) {          // Loop forever...
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
  Serial.printf("Protomatter status: %d\n", status);
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
  // set initial values
  aaahhh.setMovement(accel.acceleration.x, accel.acceleration.y, accel.acceleration.z);

  // Temperature & Humidity
  if (!sht4.begin()) {
    err(400, "SHT4x failed to start");
  }
  Serial.print("SHT4x Serial 0x");
  Serial.println(sht4.readSerial(), HEX);  // 0xF5D9FCC
  sht4.setPrecision(SHT4X_MED_PRECISION);  // SHT4X_HIGH_PRECISION SHT4X_LOW_PRECISION
  sht4.getEvent(&humidity, &temp);

  // if (!sgp.begin()) {
  //   err(600, "SGB30 failed to start");
  // }
  // Serial.print("SGP30 Serial 0x");
  // Serial.print(sgp.serialnumber[0], HEX);
  // Serial.print(sgp.serialnumber[1], HEX);
  // Serial.println(sgp.serialnumber[2], HEX);
  //sgp.setIAQBaseline(0x8E68, 0x8F41);  // Will vary for each sensor!

  // Interwebs
  interwebsSetup();

  int countdownMS = Watchdog.enable(1000);
  Serial.print("Enabling watchdog at ");
  Serial.print(countdownMS, DEC);
  Serial.println(" ms countdown.");
}

// MQTT --------------------------------------------------------------------------------------------

void mqttCurrentStatus(void) {
  String on = "ON";
  if (!DISPLAY_ON) on = "OFF";
  interwebs.mqttSendMessage("cryptid/display/status", on);
  interwebs.mqttSendMessage("cryptid/display/temperature", temp.temperature);
  interwebs.mqttSendMessage("cryptid/display/humidity", humidity.relative_humidity);
}

void interwebsSetup(void) {
  interwebs.setBirth("cryptid/display/status", "online");
  interwebs.setWill("cryptid/display/status", "offline");

  interwebs.addDiscovery("homeassistant/switch/display/cryptidDisplay/config",
    "{\"name\":\"display\","
    "\"stat_t\":\"cryptid/display/state\","
    "\"cmd_t\":\"cryptid/display/set\","
    "\"uniq_id\":\"cryptidDisplaySet\","
    "\"dev\":{\"ids\":[\"cryptidDisplay\"],\"name\":\"Cryptid Display\"}}"
  );
  interwebs.addDiscovery("homeassistant/sensor/temperature/cryptidDisplay/config",
    "{\"name\":\"temperature\","
    "\"dev_cla\":\"temperature\","
    "\"stat_t\":\"cryptid/display/temperature\","
    "\"unit_of_measurement\":\"°C\","
    "\"uniq_id\":\"cryptidDisplayTempC\","
    "\"dev\":{\"ids\":[\"cryptidDisplay\"],\"name\":\"Cryptid Display\"}}"
  );
  interwebs.addDiscovery("homeassistant/sensor/humidity/cryptidDisplay/config",
    "{\"name\":\"humidity\","
    "\"dev_cla\":\"humidity\","
    "\"stat_t\":\"cryptid/display/humidity\","
    "\"unit_of_measurement\":\"%\","
    "\"uniq_id\":\"cryptidDisplayHumidity\","
    "\"dev\":{\"ids\":[\"cryptidDisplay\"],\"name\":\"Cryptid Display\"}}"
  );

  // Turn display on and off.
  interwebs.onMqtt("cryptid/display/set", [](char* payload, uint8_t /*len*/){
    String pStr = String(payload);
    if (pStr == "on" || pStr == "ON" || pStr == "1") {
      DISPLAY_ON = true;
      interwebs.mqttSendMessage("cryptid/display/status", "ON");
    }
    else if (pStr == "off" || pStr == "OFF" || pStr == "0") {
      DISPLAY_ON = false;
      interwebs.mqttSendMessage("cryptid/display/status", "OFF");
    }
  });

  // Update current time.
  interwebs.onMqtt("current_time", [](char* payload, uint8_t /*len*/){
    timeDisplay.setTime(payload);
  });

  // Update weather.
  interwebs.onMqtt("weather/temperature", [](char* payload, uint8_t /*len*/){
    weatherExterior.temp_c = String(payload).toFloat();
    weatherExterior.temp_f = celsius2fahrenheit(weatherExterior.temp_c);
    weatherExterior.temp_last = millis();
  });
  interwebs.onMqtt("weather/feelslike", [](char* payload, uint8_t /*len*/){
    weatherExterior.feelslike_c = String(payload).toFloat();
    weatherExterior.feelslike_f = celsius2fahrenheit(weatherExterior.feelslike_c);
    weatherExterior.feelslike_last = millis();
    return;
  });
  interwebs.onMqtt("weather/humidity", [](char* payload, uint8_t /*len*/){
    weatherExterior.humidity = min(max(0, strtol(payload, nullptr, 10)), 100);
    weatherExterior.humidity_last = millis();
    return;
  });
  interwebs.onMqtt("weather/code", [](char* payload, uint8_t /*len*/){
    weatherExterior.code = (weather_code_t) min(max(0, strtol(payload, nullptr, 10)), 2000);
    weatherExterior.code_last = millis();
    return;
  });
  interwebs.onMqtt("weather/isday", [](char* payload, uint8_t /*len*/){
    // 1 = day, 0 = night
    weatherExterior.is_day = min(max(0, strtol(payload, nullptr, 10)), 1);
    weatherExterior.is_day_last = millis();
    return;
  });
  interwebs.onMqtt("weather/aqi", [](char* payload, uint8_t /*len*/){
    weatherExterior.aqi = min(max(0, strtol(payload, nullptr, 10)), 500);
    weatherExterior.aqi_last = millis();
    return;
  });

  // Send discovery when Home Assistant notifies it's online.
  interwebs.onMqtt("homeassistant/status", [](char* payload, uint8_t /*len*/){
    if (payload == "online") {
      interwebs.sendDiscoveries();
      mqttCurrentStatus();
    }
  });
}

// LOOP --------------------------------------------------------------------------------------------

uint32_t prevTime = 0;      // Used for frames-per-second throttle

current_display_t currentDisplay = CURRENT_DISPLAY_NONE;

void loop(void) {
  Watchdog.reset();

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
  interwebs.loop();

  // Draw things.
  if (DISPLAY_ON) {
    updateDisplay();
  }

  // Status LEDs.
  // This chip seems to hiccup when first connecting, so let's be specific with our colors...
  switch (interwebs.getStatus()) {
    case MQTT_LOOPED_STATUS_INIT:
      gfx.drawStatusPixel(120, 125); break;
    // WiFi
    case MQTT_LOOPED_STATUS_WIFI_READY:
      gfx.drawStatusPixel(90, 120); break;
    case MQTT_LOOPED_STATUS_WIFI_OFFLINE:
      gfx.drawStatusPixel(10, 40); break;
    case MQTT_LOOPED_STATUS_WIFI_ERRORS:
      gfx.drawStatusPixel(0, 30); break;
    case MQTT_LOOPED_STATUS_WIFI_CLOSING_SOCKET:
      gfx.drawStatusPixel(240, 270); break;
    // < WiFi offline
    case MQTT_LOOPED_STATUS_WIFI_CONNECTED:
      gfx.drawStatusPixel(130, 150); break;
    // MQTT
    case MQTT_LOOPED_STATUS_MQTT_CONNECTING:
    case MQTT_LOOPED_STATUS_MQTT_CONNECTION_WAIT:
    case MQTT_LOOPED_STATUS_MQTT_CONNECTION_SUCCESS:
      gfx.drawStatusPixel(270, 330); break;
    case MQTT_LOOPED_STATUS_MQTT_MISSING_CONACK:
    case MQTT_LOOPED_STATUS_MQTT_CONNECTION_CONFIRMED:
    case MQTT_LOOPED_STATUS_MQTT_CLOSING_SOCKET:
    case MQTT_LOOPED_STATUS_MQTT_DISCONNECTED:
    case MQTT_LOOPED_STATUS_MQTT_OFFLINE:
    case MQTT_LOOPED_STATUS_MQTT_ERRORS:
      gfx.drawStatusPixel(330, 359); break;
    // < MQTT offline
    case MQTT_LOOPED_STATUS_OKAY:
    case MQTT_LOOPED_STATUS_READING_SUB_PACKET:
    case MQTT_LOOPED_STATUS_ACTIVE:
      break; // none
    // > MQTT is active
    case MQTT_LOOPED_STATUS_MQTT_SUBSCRIBED:
    case MQTT_LOOPED_STATUS_MQTT_SUBSCRIBING:
    case MQTT_LOOPED_STATUS_MQTT_SUBSCRIPTION_FAIL:
    case MQTT_LOOPED_STATUS_MQTT_ANNOUNCED:
    case MQTT_LOOPED_STATUS_READING_CONACK_PACKET:
    case MQTT_LOOPED_STATUS_READING_SUBACK_PACKET:
    case MQTT_LOOPED_STATUS_READING_PUBACK_PACKET:
    case MQTT_LOOPED_STATUS_READING_PING_PACKET:
    case MQTT_LOOPED_STATUS_SENDING_DISCOVERY:
    case MQTT_LOOPED_STATUS_SUBSCRIPTION_PACKET_READ:
    case MQTT_LOOPED_STATUS_SUBSCRIPTION_IN_QUEUE:
    case MQTT_LOOPED_STATUS_VERIFY_CONNECTION:
    case MQTT_LOOPED_STATUS_CONNECTION_VERIFIED:
    case MQTT_LOOPED_STATUS_MQTT_PUBLISHING:
    case MQTT_LOOPED_STATUS_MQTT_PUBLISHED:
      break; // none
    default: // wtf
      gfx.drawStatusPixel(0, 360); break;
  }
  // if (!interwebs.wifiIsConnected()) {
  //   gfx.drawStatusPixel(260, 300);
  // } else if (!interwebs.mqttIsConnected()) {
  //   gfx.drawStatusPixel(10, 45);
  // }

  // Done.
  gfx.toBuffer();  // Move pixels[] to matrix
  matrix.show();   // Copy data to matrix buffers
}

// DISPLAY -----------------------------------------------------------------------------------------

void updateDisplay(void) {
  // AAAHHH
  if (aaahhh.isMovingFast()) {
    aaahhh.display();
    return;
  }

  // INTERIOR (default)
  if (currentDisplay == CURRENT_DISPLAY_INT_TEMP_HUMID) {
    updateDisplayDefault();
    return;
  }

  // CURRENT WEATHER
  if (currentDisplay == CURRENT_DISPLAY_EXT_TEMP_HUMID) {
    // No data.
    if (weatherExterior.code == WEATHER_CODE_UNKNOWN) {
      gfx.drawStatusPixel(200, 240, 2);
      updateDisplayDefault();
      return;
    }
    // Expired data.
    if (
      millis() - weatherExterior.temp_last > 600000 ||
      millis() - weatherExterior.humidity_last > 600000 ||
      millis() - weatherExterior.code_last > 600000
    ) { // 1 min = 60000 ms
      // Weather more than 10 minutes out of date.
      gfx.drawStatusPixel(40, 80, 2);
      updateDisplayDefault();
      return;
    }
    // Okay.
    bool is_day = true;
    if (millis() - weatherExterior.is_day_last < 600000) {
      is_day = weatherExterior.is_day;
    }
    tempDisplay.update(weatherExterior.temp_f);
    humidityDisplay.update(weatherExterior.humidity);
    weatherSymbol.drawSymbol(weatherExterior.code, is_day);
    // air quality index separate expiry
    if (millis() - weatherExterior.aqi_last > 1800000) { // 30min
      gfx.drawStatusPixel(120, 180, 3);
    } else {
      aqi.update(weatherExterior.aqi);
    }
    return;
  }

  // CURRENT TIME
  if (currentDisplay == CURRENT_DISPLAY_DATE_TIME) {
    // No data.
    if (timeDisplay.getDay() == 0) {
      gfx.drawStatusPixel(170, 220, 3);
      updateDisplayDefault();
      return;
    }
    // Data expired.
    if (millis() - timeDisplay.last_updated > 1200000) { // 20min
      gfx.drawStatusPixel(10, 50, 3);
      updateDisplayDefault();
      return;
    }
    // Okay.
    timeDisplay.updateScreen();
    return;
  }

  // Oops.
  gfx.drawStatusPixel(5, 20, 7);
  gfx.drawStatusPixel(15, 30, 8);
  gfx.drawStatusPixel(25, 40, 9);
}

void updateDisplayDefault(void) {
  tempDisplay.update(weatherInterior.temp_f);
  humidityDisplay.update(weatherInterior.humidity);
  weatherSymbol.drawSymbolInterior();
}

// EVERY N SECONDS ---------------------------------------------------------------------------------

uint16_t loopCounter = 0;  // Counts up every frame based on MAX_FPS.

void everyN(void) {
  every_n_seconds(1, 0) {
    // Read temperature and humidity
    sht4.getEvent(&humidity, &temp);
    weatherInterior.temp_c = temp.temperature;
    weatherInterior.temp_f = celsius2fahrenheit(temp.temperature);
    weatherInterior.humidity = humidity.relative_humidity;
    // Set temperature and humidity for air quality sensor.
    // sgp.setHumidity(getAbsoluteHumidity(temp.temperature, humidity.relative_humidity));
  }
  every_n_seconds(1, 1) {
    // uint16_t TVOC_base, eCO2_base;
    // if (!sgp.getIAQBaseline(&eCO2_base, &TVOC_base)) {
    //   Serial.println("SGP: Failed to get baseline readings");
    // } else {
    //   Serial.print("SGP: Baseline values: eCO2: 0x"); Serial.print(eCO2_base, HEX);
    //   Serial.print(" & TVOC: 0x"); Serial.println(TVOC_base, HEX);
    // }
  }
  every_n_seconds(1, 2) {
    // if (!sgp.IAQmeasure()) {
    //   Serial.println("SGP: Measurement failed");
    // } else {
    //   Serial.print("TVOC "); Serial.print(sgp.TVOC); Serial.print(" ppb\t");
    //   Serial.print("eCO2 "); Serial.print(sgp.eCO2); Serial.println(" ppm");
    // }
  }
  every_n_seconds(1, 3) {
    // if (!sgp.IAQmeasureRaw()) {
    //   Serial.println("SGP: Raw Measurement failed");
    // } else {
    //   Serial.print("Raw H2 "); Serial.print(sgp.rawH2); Serial.print(" \t");
    //   Serial.print("Raw Ethanol "); Serial.print(sgp.rawEthanol); Serial.println("");
    // }
  }
  every_n_seconds(5, 4) {
    // Loop current display.
    if (currentDisplay == CURRENT_DISPLAY_INT_TEMP_HUMID) {
      currentDisplay = CURRENT_DISPLAY_EXT_TEMP_HUMID;
    } else if (currentDisplay == CURRENT_DISPLAY_EXT_TEMP_HUMID) {
      currentDisplay = CURRENT_DISPLAY_DATE_TIME;
    } else {
      currentDisplay = CURRENT_DISPLAY_INT_TEMP_HUMID;
    }
  }
  every_n_seconds(20, 5) {
    mqttCurrentStatus();
  }
  every_n_seconds(60, 5) {
    Serial.print("Free Memory: ");
    Serial.println(freeMemory());
  }
  loopCounter++;
}
