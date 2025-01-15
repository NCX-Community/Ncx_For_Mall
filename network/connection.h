#ifndef CONNECTION_H
#define CONNECTION_H
#include "util.h"
#include "buffer.h"

enum ConnectionState {
    Invalid = -1,
    CONNECTED = 0,
    DISCONNECTED
};

class Connection : public std::enable_shared_from_this<Connection>{
public:
    DISALLOW_COPY_AND_MOVE(Connection);
    Connection(int client_fd, EventLoop*er);
    ~Connection();

    void ConnectionEstablished();
    void ConnectionConstructor();

    void handle_message();
    void set_message_handle(std::function<void(const std::shared_ptr<Connection>&)> on_message);

    void handle_close();
    void set_disconnect_client_handle(std::function<void(const std::shared_ptr<Connection>&)> disconnectClient);

    void handle_data_in();
    void handle_data_out();

    void Recv(std::string& buf);
    void Recv(std::string& buf, size_t len);
    void Send(const std::string& msg);
    
    int get_fd() const;
    int get_conn_id() const;
    ConnectionState get_state() const;
    EventLoop* get_epoll_run() const;

    void setExChannel(Transfer* exchannel);
    void enableExchange();

    void set_nonblocking();

private:
    int conn_id;
    int client_fd;
    ConnectionState state;

    EventLoop* loop_;

    std::unique_ptr<Channel> channel;
    std::unique_ptr<Buffer> input_buffer;   // 输入缓冲区
    std::unique_ptr<Buffer> output_buffer;  // 输出缓冲区

    /// server.h
    std::function<void(const std::shared_ptr<Connection>&)> on_close_;
    std::function<void(const std::shared_ptr<Connection>&)> on_message_;

    void Read(); 
    void Write(); 
    void ReadNonBlocking();
    void WriteNonBlocking();
};

#endif