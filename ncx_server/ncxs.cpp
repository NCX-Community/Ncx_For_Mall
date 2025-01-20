#include "nserver.h"
#include "InetAddress.h"
#include <iostream>

int 
main()
{
    struct SControlChannelArgs sc_args {
        InetAddress("127.0.0.1", 7777),
        256        
    };
    struct ServerArgs args {
        InetAddress("127.0.0.1", 8888),
        256,
        sc_args
    };

    NServer ncxs(args);
    ncxs.run_server();
}