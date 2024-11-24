#ifndef BUFFER_H
#define BUFFER_H
#include"util.h"

class Buffer {
private:
    std::string buf;
public:
    Buffer() = default;
    ~Buffer() = default;
    void set_buf(const char* data, size_t len);
    void append(const char* data, size_t len);
    void clear();
    const char* data();
    size_t size();
};

#endif