#include <toml++/toml.hpp>
#include "util.h"
#include "InetAddress.h"
#include "singleton.h"
#include "nserver.h"
#include "nclient.h"

// #define TOML_EXCEPTIONS 0 

class Config
{
public:
    Config() = delete;
    Config(const char* path)
    {
        config = toml::parse_file(path);
    }
    ~Config() = default;
protected:
    toml::parse_result config;
};

class ServerConfig : public Config
{
public:
    ServerConfig() = delete;
    ServerConfig(const char* path) : Config(path) {}
    ~ServerConfig() = default;

    ServerArgs parseAsServerArgs() 
    {
        if(config.empty())
        {
            throw std::runtime_error("Failed To Parse Server Config File!");
        }
        toml::table* table = config.as_table();
        if(!table->at_path("server")) {
            throw std::runtime_error("Failed To Parse Server Config File! [ server didn't exist !]");
            if(!table->at_path("server.server_ip")) {
                throw std::runtime_error("Failed To Parse Server Config File! [ server_ip didn't exist !]");
            }
            if(!table->at_path("server.server_port")) {
                throw std::runtime_error("Failed To Parse Server Config File! [ server_port didn't exist !]");
            }
            if(!table->at_path("server.backlog")) {
                throw std::runtime_error("Failed To Parse Server Config File! [ backlog didn't exist !]");
            }
        }
        int backlog = table->at_path("server.backlog").as_integer()->get();
        std::string server_ip = table->at_path("server.server_ip").as_string()->get();
        int server_port = table->at_path("server.server_port").as_integer()->get();
        
        return ServerArgs{
            InetAddress(server_ip.c_str(), static_cast<uint16_t>(server_port)),
            backlog
        };
    }
};

class ClientConfig : public Config
{
public:
    ClientConfig() = delete;
    ClientConfig(const char* path) : Config(path) {}
    ~ClientConfig() = default;

    std::vector<CControlChannelArgs> parseAsControlChannelArgsVec() 
    {
        std::vector<CControlChannelArgs> result;

        if(config.empty())
        {
            throw std::runtime_error("Failed To Parse Client Config File!");
        }
        toml::table* table = config.as_table();
        if(!table->at_path("client")) {
            throw std::runtime_error("Failed To Parse Client Config File! [ client didn't exist !]");
            if(!table->at_path("client.server_ip")) {
                throw std::runtime_error("Failed To Parse Client Config File! [ server_ip didn't exist !]");
            }
            if(!table->at_path("client.server_port")) {
                throw std::runtime_error("Failed To Parse Client Config File! [ server_port didn't exist !]");
            }
        }
        std::string server_ip = table->at_path("client.server_ip").as_string()->get();
        int server_port = table->at_path("client.server_port").as_integer()->get();
        InetAddress server_addr(server_ip.c_str(), static_cast<uint16_t>(server_port));

        // parse all client service
        table->for_each([server_addr, &result] (const toml::key& key, auto val)
        {
            if(key == "client") { return; }
            auto service_name = key.data();
            std::string service_ip = val.as_table()->at_path("service_ip").as_string()->get();
            int service_port = val.as_table()->at_path("service_port").as_integer()->get();
            int proxy_port = val.as_table()->at_path("proxy_port").as_integer()->get();
            CControlChannelArgs args{
                server_addr,
                service_name,
                InetAddress(service_ip.c_str(), static_cast<uint16_t>(service_port)),
                static_cast<uint16_t>(proxy_port)
            };
            result.push_back(args);
        });

        return result;
    }
};