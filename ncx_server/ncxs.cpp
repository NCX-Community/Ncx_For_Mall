#include "nserver.h"
#include "InetAddress.h"
#include "config.h"
#include <iostream>

const char* CONFIG_FILE = "ncxs.toml";

int 
main()
{
    // read server config 
    ServerConfig config(CONFIG_FILE);
    std::puts("SERVERCONFIG LOAD SUCCESS!");
    NServer ncxs(config.parseAsServerArgs());
    ncxs.run_server();
}