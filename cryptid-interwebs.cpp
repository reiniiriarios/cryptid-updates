#include <WiFiNINA.h>
#include <MQTT.h>

#include "cryptid-utilities.h"
#include "cryptid-wifi-config.h"
#include "cryptid-interwebs.h"

Interwebs::Interwebs() {
  IPAddress mqtt_server(172,16,0,131);
  mqttBroker = mqtt_server;
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
  delay(2000); // wait a breath, this seems to resolve something
  for (uint8_t i = 0; i < 5; i++) {
    Serial.print("Attempting to connect to SSID: ");
    Serial.println(wifi_ssid);
    // connect to WPA/WPA2 network; change this line if using open or WEP network
    int wifiStatus = WiFi.begin(wifi_ssid, wifi_pass);
    delay(1000);
    if (wifiStatus != WL_CONNECTED) {
      Serial.println("Connection failed");

      continue;
    }

    Serial.print("Waiting for connection");
    for (uint8_t i = 0; i < 10; i++) {
      delay(500);
      Serial.print(".");
    }
    Serial.println();

    printWifiStatus();

    if (!mqttInit()) {
      return false;
    }

    return true;
  }

  return false;
}

bool Interwebs::mqttInit(void) {
  Serial.print("MQTT connecting...");
  mqttClient.begin(mqttBroker, wifiClient);
  mqttClient.onMessage(mqttMessageReceived);

  // Connect
  bool connected = false;
  if (!attempt([&](){
    Serial.print(".");
    return mqttClient.connect("cryptidUpdates", "cryptid", "public");
  })) {
    Serial.println("Error connecting to MQTT broker.");
    return false;
  }
  Serial.println();

  // Subscribe
  if (!attempt([&](){ return mqttClient.subscribe("test"); }, 5, 250)) {
    Serial.println("Error subscribing to /test");
  }
  if (!attempt([&](){ return mqttClient.subscribe("hello"); }, 5, 250)) {
    Serial.println("Error subscribing to /hello");
  }

  return true;
}

void Interwebs::mqttMessageReceived(String &topic, String &payload) {
  if (topic == "test") {
    Serial.println("MQTT test: " + payload);
  }
  else if (topic == "hello") {
    Serial.println("MQTT says, 'Hello " + payload + "!'");
  }
  else {
    Serial.println("Unrecognized MQTT topic: " + topic);
  }
}

void Interwebs::mqttSendMessage(void) {
  Serial.println("MQTT publishing");
  if (!mqttClient.publish("hello", "world")) {
    Serial.println("Error publishing");
  }
}

void Interwebs::mqttLoop(void) {
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
