#include "connection.h"
#include "socket.h"
#include "channel.h"
#include "buffer.h"
#include "epoll_run.h"

Connection::Connection(int _client_fd, EpollRun *_er) : client_fd(_client_fd), er(_er)
{
    // 创建一个随机数生成器
    std::random_device rd;                         // 用于生成种子
    std::mt19937 gen(rd());                        // 使用Mersenne Twister算法的随机数生成器
    std::uniform_int_distribution<> dis(1, 10000); // 定义随机数分布范围
    this->conn_id = dis(gen);

    if (er)
    {
        channel = std::make_unique<Channel>(er, client_fd);
        std::function<void()> cb = std::bind(&Connection::handle_message, this);
        this->channel->set_read_callback(cb);
        this->channel->set_et();
    }

    input_buffer = std::make_unique<Buffer>();
    output_buffer = std::make_unique<Buffer>();
}

void Connection::ConnectionEstablished() {
    this->channel->set_tie(shared_from_this());
    this->channel->enableRead();
    state = ConnectionState::CONNECTED;
}

Connection::~Connection() {};

void Connection::ConnectionConstructor() {
    er->delete_channel(channel.get());
}

void Connection::set_message_handle(std::function<void(Connection *)> on_message)
{
    on_message_ = std::move(on_message);
}

void Connection::handle_message()
{
    if (on_message_)
    {
        on_message_(this);
    }
}

void Connection::set_disconnect_client_handle(std::function<void(const std::shared_ptr<Connection>&)> disconnectClient)
{
    on_close_ = std::move(disconnectClient);
}

void Connection::handle_close()
{
    if (state != ConnectionState::DISCONNECTED)
    {
        state = ConnectionState::DISCONNECTED;
        if (on_close_)
        {
            on_close_(shared_from_this());
        }
    }
}

/// buffer function set

void Connection::set_output_buffer(const char *data, size_t len) { output_buffer->set_buf(data, len); }
Buffer *Connection::get_output_buffer() { return output_buffer.get(); }
Buffer *Connection::get_input_buffer() { return input_buffer.get(); }

void Connection::Send(const char *msg, size_t len)
{
    output_buffer->append(msg, len);
    Write();
}

void Connection::Send(const char *msg)
{
    size_t len = strlen(msg);
    output_buffer->set_buf(msg, len);
    Write();
}

void Connection::Send(const std::string &msg)
{
    output_buffer->set_buf(msg.c_str(), msg.size());
    Write();
}

void Connection::Write()
{
    WriteNonBlocking();
    output_buffer->clear();
}

void Connection::Read()
{
    input_buffer->clear();
    ReadNonBlocking();
}

void Connection::ReadNonBlocking()
{
    char buf[1024];
    while (true)
    {
        int read_bytes = read(client_fd, buf, 1024);
        if (read_bytes > 0)
        {
            input_buffer->append(buf, read_bytes);
        }
        else if (read_bytes < 0)
        {
            // read error
            if (errno == EAGAIN || errno == EWOULDBLOCK)
            {
                break;
            }
            else if (errno == EINTR)
            {
                continue;
            }
        }
        else
        {
            handle_close();
            break;
        }
    }
}

void Connection::WriteNonBlocking()
{
    ssize_t send_bytes = output_buffer->size();
    char send_buf[send_bytes];
    bzero(send_buf, send_bytes);
    memcpy(send_buf, output_buffer->data(), send_bytes);

    ssize_t send_left = send_bytes;
    while (send_left > 0)
    {
        ssize_t sended_bytes = write(client_fd, send_buf, send_bytes);
        if (sended_bytes < 0)
        {
            if (errno == EAGAIN || errno == EWOULDBLOCK)
            {
                break;
            }
            else if (errno == EINTR)
            {
                continue;
            }
            else
            {
                handle_close();
                break;
            }
        }
        send_left -= sended_bytes;
    }
}

/// @brief  get function set

int Connection::get_conn_id() const
{
    return conn_id;
}

int Connection::get_fd() const
{
    return client_fd;
}

ConnectionState Connection::get_state() const
{
    return state;
}

EpollRun *Connection::get_epoll_run() const
{
    return er;
}