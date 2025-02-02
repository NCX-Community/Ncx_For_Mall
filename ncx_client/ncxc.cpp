#include "nclient.h"
#include "ccontrolchannel.h"
#include "config.h"

const char* CLIENT_CONFIG_FILE = "ncxc.toml";

int 
main() 
{
    // read client config 
    ClientConfig config(CLIENT_CONFIG_FILE);
    NClient client(config.parseAsControlChannelArgsVec());
    client.run_client();
}