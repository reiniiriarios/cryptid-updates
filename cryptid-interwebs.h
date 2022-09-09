#ifndef CRYPTID_INTERWEBS_H
#define CRYPTID_INTERWEBS_H

#include <SPI.h>
#include <WiFiNINA.h>

#include "cryptid-wifi-config.h"

/**
 * @brief Connect to the interwebs and discover all the interesting webs.
 */
class Interwebs {
  public:
    /**
     * @brief Construct a new Interwebs object.
     */
    Interwebs();

    /**
     * @brief Print WiFi status over the Serial connection.
     */
    void Interwebs::printWifiStatus(void);

    /**
     * @brief If connection is sending data, print it over the Serial connection. Run every loop.
     */
    void Interwebs::read(void);

    /**
     * @brief Check connection, close client if not connected. Run at end of every loop.
     */
    void Interwebs::checkStatus(void);

    /**
     * @brief Fetch data.
     */
    bool Interwebs::fetchData(void);

  private:
    /**
     * @brief Connection status.
     */
    int status = WL_IDLE_STATUS;

    /**
     * @brief The WiFi client.
     */
    WiFiClient client;

    /**
     * @brief The IP address to connect to.
     */
    IPAddress *server;
};

#endif
