#ifndef SCONTROLCHANNEL_H
#define SCONTROLCHANNEL_H

#include "util.h"

/// @brief 控制通道参数集
struct SControlChannelArgs 
{
    InetAddress proxy_addr_;    // 监听外部连接地址
    int backlog_ {256};
};

class SControlChannel 
{
public:
private:
};


#endif