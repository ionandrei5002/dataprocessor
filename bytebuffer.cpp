#include "bytebuffer.h"

ByteBuffer::ByteBuffer(ViewByteBuffer &ot)
{
    _size = ot._size;
    _data = new char[ot._size];
    memcpy(_data, ot._data, ot._size);
}
ViewByteBuffer::ViewByteBuffer(ByteBuffer& ot)
{
    _size = ot._size;
    _data = ot._data;
}
