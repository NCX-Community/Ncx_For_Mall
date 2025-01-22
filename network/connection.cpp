#include "connection.h"
#include "socket.h"
#include "channel.h"
#include "buffer.h"
#include "EventLoop.h"

Connection::Connection(int _client_fd, EventLoop *loop, const InetAddress &local, const InetAddress &peer) : 
            client_fd(_client_fd), 
            loop_(loop),
            state(ConnectionState::CONNECTING),
            local_addr_(local),
            peer_addr_(peer)
{
    // 创建一个随机数生成器
    std::random_device rd;                         // 用于生成种子
    std::mt19937 gen(rd());                        // 使用Mersenne Twister算法的随机数生成器
    std::uniform_int_distribution<> dis(1, 10000); // 定义随机数分布范围
    this->conn_id = dis(gen);

    if (loop_)
    {
        channel = std::make_unique<Channel>(loop_, client_fd);
        channel->set_read_callback([this](){
            this->handle_data_in();
        });
        channel->set_write_callback([this](){
            this->handle_data_out();
        });
    }

    input_buffer = std::make_unique<Buffer>();
    output_buffer = std::make_unique<Buffer>();
}

// 在连接回调函数和各种参数设置完成后调用，调用后连接体开始处理读写事件
void Connection::ConnectionEstablished()
{
    this->channel->set_tie(shared_from_this());
    this->channel->enableRead();
    //this->channel->enableWrite();

    state = ConnectionState::CONNECTED;
    handle_conn();
}

Connection::~Connection() { std::printf("connection %d destructed\n", conn_id); };

// 在连接体析构时调用，用于删除连接体的channel
void Connection::ConnectionConstructor()
{
    if (state == ConnectionState::CONNECTED)
    {
        state = ConnectionState::DISCONNECTED;
        channel->disableAll();
    }
    channel->remove();
}

void Connection::set_conn_handle(std::function<void(const std::shared_ptr<Connection> &)> on_conn){ on_conn_ = std::move(on_conn); }
void Connection::handle_conn(){ if (on_conn_) { on_conn_(shared_from_this()); } }
void Connection::set_message_handle(std::function<void(const std::shared_ptr<Connection> &, Buffer*)> on_message){ on_message_ = std::move(on_message); }
void Connection::handle_message()
{
    if (on_message_)
    {
        on_message_(shared_from_this(), input_buffer.get());
    }
    else std::puts("handle message doesn't init!");
}

// 连接体非阻塞读取数据进Buffer
void Connection::handle_data_in()
{
    ReadNonBlocking();
    handle_message();
}

// 连接体非阻塞发送Buffer中的数据
void Connection::handle_data_out(){ WriteNonBlocking(); }

void Connection::set_close_handle(std::function<void(const std::shared_ptr<Connection> &)> disconnectClient) { on_close_ = std::move(disconnectClient); }
void Connection::handle_close()
{
    if (state != ConnectionState::DISCONNECTED)
    {
        if (on_close_){ on_close_(shared_from_this()); }
        if (notice_on_close_)
        state = ConnectionState::DISCONNECTED;
    }
}

// 强制关闭连接
void Connection::force_close()
{
    if(state == ConnectionState::CONNECTED || state == ConnectionState::DISCONNECTING)
    {
        state = ConnectionState::DISCONNECTING;
        loop_->run_on_onwer_thread(
            std::bind(&Connection::handle_close, this)
        );
    }
}

void Connection::notice_close() { if (notice_on_close_) {  notice_on_close_(); } }
void Connection::set_close_notice(std::function<void()> notice_on_close) { notice_on_close_ = std::move(notice_on_close); }

void Connection::Recv(std::string& msg) { Recv(msg, static_cast<size_t>(msg.size())); }

void Connection::Recv(std::string& msg, size_t len) { msg = input_buffer->RetrieveAsString(len); }

void Connection::Send(const std::string& msg) 
{
    output_buffer->Append(msg);
    Write(); // todo: 监听写事件
}

void Connection::Write() { WriteNonBlocking(); }
void Connection::Read() { ReadNonBlocking(); }

void Connection::ReadNonBlocking()
{
    char buf[1024];
    ssize_t read_bytes = 0;
    while (true)
    {
        read_bytes = read(client_fd, buf, sizeof(buf));
        if (read_bytes > 0)
        {
            input_buffer->Append(buf, read_bytes);
        }
        else if (read_bytes < 0)
        {
            // read error
            if (errno == EAGAIN || errno == EWOULDBLOCK)
            {
                // 非阻塞模式下没有数据可读
                // std::puts("no data to read");
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
    std::string send_buf = output_buffer->RetrieveAllAsString();
    ssize_t send_bytes = send_buf.size();
    ssize_t send_left = send_bytes;
    
    while (send_left > 0)
    {
        ssize_t sended_bytes = write(client_fd, send_buf.data(), send_bytes);
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

/// get function set
int Connection::get_conn_id() const { return conn_id; }
int Connection::get_fd() const { return client_fd; }
ConnectionState Connection::get_state() const { return state; }
EventLoop *Connection::get_epoll_run() const { return loop_; }
Buffer *Connection::get_input_buffer() const { return input_buffer.get(); }
Buffer *Connection::get_output_buffer() const { return output_buffer.get(); }

void Connection::set_nonblocking()
{
    int flags = fcntl(client_fd, F_GETFL, 0);
    fcntl(client_fd, F_SETFL, flags | O_NONBLOCK);
}