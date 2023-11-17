#ifndef H_CRYPTID_INTERWEBS
#define H_CRYPTID_INTERWEBS

#include <WiFiNINA.h>
#include <MQTT.h>

#include "error_display.h"
#include "weather.h"
#include "time.h"
#include "types.h"
#include "error_display.h"
#include "weather.h"
#include "utilities.h"

#define MQTT_CLIENT_ID "cryptidUpdates"
#define MQTT_USER "cryptid"
#define MQTT_PASS "public"

//-------- Rob MQTTClient Private Methods/Properties --------

template<typename Tag>
struct robbed {
  /* export it ... */
  typedef typename Tag::type type;
  static type ptr;
};

template<typename Tag>
typename robbed<Tag>::type robbed<Tag>::ptr;

template<typename Tag, typename Tag::type p>
struct rob : robbed<Tag> {
  /* fill it ... */
  struct filler {
    filler() { robbed<Tag>::ptr = p; }
  };
  static filler filler_obj;
};

template<typename Tag, typename Tag::type p>
typename rob<Tag, p>::filler rob<Tag, p>::filler_obj;

struct MQTTClientConnected { typedef bool MQTTClient::*type; };
template class rob<MQTTClientConnected, &MQTTClient::_connected>;

struct MQTTClientLastError { typedef lwmqtt_err_t MQTTClient::*type; };
template class rob<MQTTClientLastError, &MQTTClient::_lastError>;

struct MQTTClientNetClient { typedef Client* MQTTClient::*type; };
template class rob<MQTTClientNetClient, &MQTTClient::netClient>;

struct MQTTClientClient { typedef lwmqtt_client_t MQTTClient::*type; };
template class rob<MQTTClientClient, &MQTTClient::client>;

struct MQTTClientWill { typedef lwmqtt_will_t* MQTTClient::*type; };
template class rob<MQTTClientWill, &MQTTClient::will>;

struct MQTTClientReturnCode { typedef lwmqtt_return_code_t MQTTClient::*type; };
template class rob<MQTTClientReturnCode, &MQTTClient::_returnCode>;

struct WiFiClientSock { typedef uint8_t WiFiClient::*type; };
template class rob<WiFiClientSock, &WiFiClient::_sock>;

//-------- End MQTTClient Robbery --------

typedef enum {
  INTERWEBS_STATUS_INIT = 0,
  INTERWEBS_STATUS_WIFI_CONNECTING = 1,
  INTERWEBS_STATUS_WIFI_CONNECTED = 2,
  INTERWEBS_STATUS_WIFI_OFFLINE = 3,
  INTERWEBS_STATUS_WIFI_ERRORS = 4,
  INTERWEBS_STATUS_MQTT_CONNECTING = 10,
  INTERWEBS_STATUS_MQTT_CLOSING_SOCKET = 15,
  INTERWEBS_STATUS_MQTT_CONNECTING_2 = 14,
  INTERWEBS_STATUS_MQTT_CONNECTION_SUCCESS = 17,
  INTERWEBS_STATUS_MQTT_CONNECTED = 11,
  INTERWEBS_STATUS_MQTT_OFFLINE = 12,
  INTERWEBS_STATUS_MQTT_SUBSCRIPTION_FAIL = 16,
  INTERWEBS_STATUS_MQTT_ERRORS = 13,
} interwebs_status_t;

/**
 * @brief Connect to the interwebs and discover all the interesting webs.
 */
class Interwebs {
  public:
    /**
     * @brief Current status of interwebs connections.
     */
    int status = INTERWEBS_STATUS_INIT;

    /**
     * @brief The current weather.
     */
    weather_t* weather = nullptr;

    /**
     * @brief A pointer to the time display object.
     */
    TimeDisplay* time = nullptr;

    /**
     * @brief Construct a new Interwebs object.
     * 
     * @param gfx_p A pointer to the graphics object.
     * @param err_p A pointer to the error display object.
     * @param time_p A pointer to the time display object.
     */
    Interwebs(Gfx *gfx_p, ErrorDisplay *err_p, TimeDisplay *time_p);

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
    bool wifiInit(void);

    /**
     * @brief Reconnect to WiFi. This method operates step-by-step, continuing each call.
     * 
     * @return Connected
     */
    bool wifiReconnect(void);

    /**
     * @brief Initialize MQTT client.
     *
     * @return Success
     */
    bool mqttInit(void);

    /**
     * @brief Reconnect to MQTT broker. This method operates step-by-step, continuing each call.
     * 
     * @return Connected
     */
    bool mqttReconnect(void);

    /**
     * @brief Verify connection to WiFi and MQTT.
     * 
     * @return Connected
     */
    bool verifyConnection(void);

    /**
     * @brief Connect MQTT subscriptions.
     *
     * @return All subscribed.
     */
    bool mqttSubscribe(void);

    /**
     * @brief Handle MQTT messages received.
     */
    void mqttMessageReceived(String &topic, String &payload);

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

    /**
     * @brief Send all discoveries.
     */
    bool mqttSendDiscovery(void);

    /**
     * @brief Send temperature discovery.
     */
    bool mqttSendDiscoveryTemp(void);

    /**
     * @brief Send humidity discovery.
     */
    bool mqttSendDiscoveryHumid(void);

  private:
    /**
     * @brief A pointer to the graphics object.
     */
    Gfx *gfx;

    /**
     * @brief A pointer to the error display object.
     */
    ErrorDisplay *err;

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
    MQTTClient *mqttClient;

    /**
     * @brief The IP address to connect to.
     */
    IPAddress mqttBroker;
};

#endif
