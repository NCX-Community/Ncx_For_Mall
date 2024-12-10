#include "buffer.h"

void Buffer::append(const char* data, size_t len) {
    buf.append(data, len);
}

void Buffer::clear() {
    buf.clear();
}

void Buffer::clear(size_t len) {
    buf.erase(0, len);
}

size_t Buffer::size() {
    return buf.size();
}

const char* Buffer::data() {
    return buf.data();
}

void Buffer::set_buf(const char* data, size_t len) {
    buf.assign(data, len);
}