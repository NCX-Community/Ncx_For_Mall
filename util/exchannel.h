#ifndef EXCHANNEL_H
#define EXCHANNEL_H
#include "util.h"

// conn1 exchange data to conn2
class ExChannel {
public:
    DISALLOW_COPY_AND_MOVE(ExChannel);
    ExChannel(Connection* conn1, Connection* conn2);
    ~ExChannel();

    void handle_exchange();

private: 
    Connection* conn1;
    Connection* conn2;
};

#endif