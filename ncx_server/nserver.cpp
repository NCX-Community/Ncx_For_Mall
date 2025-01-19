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
            // handle control channel hello
            handle_control_channel_hello(origin_conn);
            break;
        case protocol::Hello::DATA_CHANNEL_HELLO :
            std::puts("RECV DATA_CHANNEL_HELLO");
            break;
    }
}

void NServer::handle_control_channel_hello(std::shared_ptr<Connection> origin_conn)
{
    // send hello back
    // 生成唯一标识controlchannel的id标识
    std::string nonce = UUID::GenerateUUIDAsString();
    protocol::Hello control_hello;
    control_hello.set_hello_type(protocol::Hello::CONTROL_CHANNEL_HELLO);
    control_hello.set_digest(nonce);
    std::string serialized_control_hello = control_hello.SerializeAsString();
    std::string control_hello_with_header = PROTOMSGUTIL::HeaderInstaller(serialized_control_hello);
    origin_conn->Send(control_hello_with_header);
    std::puts("SERVER SEND CONTROL_CHANNEL_HELLO");
}

void NServer::run_server()
{
    loop_->run();
}