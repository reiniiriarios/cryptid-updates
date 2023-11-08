#include <WiFiNINA.h>
#include <MQTT.h>

extern "C" {
  #include <utility/wl_definitions.h>
  #include <utility/wl_types.h>
  #include <string.h>
  #include <utility/debug.h>
}

#include <utility/server_drv.h>
#include <utility/wifi_drv.h>
#include <utility/WiFiSocketBuffer.h>

#include "cryptid-wifi-config.h"
#include "cryptid-interwebs.h"

Interwebs::Interwebs() {
  IPAddress mqtt_server(46,226,109,159);
  server = mqtt_server;
}

bool Interwebs::connect(void) {
  if (WiFi.status() == WL_NO_MODULE) {
    Serial.println("Communication with WiFi module failed");

    return false;
  }

  String fv = WiFi.firmwareVersion();
  if (fv < WIFI_FIRMWARE_LATEST_VERSION) {
    Serial.println("WiFi firmware upgrade available");
  }

  // attempt to connect to Wifi network
  for (uint8_t i = 0; i < 5; i++) {
    Serial.print("Attempting to connect to SSID: ");
    Serial.println(wifi_ssid);
    // connect to WPA/WPA2 network; change this line if using open or WEP network
    int wifiStatus = WiFi.begin(wifi_ssid, wifi_pass);
    if (wifiStatus != WL_CONNECTED) {
      Serial.println("Connection failed");

      continue;
    }

    Serial.print("Waiting for connection");
    for (uint8_t i = 0; i < 10; i++) {
      delay(1000);
      Serial.print(".");
    }
    Serial.println();

    printWifiStatus();
    connectionStatus = INTERWEBS_STATUS_WIFI;

    if (!mqttInit()) {
      return false;
    }

    return true;
  }

  connectionStatus = INTERWEBS_STATUS_NO_WIFI;
  return false;
}

bool Interwebs::mqttInit(void) {
  Serial.print("MQTT connecting...");
  // @todo Replace const with `server`
  mqttClient.begin(INTERWEBS_MQTT_HOST, client);
  mqttClient.onMessage(mqttMessageReceived);

  // Connect
  bool connected = false;
  for (uint8_t i = 0; !connected && i < 10; i++) {
    connected = mqttClient.connect(INTERWEBS_MQTT_CLIENT_ID, INTERWEBS_MQTT_USER, INTERWEBS_MQTT_PASS);
    if (connected) {
      break;
    }
    delay(1000);
    Serial.print(".");
  }
  if (!connected) {
    Serial.println("Error connecting to MQTT broker.");
    return false;
  }
  Serial.println();

  // Subscribe
  mqttClient.subscribe("/test");
  mqttClient.subscribe("/hello");

  return true;
}

void Interwebs::mqttMessageReceived(String &topic, String &payload) {
  if (topic == "test") {
    Serial.print("MQTT test: " + payload);
  }
  else if (topic == "hello") {
    Serial.print("MQTT says, 'Hello world!'");
  }
  else {
    Serial.print("Unrecognized MQTT topic: " + topic);
  }
}

void Interwebs::mqttSendMessage(void) {
  Serial.println("MQTT publishing");
  mqttClient.publish("/hello", "world");
}

void Interwebs::mqttLoop(void) {
  Serial.println("MQTT Looping");
  mqttClient.loop();
}

void Interwebs::printWifiStatus(void) {
  // print the SSID of the network you're attached to
  Serial.print("SSID: ");
  Serial.println(WiFi.SSID());
  // print the board's IP address
  IPAddress ip = WiFi.localIP();
  Serial.print("IP Address: ");
  Serial.println(ip);
  // print the received signal strength
  long rssi = WiFi.RSSI();
  Serial.print("Signal strength (RSSI): ");
  Serial.print(rssi);
  Serial.println(" dBm");
}
