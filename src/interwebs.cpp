#include <WiFiNINA.h>
#include <utility/wifi_drv.h>
#include <utility/server_drv.h>
#include <utility/WiFiSocketBuffer.h>
#include <MQTT.h>

#include "utilities.h"
#include "../wifi-config.h"
#include "interwebs.h"

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

  status = INTERWEBS_STATUS_WIFI_CONNECTING;
	uint8_t wifiStatus = WL_IDLE_STATUS;
  uint8_t completeAttempts = 5;
  do {
    wifiStatus = WiFiDrv::wifiSetPassphrase(wifi_ssid, strlen(wifi_ssid), wifi_pass, strlen(wifi_pass));
    if (wifiStatus != WL_FAILURE) {
      Serial.print("Connecting");
    	uint8_t attempts = 5;
      do {
        Serial.print(".");
        delay(2000);
        wifiStatus = WiFiDrv::getConnectionStatus();
      } while ((wifiStatus == WL_IDLE_STATUS || wifiStatus == WL_NO_SSID_AVAIL || wifiStatus == WL_SCAN_COMPLETED) && --attempts > 0);
    }
  } while (wifiStatus != WL_CONNECTED && --completeAttempts > 0);

  if (wifiStatus != WL_CONNECTED) {
    status = INTERWEBS_STATUS_WIFI_ERRORS;
    return false;
  }

  status = INTERWEBS_STATUS_WIFI_CONNECTED;
  return true;
}

bool Interwebs::mqttInit(void) {
  Serial.print("MQTT connecting...");
  mqttClient.begin(mqttBroker, wifiClient);
  mqttClient.onMessage(mqttMessageReceived);

  status = INTERWEBS_STATUS_MQTT_CONNECTING;

  // Connect
  bool connected = false;
  if (!attempt([&](){
    Serial.print(".");
    return mqttClient.connect(MQTT_CLIENT_ID, MQTT_USER, MQTT_PASS);
  })) {
    Serial.println("Error connecting to MQTT broker.");
    return false;
  }
  Serial.println();

  // Subscribe
  if (!attempt([&](){ return mqttClient.subscribe("test"); }, 5, 250)) {
    Serial.println("Error subscribing to /test");
    status = INTERWEBS_STATUS_MQTT_ERRORS;
  }
  if (!attempt([&](){ return mqttClient.subscribe("hello"); }, 5, 250)) {
    Serial.println("Error subscribing to /hello");
    status = INTERWEBS_STATUS_MQTT_ERRORS;
  }

  status = INTERWEBS_STATUS_MQTT_CONNECTED;

  return true;
}

bool Interwebs::mqttReconnect(void) {
  if (status == INTERWEBS_STATUS_MQTT_CONNECTED) {
    status = INTERWEBS_STATUS_MQTT_OFFLINE;
  }

  // mqttClient.netClient
  WiFiClient* mqttNetClient = (WiFiClient*)(mqttClient.*robbed<MQTTClientNetClient>::ptr);

  // step 1b copied from WiFiClient::stop()
  if (status == INTERWEBS_STATUS_MQTT_CLOSING_SOCKET) {
    if (mqttNetClient->status() != CLOSED) {
      // if not closed yet, keep waiting until the next loop
      return false;
    }
    WiFiSocketBuffer.close(mqttNetClient->*robbed<WiFiClientSock>::ptr);
    // _sock
    mqttNetClient->*robbed<WiFiClientSock>::ptr = NO_SOCKET_AVAIL;
    status = INTERWEBS_STATUS_MQTT_CONNECTING;
  }

  // step 1 copied from WiFiClient::connect()
  if (status == INTERWEBS_STATUS_MQTT_OFFLINE || status == INTERWEBS_STATUS_MQTT_CONNECTING) {
    Serial.println("Reconnecting MQTT...");
    status = INTERWEBS_STATUS_MQTT_CONNECTING;

    if (!(mqttClient.*robbed<MQTTClientNetClient>::ptr)->connected()) {
      Serial.println("Connecting via network client...");

      // step 1a copied from WiFiClient::stop()
      // If the socket isn't closed, close it and wait for the next loop.
      if (mqttNetClient->*robbed<WiFiClientSock>::ptr != NO_SOCKET_AVAIL) {
        ServerDrv::stopClient(mqttNetClient->*robbed<WiFiClientSock>::ptr);
        status = INTERWEBS_STATUS_MQTT_CLOSING_SOCKET;
        return false;
      }

      mqttNetClient->*robbed<WiFiClientSock>::ptr = ServerDrv::getSocket();
      if (mqttNetClient->*robbed<WiFiClientSock>::ptr == NO_SOCKET_AVAIL) {
        // failure, flag to start over
      	Serial.println("No Socket available");
        mqttClient.*robbed<MQTTClientLastError>::ptr = LWMQTT_NETWORK_FAILED_CONNECT;
        status = INTERWEBS_STATUS_MQTT_OFFLINE;
        return false;
      }

    	ServerDrv::startClient(uint32_t(mqttBroker), (uint16_t)1883, mqttNetClient->*robbed<WiFiClientSock>::ptr);
    }
    // step complete, break
    status = INTERWEBS_STATUS_MQTT_CONNECTING_2;
    return false;
  }

  // step 2 copied from MQTTClient::connect()
  if (status = INTERWEBS_STATUS_MQTT_CONNECTING_2) {
    // check step 1 was successful
  	if (!(mqttClient.*robbed<MQTTClientNetClient>::ptr)->connected()) {
      status = INTERWEBS_STATUS_MQTT_OFFLINE;
      mqttClient.*robbed<MQTTClientLastError>::ptr = LWMQTT_NETWORK_FAILED_CONNECT;
      return false;
  	}

    Serial.println("MQTT connecting to broker...");
    // prepare options
    lwmqtt_options_t options = lwmqtt_default_options;
    options.keep_alive = 60;
    options.clean_session = true;
    options.client_id = lwmqtt_string(MQTT_CLIENT_ID);
    options.username = lwmqtt_string(MQTT_USER);
    options.password = lwmqtt_string(MQTT_PASS);

    // connect to broker
    mqttClient.*robbed<MQTTClientLastError>::ptr = lwmqtt_connect(
      &(mqttClient.*robbed<MQTTClientClient>::ptr),
      options,
      mqttClient.*robbed<MQTTClientWill>::ptr,
      &(mqttClient.*robbed<MQTTClientReturnCode>::ptr),
      1000
    );
    if (mqttClient.*robbed<MQTTClientLastError>::ptr != LWMQTT_SUCCESS) {
      Serial.println("MQTT broker connection failed.");
      mqttClient.*robbed<MQTTClientConnected>::ptr = false;
      (mqttClient.*robbed<MQTTClientNetClient>::ptr)->stop();

      return false;
    }
    // complete
    Serial.println("MQTT connected to broker.");
    status = INTERWEBS_STATUS_MQTT_CONNECTED;
    mqttClient.*robbed<MQTTClientConnected>::ptr = true;
  }

  return status == INTERWEBS_STATUS_MQTT_CONNECTED;
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

void Interwebs::mqttSendMessage(String topic, String payload) {
  if (verifyConnection()) {
    Serial.println("MQTT publishing to " + topic);
    if (!mqttClient.publish(topic, payload)) {
      Serial.println("Error publishing");
    }
  }
}

bool Interwebs::verifyConnection() {
  if (!wifiIsConnected()) {
    status = INTERWEBS_STATUS_WIFI_OFFLINE;
    Serial.println("WiFi disconnected...");
    if (!wifiInit()) {
      Serial.println("Error reconnecting WiFi");
      return false;
    }
  }
  if (!mqttIsConnected()) {
    Serial.println("MQTT disconnected...");
    if (!mqttReconnect()) {
      return false;
    }
  }

  return true;
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
