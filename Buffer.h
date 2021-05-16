/**
 * @file Buffer.h
 * @author kangjinci (you@domain.com)
 * @brief 
 * @version 0.1
 * @date 2021-05-15
 * 
 * @copyright Copyright (c) 2021
 * 
 */
#pragma once
#include <vector>
#include <cstring>
#include <iostream>


namespace kjc {
class Buffer {
private:
    char initial[64] = {0};
    char* buf = nullptr;
    int len = 0, cap = 64;
public:
    Buffer(const char* s = 0) {
        if (s) {
            int n = strlen(s);
            resize(n + 1);
            strcpy(buf, s);
            len = n;
            buf[len] = '\0';
        }
    }
    ~Buffer() {
        if (buf != initial)
            delete[] buf;
        buf = nullptr;
        len = 0;
    }
    Buffer(const Buffer& rhs) {
        resize(rhs.len + 1);
        strcpy(buf, rhs.buf);
        len = rhs.len;
    }
    Buffer& operator=(const Buffer& rhs) {
        if (this == &rhs) {
            return *this;   
        }
        delete[] buf;
        buf = nullptr;
        len = 0;
        resize(rhs.len + 1);
        strcpy(buf, rhs.buf);
        buf[len] = '\0';
        return *this;
    }

    Buffer(Buffer&& rhs) noexcept {
        buf = rhs.buf;
        len = rhs.len;
        cap = rhs.cap;
        rhs.buf = nullptr;
        rhs.len = 0;
    }
    Buffer& operator=(Buffer&& rhs) noexcept {
        if (this != &rhs) {
            buf = rhs.buf;
            rhs.buf = nullptr;
            len = rhs.len;
            cap = rhs.cap;
            rhs.len = 0;
        }
        return *this;
    }

public:
    char& operator[](int index) {
        return buf[index];
    }
    const char& operator[](int index) const {
        return buf[index];
    }
    bool operator<(const Buffer& rhs) const {
        return strcmp(buf, rhs.buf) < 0;
    }
    bool operator>(const Buffer& rhs) const {
        return strcmp(buf, rhs.buf) > 0;
    }
    bool operator==(const Buffer& rhs) const {
        return strcmp(buf, rhs.buf) == 0;
    }
    bool operator!=(const Buffer& rhs) const {
        return !(*this == rhs);
    }
    Buffer& operator+(const Buffer& rhs) {
        resize(len + rhs.len + 1);
        strcpy(&buf[len], rhs.buf);
        len += rhs.len;
        return *this;
    }
    Buffer& operator+=(const Buffer& rhs) {
        return *this + rhs;
    }
    const char* get_cstr() const {
        return buf;
    }
    char& begin() {
        return buf[0];
    }
    char& end() {
        return buf[len];
    }
    friend std::ostream& operator<<(std::ostream& os, const Buffer& rhs);
public:
    inline int size() const {
        return len;
    }
    inline int capacity() const {
        return cap;
    }
    inline int length() const {
        return len;
    }
public:
    void push_back(char c) {
        resize(len + 2);
        buf[len++] = c;
        buf[len] = '\0';
    }
private:
    void resize(int n) {
        if (buf == nullptr) {
            buf = initial;
            cap = 64;
        }
        if (len + n > cap) {
            auto buff = new char[2 * cap + n];
            strcpy(buff, buf);
            if (buf != initial)
                delete[] buf;
            buf = buff;
            cap = 2 * cap + n;
        }
    }
};

std::ostream& operator<<(std::ostream& os, const Buffer& rhs) {
    os << rhs.buf;
    return os;
}
}
