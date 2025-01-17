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
    // read hello
    protocol::Hello hello;
    size_t hello_len = sizeof(hello);
    // hello消息被分包，需要等待完整消息
    if(conn_input_buf->readAbleBytes() == 0) return;

    std::string recv_msg = conn_input_buf->RetrieveAllAsString();
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

    // 处理完成发送ack
    protocol::Ack ack;
    ack.set_ack_content(protocol::Ack::OK);
    std::string send_ack = ack.SerializeAsString();
    origin_conn->Send(send_ack);
    // std::puts("SEND ACK OK");
}

void NServer::run_server()
{
    loop_->run();
}