#include "exchannel.h"
#include "connection.h"

ExChannel::ExChannel(Connection* conn1, Connection* conn2): conn1(conn1), conn2(conn2) {}
ExChannel::~ExChannel() {}
void ExChannel::handle_exchange() {
    /// conn1 read, send to conn2
    /// send to conn1, transfer to conn2
    conn1->Read();
    // 把conn1的input buffer复制到conn2的output Buffer
    size_t send_bytes = conn1->get_input_buffer()->size();
    char send_buf[send_bytes];
    bzero(send_buf, send_bytes);
    memcpy(send_buf, conn1->get_input_buffer()->data(), send_bytes);
    conn2->get_output_buffer()->clear();
    conn2->get_output_buffer()->append(send_buf, send_bytes);
    conn2->Write();
}