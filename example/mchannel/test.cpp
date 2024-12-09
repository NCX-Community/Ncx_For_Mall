#include "util.h"
#include "musl_channel.h"

void SendThreadFunc1(std::shared_ptr<MuslChannelTx> sender) {
    const char* message = "Hello, I am thread 1!";
    while (true) {
        sender->send(message);
        std::this_thread::sleep_for(std::chrono::seconds(1));
    }
}

void SendThreadFunc2(std::shared_ptr<MuslChannelTx> sender) {
    const char* message = "Hello, I am thread 2!";
    while (true) {
        sender->send(message);
        std::this_thread::sleep_for(std::chrono::seconds(1));
    }
}

void ReceiveThreadFunc(std::unique_ptr<MuslChannelRx> receiver) {
    while (true) {
        char* message = receiver->receive<char*>();
        std::cout << "Received: " << message << std::endl;
    }
}

int main() {
    MuslChannel channel;
    std::shared_ptr<MuslChannelTx> sender1 = channel.getSender();
    std::shared_ptr<MuslChannelTx> sender2 = channel.getSender();
    std::unique_ptr<MuslChannelRx> receiver = channel.getReceiver();
    std::thread sendThread(SendThreadFunc1, sender1);
    std::thread sendThread2(SendThreadFunc2, sender2);
    std::thread recvThread(ReceiveThreadFunc, std::move(receiver));
    sendThread.join();
    recvThread.join();
}