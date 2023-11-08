#ifndef H_CRYPTID_INTERWEBS
#define H_CRYPTID_INTERWEBS

#include <WiFiNINA.h>
#include <MQTT.h>

typedef enum {
  INTERWEBS_STATUS_INIT = 0,
  INTERWEBS_STATUS_NO_WIFI = 1,
  INTERWEBS_STATUS_WIFI = 2,
	INTERWEBS_STATUS_DISCONNECTED = 10,
  INTERWEBS_STATUS_CONNECTING = 11,
  INTERWEBS_STATUS_CONNECTED = 12,
  INTERWEBS_NO_SOCKET_AVAIL = 255
} interwebs_status_t;

#define INTERWEBS_MQTT_HOST "public.cloud.shiftr.io"
#define INTERWEBS_MQTT_USER "public"
#define INTERWEBS_MQTT_PASS "public"
#define INTERWEBS_MQTT_CLIENT_ID "arduino"

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
     * @brief Initialize MQTT client.
     *
     * @return Success
     */
    bool mqttInit(void);

    /**
     * @brief Handle MQTT messages received.
     */
    static void mqttMessageReceived(String &topic, String &payload);

    /**
     * @brief Main MQTT client loop.
     */
    void mqttLoop(void);

    void mqttSendMessage(void);

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
     * @brief The MQTT client.
     */
    MQTTClient mqttClient;

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
