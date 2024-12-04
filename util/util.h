#ifndef UTIL_H
#define UTIL_H

#include<iostream>
#include<vector>
#include<cstring>
#include<unordered_map>
#include<sys/epoll.h>
#include<functional>
#include<unistd.h>
#include<sys/epoll.h>
#include<stdio.h>
#include<stdlib.h>
#include<arpa/inet.h>
#include<string>
#include<cerrno>
#include<memory>
#include<random>
#include<thread>
#include<queue>
#include<mutex>
#include<condition_variable>
#include<future>
#include<sys/eventfd.h>
#include<fcntl.h>

#include "log.h"

class EpollRun;
class Channel;
class Epoll;
class Endpoint;
class EpollRun;
class TcpSocket;
class Server;
class Acceptor;
class Connection;
class Buffer;
class EvPoolThread;
class EpThreadPool;
class ExChannel;

/// disallow copy constructor
#define DISALLOW_COPY(TypeName) \
    TypeName(const TypeName&) = delete; \
    void operator=(const TypeName&) = delete;

/// disallow move constructor
#define DISALLOW_MOVE(TypeName) \
    TypeName(TypeName&&) = delete; \
    void operator=(TypeName&&) = delete;

/// disallow copy and move constructor
#define DISALLOW_COPY_AND_MOVE(TypeName) \
    DISALLOW_COPY(TypeName) \
    DISALLOW_MOVE(TypeName)


#endif