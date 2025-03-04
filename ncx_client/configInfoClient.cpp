#include "configInfoClient.h"

ConfigInfoClient::ConfigInfoClient() 
:   channel_(grpc::CreateChannel("localhost:0", grpc::InsecureChannelCredentials())),
    stub_(ConfigInfo::NewStub(channel_)) {}

void ConfigInfoClient::AskConfigInfo(const std::string& token) {
    ClientContext cx;
    ConfigInfoReq req;
    ConfigInfoResp resp;
    
    req.set_token(token);
    std::cout<<"Client begin to ask config info with token "<< token << std::endl;
    Status status = stub_->AskConfigInfo(&cx, req, &resp);

    if(status.ok()) {
        std::cout<<"Client get config info: "<< resp.DebugString() << std::endl;
        // parse config files and run client
        NClient client(parseConfig(resp));
        client.run_client();
    }
    else {
        std::cout<<"Client get config info failed with error code: "<< status.error_code() << std::endl;
        exit(1);
    }
}

std::vector<CControlChannelArgs> ConfigInfoClient::parseConfig(const ConfigInfoResp& resp) {
    std::vector<CControlChannelArgs> args;
    for(int i = 0; i < resp.service_configs_size(); i++) {
        struct CControlChannelArgs arg {
            InetAddress(resp.service_configs(i).server_ip().c_str(), static_cast<uint16_t>(resp.service_configs(i).server_port())),
            resp.service_configs(i).service_name(),
            InetAddress(resp.service_configs(i).service_ip().c_str(), static_cast<uint16_t>(resp.service_configs(i).service_port())),
            static_cast<uint16_t>(resp.service_configs(i).proxy_port())
        };

        args.emplace_back(CControlChannelArgs(std::move(arg)));
    }

    return args;
}