#ifndef CONNECTION_H
#define CONNECTION_H
#include "util.h"
#include "buffer.h"

enum ConnectionState {
    Invalid = -1,
    CONNECTED,
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

    // data_in_callback
    void handle_data_in();

    // data_out_callback
    void handle_data_out();

    void set_output_buffer(const char* data, size_t len);
    Buffer* get_output_buffer();
    Buffer* get_input_buffer();

    void Read(); //fixme 置为私有
    void Write(); //fixme 置为私有

    void Recv(char* buf);
    void Recv(char* buf, size_t len);
    void Send(const char* msg, size_t len);
    void Send(const char* msg);
    void Send(const std::string& msg);
    
    int get_fd() const;
    int get_conn_id() const;
    ConnectionState get_state() const;
    EventLoop* get_epoll_run() const;

    void setExChannel(Transfer* exchannel);
    void enableExchange();

    void set_nonblocking();

    void flash_in_data();
    void flash_out_data();
private:
    int conn_id;
    int client_fd;
    ConnectionState state;

    EventLoop* er;

    std::unique_ptr<Channel> channel;
    std::unique_ptr<Buffer> input_buffer;   // 输入缓冲区
    std::unique_ptr<Buffer> output_buffer;

    bool is_in_transfer();
    std::unique_ptr<Transfer> exchannel_;

    /// server.h
    std::function<void(const std::shared_ptr<Connection>&)> on_close_;
    std::function<void(const std::shared_ptr<Connection>&)> on_message_;

    void ReadNonBlocking();
    void WriteNonBlocking();
};

#endif