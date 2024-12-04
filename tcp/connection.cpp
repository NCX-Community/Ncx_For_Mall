#include "connection.h"
#include "socket.h"
#include "channel.h"
#include "buffer.h"
#include "epoll_run.h"
#include "exchannel.h"

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
        this->channel->set_et();
    }

    input_buffer = std::make_unique<Buffer>();
    output_buffer = std::make_unique<Buffer>();
}

// 在连接回调函数和各种参数设置完成后调用
void Connection::ConnectionEstablished()
{
    this->channel->set_tie(shared_from_this());

    if (on_data_in_)
    {
        this->channel->set_read_callback(std::bind(&Connection::handle_data_in, this));
        this->channel->enableRead();
    }

    if (on_data_out_)
    {
        this->channel->set_write_callback(std::bind(&Connection::handle_data_out, this));
        this->channel->enableWrite();
    }

    state = ConnectionState::CONNECTED;
}

Connection::~Connection() {};

void Connection::ConnectionConstructor()
{
    er->delete_channel(channel.get());
}

void Connection::set_message_handle(std::function<void(const std::shared_ptr<Connection>&)> on_message)
{
    on_message_ = std::move(on_message);
}

void Connection::handle_message()
{
    // printf("tcp connection handle message\n");
    if (on_message_)
    {
        on_message_(shared_from_this());
    }
}

void Connection::set_data_in_handle(std::function<void(const std::shared_ptr<Connection>&)> on_data_in)
{
    on_data_in_ = std::move(on_data_in);
}

void Connection::handle_data_in()
{
    if (on_data_in_)
    {
        on_data_in_(shared_from_this());
    }
    else
        std::puts("data in callback not init");
}

void Connection::set_data_out_handle(std::function<void(const std::shared_ptr<Connection>&)> on_data_out)
{
    on_data_out_ = std::move(on_data_out);
}

void Connection::handle_data_out()
{
    if (on_data_out_)
    {
        on_data_out_(shared_from_this());
    }
    else
        std::puts("data out callback not init");
}

void Connection::set_disconnect_client_handle(std::function<void(const std::shared_ptr<Connection> &)> disconnectClient)
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
        int read_bytes = read(client_fd, buf, sizeof(buf));
        printf("read %d bytes\n", read_bytes);
        if (read_bytes > 0)
        {
            input_buffer->append(buf, read_bytes);
        }
        else if (read_bytes < 0)
        {
            // read error
            if (errno == EAGAIN || errno == EWOULDBLOCK)
            {
                // 非阻塞模式下没有数据可读
                std::puts("no data to read");
                break;
            }
            else if (errno == EINTR)
            {
                // 读取被中断，继续读取
                continue;
            }
            else
            {
                // 其他错误，处理错误逻辑
                perror("read error");
                handle_close();
                break;
            }
        }
        else
        {
            // read 返回 0，表示连接已关闭
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

void Connection::setExChannel(ExChannel *exchannel)
{
    exchannel_ = std::unique_ptr<ExChannel>(exchannel);
}

void Connection::enableExchange()
{
    if (exchannel_)
    {
        // 设置读为读后转发
        this->channel->set_read_callback(std::bind(&ExChannel::handle_exchange, exchannel_.get()));
    }
}

void Connection::set_nonblocking() 
{
    int flags = fcntl(client_fd, F_GETFL, 0);
    fcntl(client_fd, F_SETFL, flags | O_NONBLOCK);
}