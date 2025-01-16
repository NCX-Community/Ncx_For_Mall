#include "util.h"
#include "InetAddress.h"
#include "merror.h"
#include "socket.h"
#include "EventLoop.h"
#include "connection.h"
#include "client.h"

#include <unistd.h>

static const InetAddress SERVER_ADDR("0.0.0.0", 6667);

class EchoClient {
public:
    EchoClient(EventLoop* loop, const InetAddress& server_addr);
    ~EchoClient();
    void run();

private:
    EventLoop* loop_;
    std::unique_ptr<Client> client_;
};


EchoClient::EchoClient(EventLoop* loop, const InetAddress& server_addr) {
    loop_ = loop;
    client_ = std::make_unique<Client>(loop_, server_addr);
    client_->set_on_connect_cb([](std::shared_ptr<Connection> conn) {
        std::puts("connected to server, begin to send message");
        conn->Send("welcome to ncx!");
    });
    client_->set_on_message_cb([](std::shared_ptr<Connection> conn, Buffer* buf) {
        std::string msg = buf->RetrieveAllAsString();
        std::cout << "recv: " << msg.data() << std::endl;
        sleep(1);
        conn->Send(msg);
    });
}

EchoClient::~EchoClient() {}

void EchoClient::run() {
    client_->connect();
    loop_->run();
}

int main(void)
{
    EventLoop loop;
    EchoClient client(&loop, SERVER_ADDR);
    client.run();
    return 0;
}

