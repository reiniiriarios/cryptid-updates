#include <WiFiNINA.h>

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
    status = WiFi.begin(wifi_ssid, wifi_pass);

    if (status != WL_CONNECTED) {
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

    return true;
  }

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

bool Interwebs::checkStatus(void) {
  // return value of 0 means the client is closed
  if (!client.status()) {
    return false;
  }

  // if the server's disconnected, stop the client
  if (!client.connected()) {
    Serial.println();
    Serial.println("Disconnecting from server");
    client.stop();

    return false;
  }

  return true;
}

bool Interwebs::fetchData(void) {
  Serial.print("Starting connection to ");
  Serial.println(server);
  if (!client.connect(server, 443)) {
    Serial.println("Connection failed");

    return false;
  }

  Serial.println("Connected");
  // Make an HTTP request
  client.println("GET / HTTP/1.1");
  client.println("Host: litwavulcu.com");
  client.println("User-Agent: ArduinoWiFi/1.1");
  client.println("Connection: close");
  client.println();

  return true;
}
