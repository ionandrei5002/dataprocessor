#ifndef BYTEBUFFER_H
#define BYTEBUFFER_H

#include <memory>
#include <memory.h>
#include <experimental/string_view>

class ByteBuffer
{
public:
    uint64_t _size = 0;
    char* _data = nullptr;
public:
    ByteBuffer():_size(0),_data(nullptr) {}
    ByteBuffer(uint64_t size, const char* data):_size(size)
    {
        _data = new char[size];
        memcpy(_data, data, size);
    }
    ByteBuffer(const ByteBuffer& ot)
    {
        _size = ot._size;
        _data = new char[ot._size];
        memcpy(_data, ot._data, ot._size);
    }
    ByteBuffer& operator=(const ByteBuffer& ot)
    {
        _size = ot._size;
        _data = new char[ot._size];
        memcpy(_data, ot._data, ot._size);
        return *this;
    }
    ByteBuffer(ByteBuffer&& ot)
    {
        _size = ot._size;
        _data = ot._data;
        ot._data = nullptr;
    }
    ByteBuffer& operator=(ByteBuffer&& ot)
    {
        _size = ot._size;
        _data = ot._data;
        ot._data = nullptr;
        return *this;
    }
    ~ByteBuffer()
    {
        if (_data != nullptr)
        {
            delete[] _data;
            _data = nullptr;
        }
        _size = 0;
    }
    friend std::ostream& operator<<(std::ostream& out, const ByteBuffer& ot)
    {
        for(uint64_t i = 0; i < ot._size; ++i)
        {
            out << ot._data[i];
        }

        return out;
    }
};

namespace std
{
    bool operator<(const ByteBuffer& lv, const ByteBuffer& rv)
    {
        return std::experimental::string_view(lv._data, lv._size) < std::experimental::string_view(rv._data, rv._size);
    }
}

#endif // BYTEBUFFER_H
