#include "protocol.h"

PROTOCOL::Hello PROTOCOL::read_hello(int cli_fd)
{
    Hello hello;
    ssize_t total_bytes_read = 0;
    ssize_t bytes_to_read = sizeof(Hello);
    char *buffer = reinterpret_cast<char *>(&hello);

    while (total_bytes_read < bytes_to_read)
    {
        ssize_t bytes_read = recv(cli_fd, buffer + total_bytes_read, bytes_to_read - total_bytes_read, 0);
        if (bytes_read > 0)
        {
            total_bytes_read += bytes_read;
        }
        else if (bytes_read == 0)
        {
            // 连接关闭
            // std::cerr << "Connection closed by peer" << std::endl;
            break;
        }
        else
        {
            if (errno == EAGAIN || errno == EWOULDBLOCK)
            {
                // 没有数据可读，继续尝试
                continue;
            }
            else
            {
                // 读取错误
                std::cerr << "Read error: " << strerror(errno) << std::endl;
                break;
            }
        }
    }

    if (total_bytes_read == bytes_to_read)
    {
        // 成功读取完整的 Hello 结构体
        return hello;
    }
    else
    {
        // 读取失败，处理错误情况
        throw std::runtime_error("Failed to read complete Hello structure");
    }
}