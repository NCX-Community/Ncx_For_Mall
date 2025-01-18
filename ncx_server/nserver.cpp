#include "nserver.h"


NServer::NServer(ServerArgs args) :
    args_(args)
{
    // 资源创建
    loop_ = std::make_unique<EventLoop>();
    ncx_acceptor_ = std::make_shared<Server>(loop_.get(), args_.server_addr_, args_.backlog_);
    sc_map_ = std::make_unique<SControlChannelMap>();
                                       
    // 绑定初始连接的读事件回调--握手阶段
    ncx_acceptor_->bind_on_message(std::bind(&NServer::handle_new_connection, this, std::placeholders::_1, std::placeholders::_2));    
}

void NServer::handle_new_connection(std::shared_ptr<Connection> origin_conn, Buffer* conn_input_buf) 
{
    uint32_t msg_len = PROTOMSGUTIL::CanReadMsg(conn_input_buf);
    if(!msg_len) return;

    // 接受到足够的数据，可以read hello
    protocol::Hello hello;
    std::string recv_msg = conn_input_buf->RetrieveAsString(msg_len);
    if(!hello.ParseFromString(recv_msg)) {
        std::cerr << "ParseFromString failed" << std::endl;
    }
    // switch hello type
    switch (hello.hello_type()) {
        case protocol::Hello::CONTROL_CHANNEL_HELLO :
            std::puts("RECV CONTROL_CHANNEL_HELLO");
            break;
        case protocol::Hello::DATA_CHANNEL_HELLO :
            std::puts("RECV DATA_CHANNEL_HELLO");
            break;
    }

    protocol::Ack ack;
    ack.set_ack_content(protocol::Ack::OK);
    std::string send_ack = ack.SerializeAsString();
    std::string send_ack_with_header = PROTOMSGUTIL::HeaderInstaller(send_ack);
    origin_conn->Send(send_ack_with_header);
    // std::puts("SEND ACK OK");
}

void NServer::run_server()
{
    loop_->run();
}