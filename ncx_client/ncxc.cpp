#include "nclient.h"
#include "ccontrolchannel.h"
#include "config.h"

int
main(int argc, char* argv[]) 
{
    // read client config 
    ClientConfig config(argv[1]);
    NClient client(config.parseAsControlChannelArgsVec());
    client.run_client();
}