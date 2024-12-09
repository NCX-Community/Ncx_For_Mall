#ifndef MUSL_CHANNEL_H
#define MUSL_CHANNEL_H
#include "util.h"
#include "zmq.hpp"
#include <memory>
#include <string>
#include <sstream>
#include <atomic>

// 生成唯一地址的辅助函数
inline std::string generateUniqueAddr() {
    static std::atomic<int> counter{0};
    std::stringstream ss;
    ss << "inproc://channel_" << counter++;
    return ss.str();
}

// 多生产者单消费者通道
class MuslChannel {
public:
    MuslChannel();
    ~MuslChannel();

    zmq::context_t& getContext();
    std::shared_ptr<class MuslChannelTx> getSender();
    std::unique_ptr<class MuslChannelRx> releaseReceiver();
    const std::string& getAddr() const;

private:
    zmq::context_t context_;
    std::string addr_;
    std::shared_ptr<class MuslChannelTx> sender_;
    std::unique_ptr<class MuslChannelRx> receiver_;
};

class MuslChannelTx {
public:
    MuslChannelTx(MuslChannel& channel);
    ~MuslChannelTx();

    template <typename T>
    void send(const T& message);

private:
    zmq::socket_t sender_;
};

class MuslChannelRx {
public:
    MuslChannelRx(MuslChannel& channel);
    ~MuslChannelRx();

    template <typename T>
    T receive();

private:
    zmq::socket_t receiver_;
    std::string addr_;
};

// MuslChannel类实现
inline MuslChannel::MuslChannel() 
    : context_(1), addr_(generateUniqueAddr()) {
    sender_ = std::make_shared<MuslChannelTx>(*this);
    receiver_ = std::make_unique<MuslChannelRx>(*this);
}

inline MuslChannel::~MuslChannel() {}

inline zmq::context_t& MuslChannel::getContext() {
    return context_;
}

inline std::shared_ptr<MuslChannelTx> MuslChannel::getSender() {
    return sender_;
}

inline std::unique_ptr<MuslChannelRx> MuslChannel::releaseReceiver() {
    return std::move(receiver_);
}

inline const std::string& MuslChannel::getAddr() const {
    return addr_;
}

// MuslChannelTx类实现
inline MuslChannelTx::MuslChannelTx(MuslChannel& channel)
    : sender_(channel.getContext(), ZMQ_PUSH) {
    sender_.connect(channel.getAddr());
}

inline MuslChannelTx::~MuslChannelTx() {
    sender_.close();
}

template <typename T>
inline void MuslChannelTx::send(const T& message) {
    zmq::message_t zmq_message(sizeof(T));
    memcpy(zmq_message.data(), &message, sizeof(T));
    sender_.send(zmq_message, zmq::send_flags::dontwait);
}

// MuslChannelRx类实现
inline MuslChannelRx::MuslChannelRx(MuslChannel& channel)
    : receiver_(channel.getContext(), ZMQ_PULL), addr_(channel.getAddr()) {
    receiver_.bind(addr_);
}

inline MuslChannelRx::~MuslChannelRx() {
    receiver_.close();
}

template <typename T>
inline T MuslChannelRx::receive() {
    zmq::message_t zmq_message;
    receiver_.recv(zmq_message, zmq::recv_flags::none);
    T message;
    memcpy(&message, zmq_message.data(), sizeof(T));
    return message;
}

#endif // MUSL_CHANNEL_H