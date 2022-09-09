#include <SPI.h>
#include <WiFiNINA.h>

#include "cryptid-wifi-config.h"
#include "cryptid-interwebs.h"

Interwebs::Interwebs() {
  server = new IPAddress(74,125,232,128);

  if (WiFi.status() == WL_NO_MODULE) {
    Serial.println("communication with WiFi module failed");
  }

  String fv = WiFi.firmwareVersion();
  if (fv < WIFI_FIRMWARE_LATEST_VERSION) {
    Serial.println("firmware upgrade available");
  }

  // attempt to connect to Wifi network
  for (uint8_t i = 0; i < 5 && status != WL_CONNECTED; i++) {
    Serial.print("attempting to connect to SSID: ");
    Serial.println(wifi_ssid);
    // connect to WPA/WPA2 network; change this line if using open or WEP network
    status = WiFi.begin(wifi_ssid, wifi_pass);
    // wait 5 seconds for connection:
    delay(5000);
    Serial.println("connected to WiFi");
    printWifiStatus();

    Serial.println("starting connection to server");
    // if we get a connection, report back via serial
    if (client.connect(*server, 80)) {
      Serial.println("connected to server");
      // Make a HTTP request
      client.println("GET /search?q=arduino HTTP/1.1");
      client.println("Host: www.google.com");
      client.println("Connection: close");
      client.println();
    }
  }
}

void Interwebs::printWifiStatus(void) {
  if (!client.connected()) {
    Serial.println("not connected to WiFi");
    return;
  }
  // print the SSID of the network you're attached to
  Serial.print("SSID: ");
  Serial.println(WiFi.SSID());
  // print the board's IP address
  IPAddress ip = WiFi.localIP();
  Serial.print("IP Address: ");
  Serial.println(ip);
  // print the received signal strength
  long rssi = WiFi.RSSI();
  Serial.print("signal strength (RSSI):");
  Serial.print(rssi);
  Serial.println(" dBm");
}

void Interwebs::read(void) {
  // if there are incoming bytes available from the server, read them and print them
  while (client.available()) {
    char c = client.read();
    Serial.write(c);
  }
}

void Interwebs::checkStatus(void) {
  // if the server's disconnected, stop the client
  if (!client.connected()) {
    Serial.println();
    Serial.println("disconnecting from server");
    client.stop();
  }
}
