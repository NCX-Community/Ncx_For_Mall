#include "connection.h"
#include "socket.h"
#include "channel.h"

Connection::Connection(Socket *sock, EpollRun *er)
{
    // 创建一个随机数生成器
    std::random_device rd;                         // 用于生成种子
    std::mt19937 gen(rd());                        // 使用Mersenne Twister算法的随机数生成器
    std::uniform_int_distribution<> dis(1, 10000); // 定义随机数分布范围

    this->conn_id = dis(gen);
    this->sock = sock;
    this->er = er;
    this->channel = new Channel(er, sock->get_fd());
    std::function<void()> cb = std::bind(&Connection::handleRead, this);
    this->channel->setReadHandleFunc(cb);
    this->channel->enableRead();
    this->channel->setET();
    this->buf = new Buffer();
}

Connection::~Connection()
{
    delete channel;
    delete sock;
    delete buf;
}

int Connection::get_id()
{
    return conn_id;
}

void Connection::handleRead()
{
    // printf("handleRead\n");
    char buf[1024];
    bzero(buf, sizeof(buf));
    while (true)
    { // 由于使用非阻塞IO，读取客户端buffer，一次读取buf大小数据，直到全部读取完毕
        bzero(&buf, sizeof(buf));
        ssize_t bytes_read = read(sock->get_fd(), buf, sizeof(buf));
        if (bytes_read > 0)
        {
            this->buf->append(buf, bytes_read);
            printf("message from client fd %d: %s\n", sock->get_fd(), this->buf->data());
            const char *msg = "hello i am server!";
            write(sock->get_fd(), msg, strlen(msg));
        }
        else if (bytes_read == -1 && errno == EINTR)
        { // 客户端正常中断、继续读取
            continue;
        }
        else if (bytes_read == -1 && ((errno == EAGAIN) || (errno == EWOULDBLOCK)))
        { // 非阻塞IO，这个条件表示数据全部读取完毕
            break;
        }
        else if (bytes_read == 0)
        { // EOF，客户端断开连接
            // printf("client disconnect\n");
            disconnectClient(conn_id);
            break;
        }
    }
}

void Connection::setDisconnectClient(std::function<void(int)> disconnectClient)
{
    this->disconnectClient = disconnectClient;
}

int Connection::get_fd()
{
    return sock->get_fd();
}