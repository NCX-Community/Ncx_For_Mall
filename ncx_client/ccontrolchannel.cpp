#include "ccontrolchannel.h"
#include "protocol.pb.h"
#include "connection.h"
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
    server_conn->Send(send_msg);
}

void CControlChannel::wait_ack(std::shared_ptr<Connection> server_conn, Buffer* conn_input_buf) 
{
    std::puts("WAIT ACK CALL!");
    // read Ack
    protocol::Ack ack;
    size_t ack_len = sizeof(ack);
    // ack消息被分包，需要等待完整消息
    if(conn_input_buf->readAbleBytes() == 0) return;
    std::string recv_ack = conn_input_buf->RetrieveAllAsString();
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