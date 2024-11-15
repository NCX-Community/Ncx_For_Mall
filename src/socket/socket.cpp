// 动态分配socket接口
#include "socket.h"
#include "common.h"

Socket* create_socket(Type sock_tp) {
    switch (sock_tp) {
        case Type::TCP:
            return new TcpSocket();
        case Type::UDP:
            //todo
            return nullptr;
        case Type::RAW:
            //todo
            return nullptr;
        case Type::SEQPACKET:
            //todo
            return nullptr;
    }
}

