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
    Connection(int client_fd, EpollRun*er);
    ~Connection();

    void ConnectionEstablished();
    void ConnectionConstructor();

    void handle_message();
    void handle_close();

    void set_disconnect_client_handle(std::function<void(const std::shared_ptr<Connection>&)> disconnectClient);
    void set_message_handle(std::function<void(Connection*)> on_message);

    void set_output_buffer(const char* data, size_t len);
    Buffer* get_output_buffer();
    Buffer* get_input_buffer();

    void Read();
    void Write();
    void Send(const char* msg, size_t len);
    void Send(const char* msg);
    void Send(const std::string& msg);
    
    int get_fd() const;
    int get_conn_id() const;
    ConnectionState get_state() const;
    EpollRun* get_epoll_run() const;
private:
    int conn_id;
    int client_fd;
    ConnectionState state;

    EpollRun* er;

    std::unique_ptr<Channel> channel;
    std::unique_ptr<Buffer> input_buffer;
    std::unique_ptr<Buffer> output_buffer;

    /// server.h
    std::function<void(const std::shared_ptr<Connection>&)> on_close_;
    std::function<void(Connection*)> on_message_;

    void ReadNonBlocking();
    void WriteNonBlocking();
};

#endif