#include <WiFiNINA.h>
#include <utility/wifi_drv.h>
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

  if (!wifiInit()) {
    Serial.println("Connection failed");
    return false;
  }

  Serial.print("Waiting for connection");
  for (uint8_t i = 0; i < 5; i++) {
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

bool Interwebs::wifiInit() {
  Serial.print("Attempting to connect to SSID: ");
  Serial.println(wifi_ssid);

	uint8_t status = WL_IDLE_STATUS;
  uint8_t completeAttempts = 5;
  do {
    status = WiFiDrv::wifiSetPassphrase(wifi_ssid, strlen(wifi_ssid), wifi_pass, strlen(wifi_pass));
    if (status != WL_FAILURE) {
      Serial.print("Connecting");
    	uint8_t attempts = 5;
      do {
        Serial.print(".");
        delay(2000);
        status = WiFiDrv::getConnectionStatus();
      } while ((status == WL_IDLE_STATUS || status == WL_NO_SSID_AVAIL || status == WL_SCAN_COMPLETED) && --attempts > 0);
    }
  } while (status != WL_CONNECTED && --completeAttempts > 0);

  return status == WL_CONNECTED;
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
  if (!wifiClient.connected()) {
    Serial.println("WiFi disconnected, reconnecting...");
    if (!wifiInit()) {
      Serial.println("Error connecting");
    }
  }
  if (!mqttClient.connected()) {
    Serial.println("MQTT disconnected, reconnecting...");
    if (!mqttInit()) {
      Serial.println("Error connecting");
    }
  }
  Serial.println("MQTT publishing");
  if (!mqttClient.publish("hello", "world")) {
    Serial.println("Error publishing");
  }
}

void Interwebs::mqttLoop(void) {
  mqttClient.loop();
}

bool Interwebs::mqttIsConnected(void) {
  return mqttClient.connected();
}

bool Interwebs::wifiIsConnected(void) {
  return WiFiDrv::getConnectionStatus() == WL_CONNECTED;
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
