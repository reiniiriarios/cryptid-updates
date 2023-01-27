#include <WiFiNINA.h>

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
  IPAddress litwavulcu_com(46,226,109,159);
  server = litwavulcu_com;
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
    return true;
  }

  connectionStatus = INTERWEBS_STATUS_NO_WIFI;
  return false;
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

bool Interwebs::read(void) {
  // if there are incoming bytes available from the server, read them and print them
  bool read = false;
  while (client.available()) {
    char c = client.read();
    Serial.write(c);
    read = true;
  }

  return read;
}

void Interwebs::fetchData(void) {
  if (!client.connected()) {
    Serial.println("Not connected, unable to fetch data");
    return;
  }

  // Make an HTTP request
  client.println("GET / HTTP/1.1");
  client.println("Host: litwavulcu.com");
  client.println("User-Agent: ArduinoWiFi/1.1");
  client.println("Connection: close");
  client.println();
}

bool Interwebs::startConnection() {
  if (connectionStatus == INTERWEBS_STATUS_NO_WIFI) {
    return false;
  }
  if (connectionStatus == INTERWEBS_STATUS_CONNECTING) {
    return false;
  }

  if (_sock != INTERWEBS_NO_SOCKET_AVAIL) {
    endConnection();
  }

  _sock = ServerDrv::getSocket();
  if (_sock == INTERWEBS_NO_SOCKET_AVAIL) {
  	Serial.println("No Socket available");
  	return false;
  }

  Serial.print("Starting connection to ");
  Serial.println(server);

  // Cannot use SSL. The Matrix M4 board uses a custom version
  // of WiFiNINA that does not work with the firmware updater,
  // which is required to install ssl certificates.
	ServerDrv::startClient(uint32_t(server), 80, _sock);
  waitForConnectionStart = millis();

  connectionStatus = INTERWEBS_STATUS_CONNECTING;

  return true;
}

void Interwebs::endConnection() {
  Serial.println("Resetting connection");
  ServerDrv::stopClient(_sock);

  // TODO: move this loop out of here.
  int count = 0;
  // wait maximum 5 secs for the connection to close
  while (client.status() != CLOSED && ++count < 50)
    delay(100);

  WiFiSocketBuffer.close(_sock);
  _sock = INTERWEBS_NO_SOCKET_AVAIL;
}

bool Interwebs::checkConnection() {
  if (connectionStatus == INTERWEBS_STATUS_CONNECTING) {
    if (client.connected()) {
      connectionStatus = INTERWEBS_STATUS_CONNECTED;
      Serial.println("Connected");

      return true;
    }

    // If we're past waiting, give up.
    if (millis() - waitForConnectionStart > 10000) {
      connectionStatus = INTERWEBS_STATUS_DISCONNECTED;
      Serial.println("Unable to connect");
    }

    return false;
  }
  else if (connectionStatus == INTERWEBS_STATUS_CONNECTED) {
    if (!client.connected()) {
      Serial.println("Connection failed");
      connectionStatus = INTERWEBS_STATUS_DISCONNECTED;

      return false;
    }

    return true;
  }

  return false;
}
