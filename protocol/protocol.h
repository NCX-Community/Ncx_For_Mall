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

    // 将 ControlChannelCmd 转换为 int
    inline int to_int(ControlChannelCmd cmd)
    {
        return static_cast<int>(cmd);
    }

    // 将 int 转换为 ControlChannelCmd
    inline ControlChannelCmd to_ControlChannelCmd(int value)
    {
        return static_cast<ControlChannelCmd>(value);
    }
}