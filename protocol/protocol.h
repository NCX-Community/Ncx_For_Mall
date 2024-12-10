#include "util.h"
namespace PROTOCOL
{
    enum ControlChannelCmd
    {
        CreateDataChannel,
        HeartBeat,
    };

    enum DataChannelCmd
    {
        StartForwardTcp,
        StartForwardUdp,
    };

    enum Hello
    {
        ControlChannelHello,
        DataChannelHello,
    };

    Hello read_hello(int cli_fd);
}