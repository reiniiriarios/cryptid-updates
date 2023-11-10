#ifndef H_CRYPTID_INTERWEBS
#define H_CRYPTID_INTERWEBS

#include <WiFiNINA.h>
#include <MQTT.h>

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
     * @brief Connect to the given WiFi network, using password.
     * 
     * @return bool 
     */
    bool wifiInit();

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

    /**
     * @brief Send MQTT message.
     */
    void mqttSendMessage(String topic, String payload);

    /**
     * @brief MQTT connection.
     */
    bool mqttIsConnected(void);

    /**
     * @brief WiFi connection.
     */
    bool wifiIsConnected(void);

  private:
    /**
     * @brief Millis counter to wait for connection.
     */
    uint16_t waitForConnectionStart = 0;

    /**
     * @brief The WiFi client.
     */
    WiFiClient wifiClient;

    /**
     * @brief The MQTT client.
     */
    MQTTClient mqttClient;

    /**
     * @brief The IP address to connect to.
     */
    IPAddress mqttBroker;
};

#endif
