#ifndef H_CRYPTID_INTERWEBS
#define H_CRYPTID_INTERWEBS

#include <WiFiNINA.h>

typedef enum {
  INTERWEBS_STATUS_INIT = 0,
  INTERWEBS_STATUS_NO_WIFI = 1,
  INTERWEBS_STATUS_WIFI = 2,
	INTERWEBS_STATUS_DISCONNECTED = 10,
  INTERWEBS_STATUS_CONNECTING = 11,
  INTERWEBS_STATUS_CONNECTED = 12,
  INTERWEBS_NO_SOCKET_AVAIL = 255
} interwebs_status_t;

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
     * @brief Connect to WiFi. Run in setup().
     *
     * @return Whether the connection was successful.
     */
    bool connect(void);

    /**
     * @brief Print WiFi status over the Serial connection.
     */
    void printWifiStatus(void);

    /**
     * @brief If connection is sending data, print it over the Serial connection. Run in loop().
     *
     * @return Whether bytes were read from server.
     */
    bool read(void);

    /**
     * @brief Check connection.
     *
     * @return Whether client is connected.
     */
    bool checkConnection(void);

    /**
     * @brief Start connection.
     *
     * @return Whether client connection was able to initialize. Also fails if already connecting.
     *
     * @see WiFiClient::connect()
     */
    bool startConnection(void);

    /**
     * @brief End connection.
     *
     * @see WiFiClient::stop()
     */
    void endConnection(void);

    /**
     * @brief Fetch data.
     */
    void fetchData(void);


  private:
    /**
     * @brief Connection status.
     */
    uint8_t connectionStatus = INTERWEBS_STATUS_INIT;

    /**
     * @brief Millis counter to wait for connection.
     */
    uint16_t waitForConnectionStart = 0;

    /**
     * @brief The WiFi client.
     */
    WiFiClient client;

    /**
     * @brief The IP address to connect to.
     */
    IPAddress server;

    /**
     * @brief Arduino WiFi socket.
     *
     * @see ServerDrv::getSocket()
     */
    uint8_t _sock = 255;
};

#endif
