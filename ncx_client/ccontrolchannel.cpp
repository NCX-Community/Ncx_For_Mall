#include "client.h"

CControlChannel::CControlChannel(EventLoop* loop, CControlChannelArgs args) :
    loop_(loop),
    args_(args)
{
    control_client_ = std::make_unique<Client>(loop_, args_.server_addr_);
}

void CControlChannel::do_control_channel_handshake(std::shared_ptr<Connection> server_conn)
{
    // set wait ack callback
    control_client_->set_on_message_cb(
        [this](std::shared_ptr<Connection> conn, Buffer* buf) {
            wait_ack(conn, buf);
        }
    );
    // send control channel hello
    protocol::Hello hello;
    hello.set_hello_type(protocol::Hello::CONTROL_CHANNEL_HELLO);
    std::string send_msg = hello.SerializeAsString();
    std::string send_msg_with_header = PROTOMSGUTIL::HeaderInstaller(send_msg);
    server_conn->Send(send_msg_with_header);
}

void CControlChannel::wait_ack(std::shared_ptr<Connection> server_conn, Buffer* conn_input_buf) 
{
    std::puts("WAIT ACK CALL!");
    uint32_t msg_len = PROTOMSGUTIL::CanReadMsg(conn_input_buf);
    // 没有完整的消息可读
    if(!msg_len) return;
    // read Ack
    protocol::Ack ack;
    std::string recv_ack = conn_input_buf->RetrieveAsString(msg_len);
    if(!ack.ParseFromString(recv_ack)) {
        std::cerr << "ParseFromString failed" << std::endl;
    }

    // handle ack
    switch (ack.ack_content()) {
        case protocol::Ack::OK :
            std::puts("RECV ACK OK");
            break;
        case protocol::Ack::AUTH_ERROR :
            break;
    }
}

void CControlChannel::start()
{
    // 连接前设置连接成功回调函数
    // std::puts("CONTROL CHANNEL CALLBACK CALL!");
    control_client_->set_on_connect_cb(
        std::bind(&CControlChannel::do_control_channel_handshake, this, std::placeholders::_1)
    );
    // 连接
    control_client_->connect();
}