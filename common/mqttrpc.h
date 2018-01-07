#pragma once
#include <string>
#include <vector>
#include <map>
#include <functional>
#include "jsoncpp/json/json.h"

#include "mqtt_wrapper.h"


// RPC server error codes
typedef enum {
    E_RPC_PARSE_ERROR = -32700,
    E_RPC_SERVER_ERROR = -32000,
    E_RPC_REQUEST_TIMEOUT = -32100
} TMQTTRPCErrorCode;


class TMQTTRPCServer : public IMQTTObserver,
                       public std::enable_shared_from_this<TMQTTRPCServer>
{
public:
    TMQTTRPCServer(PMQTTClientBase mqtt_client, const std::string& driver_id);

    void Init();
    void OnMessage(const struct mosquitto_message *message);

    void OnConnect(int rc);
    void OnSubscribe(int mid, int qos_count, const int *granted_qos);

    typedef std::function<Json::Value(const Json::Value& )> TMethodHandler;
    void RegisterMethod(const std::string& service, const std::string& method, TMethodHandler handler);


private:
    PMQTTClientBase MQTTClient;
    std::string DriverId;
    std::map<std::pair<const std::string, const std::string>, TMethodHandler> MethodHandlers;
};


