#ifndef TRANSFER_H
#define TRANSFER_H
#include "util.h"

// conn1 tranfer data to conn2
class Transfer {
public:
    DISALLOW_COPY_AND_MOVE(Transfer);
    Transfer(Connection* conn1, Connection* conn2);
    ~Transfer();

    void handle_transfer();
private: 
    Connection* conn1;
    Connection* conn2;    
};

#endif