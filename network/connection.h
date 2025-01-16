#ifndef CONNECTION_H
#define CONNECTION_H
#include "util.h"
#include "buffer.h"
#include "InetAddress.h"

enum ConnectionState {
    CONNECTING,
    CONNECTED,
    DISCONNECTING,
    DISCONNECTED
};

class Connection : public std::enable_shared_from_this<Connection>{
public:
    DISALLOW_COPY_AND_MOVE(Connection);
    Connection(int client_fd, EventLoop*er, const InetAddress& local, const InetAddress& peer);
    ~Connection();

    void ConnectionEstablished();
    void ConnectionConstructor();

    void handle_conn();
    void set_conn_handle(std::function<void(const std::shared_ptr<Connection>&)> on_conn);

    void handle_message();
    void set_message_handle(std::function<void(const std::shared_ptr<Connection>&, Buffer*)> on_message);

    /// close 
    void handle_close();
    void set_disconnect_client_handle(std::function<void(const std::shared_ptr<Connection>&)> disconnectClient);
    void force_close();

    void handle_data_in();
    void handle_data_out();

    void Recv(std::string& buf);
    void Recv(std::string& buf, size_t len);
    void Send(const std::string& msg);
    
    int get_fd() const;
    int get_conn_id() const;
    ConnectionState get_state() const;
    EventLoop* get_epoll_run() const;

    void set_nonblocking();

private:
    int conn_id;
    int client_fd;
    ConnectionState state;

    InetAddress local_addr_;
    InetAddress peer_addr_;

    EventLoop* loop_;

    std::unique_ptr<Channel> channel;
    std::unique_ptr<Buffer> input_buffer;   // 输入缓冲区
    std::unique_ptr<Buffer> output_buffer;  // 输出缓冲区

    /// callback
    std::function<void(const std::shared_ptr<Connection>&)> on_conn_;
    std::function<void(const std::shared_ptr<Connection>&, Buffer*)> on_message_;
    std::function<void(const std::shared_ptr<Connection>&)> on_close_;
    void Read(); 
    void Write(); 
    void ReadNonBlocking();
    void WriteNonBlocking();
};

#endif