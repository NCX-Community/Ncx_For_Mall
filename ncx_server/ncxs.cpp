#include "nserver.h"
#include "InetAddress.h"
#include <iostream>

int 
main()
{
    struct ServerArgs args {
        InetAddress("127.0.0.1", 8888),
        256
    };

    NServer ncxs(args);
    ncxs.run_server();
}