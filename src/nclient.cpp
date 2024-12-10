#include "nclient.h"
#include "endpoint.h"
#include "socket.h"

NClient::NClient() {
    // bind and connect to server
    Endpoint remote_endpoint(NSERV_IP, NSERV_PORT);
    client_ = std::make_unique<TcpSocket>(true);
    client_->connect(remote_endpoint);

    
}

void NClient::run_client() {
}