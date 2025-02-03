#include "nserver.h"
#include "InetAddress.h"
#include "config.h"
#include <iostream>

int 
main(int argc, char* argv[])
{
    // read server config 
    ServerConfig config(argv[1]);
    std::puts("SERVERCONFIG LOAD SUCCESS!");
    NServer ncxs(config.parseAsServerArgs());
    ncxs.run_server();
}