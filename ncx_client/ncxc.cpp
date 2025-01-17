#include "nclient.h"
#include "ccontrolchannel.h"

/// @brief 客户端配置信息
const static char* SERVER_IP = "127.0.0.1";
const static uint16_t SERVER_PORT = 8888;

const static char* SERVICE_IP = "127.0.0.1";
const static uint16_t SERVICE_PORT = 6666;

int 
main() 
{
    // 模拟客户端配置信息
    CControlChannelArgs args {
        InetAddress(SERVER_IP, SERVER_PORT),
        InetAddress(SERVICE_IP, SERVICE_PORT)
    };

    std::vector<CControlChannelArgs> cc_set = {args};

    NClient client(cc_set);
    client.run_client();
}