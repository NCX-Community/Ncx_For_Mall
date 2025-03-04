#include <grpcpp/ext/proto_server_reflection_plugin.h>
#include <grpcpp/grpcpp.h>
#include <grpcpp/health_check_service_interface.h>

#include "util.h"
#include "client.pb.h"
#include "client.grpc.pb.h"
#include "ccontrolchannel.h"
#include "InetAddress.h"
#include "nclient.h"

using grpc::Channel;
using grpc::ClientContext;
using grpc::Status;

using protocol::ncx_client::ConfigInfo;
using protocol::ncx_client::ConfigInfoReq;
using protocol::ncx_client::ConfigInfoResp;

class ConfigInfoClient {
public: 
    ConfigInfoClient();
    void AskConfigInfo(const std::string& token);

private:
    std::shared_ptr<Channel> channel_;
    std::unique_ptr<ConfigInfo::Stub> stub_;

    std::vector<CControlChannelArgs> parseConfig(const ConfigInfoResp& resp);
};