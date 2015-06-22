#pragma once
#include <string>
#include <vector>
#include <set>
#include "jsoncpp/json/json.h"

#include "mqtt_wrapper.h"

using namespace std;



class IMQTTRPCImpl {
public:
    virtual ~IMQTTRPCImpl() = 0;
    virtual Json::Value RunMethod(const string& service, const string& method, const Json::Value& input) = 0;
};



typedef std::shared_ptr<IMQTTRPCImpl> PMQTTRPCImpl;

class TMQTTRPCServer : public IMQTTObserver,
                       public std::enable_shared_from_this<TMQTTRPCServer>
{
public:
    TMQTTRPCServer(PMQTTClientBase mqtt_client, PMQTTRPCImpl rpc_impl, const string& driver_id);

    void Init();
    void OnMessage(const struct mosquitto_message *message);

    void OnConnect(int rc);
    void OnSubscribe(int mid, int qos_count, const int *granted_qos);

    void RegisterMethod(const string& service, const string& method);






private:
    PMQTTClientBase MQTTClient;
    string DriverId;
    PMQTTRPCImpl RPCImpl;
    set<pair<const string,const string>> Methods;
};


