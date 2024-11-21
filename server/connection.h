#ifndef CONNECTION_H
#define CONNECTION_H
#include "util.h"
#include "buffer.h"
class Connection {
public:
    Connection(Socket* sock, EpollRun*er);
    ~Connection() = default;
    int get_id();
    void handleRead();
    void setDisconnectClient(std::function<void(int)> disconnectClient);
private:
    int conn_id;
    Socket* sock;
    EpollRun* er;
    Channel* channel;
    Buffer* buf;
    std::function<void(int)> disconnectClient;
};

#endif