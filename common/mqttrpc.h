#pragma once
#include <string>
#include <vector>
#include <map>
#include <functional>
#include "jsoncpp/json/json.h"

#include "mqtt_wrapper.h"

using namespace std;



class TMQTTRPCServer : public IMQTTObserver,
                       public std::enable_shared_from_this<TMQTTRPCServer>
{
public:
    TMQTTRPCServer(PMQTTClientBase mqtt_client, const string& driver_id);

    void Init();
    void OnMessage(const struct mosquitto_message *message);

    void OnConnect(int rc);
    void OnSubscribe(int mid, int qos_count, const int *granted_qos);

    typedef std::function<Json::Value(const Json::Value& )> TMethodHandler;
    void RegisterMethod(const string& service, const string& method, TMethodHandler handler);


private:
    PMQTTClientBase MQTTClient;
    string DriverId;
    map<pair<const string,const string>, TMethodHandler> MethodHandlers;
};


