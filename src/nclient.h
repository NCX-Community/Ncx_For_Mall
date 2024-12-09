#ifndef NCLIENT_H
#define NCLIENT_H
#include "util.h"

const char* NSERV_IP = "127.0.0.1";
const uint16_t NSERV_PORT = 7000;

class NClient {
public: 
    NClient();
    void run_client();
private:
    std::unique_ptr<TcpSocket> client_;
};

#endif