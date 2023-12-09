# cryptid-updates

LED Matrix display

## Dependencies

Board | Details
---|---
Adafruit SAMD Boards | includes Adafruit Matrix Portal M4
Arduino SAMD Boards | dependency for Adafruit SAMD Boards

Library | Details
---|---
Adafruit Protomatter | RGB LED matrix
Adafruit NeoPixel | Status LED
Adafruit LIS3DH | Accelerometer
Adafruit SHT4x | Temperature & humidity sensor
WiFiNINA | WiFi
MQTT_Looped | MQTT

## Known Bug

If `MQTT_LOG` or `MQTT_DEBUG` is defined, `Serial` must be active or the `MQTT_Looped` class
will hang. This is not an issue on other M4 boards. (???)
