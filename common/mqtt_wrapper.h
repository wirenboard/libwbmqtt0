#pragma once
#include <list>
#include <string>
#include <vector>
#include <memory>
#include <mosquittopp.h>
#include <iostream>



class IMQTTObserver {
public:
    virtual ~IMQTTObserver();
    virtual void OnConnect(int rc)=0;
    virtual void OnMessage(const struct mosquitto_message *message)=0;
    virtual void OnSubscribe(int mid, int qos_count, const int *granted_qos)=0 ;
};

typedef std::shared_ptr<IMQTTObserver> PMQTTObserver;

class TMQTTClientBase
{
public:
    virtual ~TMQTTClientBase();
    virtual void Connect() = 0;
    virtual int Publish(int *mid, const std::string& topic, const std::string& payload="", int qos=0, bool retain=false) = 0;
    virtual int Subscribe(int *mid, const std::string& sub, int qos = 0) = 0;
    virtual std::string Id() const = 0;
    void Observe(PMQTTObserver observer) { Observers.push_back(observer); }
    void Unobserve(PMQTTObserver observer) { Observers.remove(observer); }

protected:
    const std::list<PMQTTObserver>& GetObservers() const { return Observers; }

private:
    std::list<PMQTTObserver> Observers;
};

typedef std::shared_ptr<TMQTTClientBase> PMQTTClientBase;

class TMQTTClient : public mosqpp::mosquittopp, public TMQTTClientBase
{
public:

    struct TConfig
    {
        int Port;
        std::string Host;
        int Keepalive;
        std::string Id;
        std::string User;
        std::string Password;

        TConfig()
            : Port(1883)
            , Host("localhost")
            , Keepalive(60)
            , Id("")
            , User("")
            , Password("")
        {}
    };

    TMQTTClient(const TConfig& config);

    void on_connect(int rc);
    void on_message(const struct mosquitto_message *message);
    void on_subscribe(int mid, int qos_count, const int *granted_qos);

    inline void Connect()
    {
        if (!MQTTConfig.User.empty()) {
            auto retVal = username_pw_set(MQTTConfig.User.c_str(), MQTTConfig.Password.c_str());
            if (retVal != MOSQ_ERR_SUCCESS) {
                std::cerr << "username_pw_set: " << retVal << std::endl;
                exit(retVal);
            }
        }
        auto retVal = connect(MQTTConfig.Host.c_str(), MQTTConfig.Port, MQTTConfig.Keepalive);
        if (retVal != MOSQ_ERR_SUCCESS) {
            std::cerr << "connect: " << retVal << std::endl;
            exit(retVal);
        }

    };
    inline void ConnectAsync() { connect_async(MQTTConfig.Host.c_str(), MQTTConfig.Port, MQTTConfig.Keepalive); };

    int Publish(int *mid, const std::string& topic, const std::string& payload="", int qos=0, bool retain=false);

    // Loop for a given duration in milliseconds. The underlying select timeout
    // is set by 'timeout' parameter
    int LoopFor(int duration, int timeout = 60);

    inline int LoopForever(int timeout=-1, int max_packets=1) { return this->loop_forever(timeout, max_packets);};
    void StartLoop() { loop_start(); }
    void StopLoop() { disconnect(); loop_stop(); }

    //~ using mosqpp::mosquittopp::subscribe;
    int Subscribe(int *mid, const std::string& sub, int qos=0);

    std::string Id() const { return MQTTConfig.Id; }

protected:
    const TConfig MQTTConfig;

private:
    std::list<PMQTTObserver> Observers;
};

typedef std::shared_ptr<TMQTTClient> PMQTTClient;

class TMQTTPrefixedClient : public TMQTTClient
{
    using Base = TMQTTClient;
public:
    TMQTTPrefixedClient(std::string prefix, const TConfig& config)
        : TMQTTClient(config)
        , Prefix(std::move(prefix))
    {}

    int Publish(int *mid, const std::string& topic, const std::string& payload="", int qos=0, bool retain=false) override;
    int Subscribe(int *mid, const std::string& sub, int qos=0) override;
    void on_message(const struct mosquitto_message *message) override;

private:
    std::string Prefix;
};

class TMQTTWrapper : public TMQTTClient, public IMQTTObserver,
                     public std::enable_shared_from_this<TMQTTWrapper> {
public:
    TMQTTWrapper(const TConfig& config): TMQTTClient(config) {}
    void Init() { Observe(shared_from_this()); }
};

class TMQTTPrefixedWrapper : public TMQTTPrefixedClient, public IMQTTObserver,
                     public std::enable_shared_from_this<TMQTTPrefixedWrapper>
{
    using Base = TMQTTPrefixedClient;
public:
    TMQTTPrefixedWrapper(std::string prefix, const TConfig& config)
        : Base(prefix, config)
    {}

    void Init() { Observe(shared_from_this()); }
};
