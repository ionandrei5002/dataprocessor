#ifndef AGGREGATORS_H
#define AGGREGATORS_H

#include "types.h"
#include "bytebuffer.h"
#include "value.h"

#include <set>

struct AggType
{
    enum type
    {
        NONE = 00,
        DISTINCTC_COUNT = 10
    };
};

class Aggregator
{
public:
    virtual ~Aggregator() {}
    virtual void input(ByteBuffer &value) = 0;
    virtual void input(ViewByteBuffer &value) = 0;
    virtual void input(std::unique_ptr<Value> &value) = 0;
    virtual std::unique_ptr<Value> output() = 0;
    virtual void reset();
    virtual Type::type inputType() = 0;
    virtual Type::type outputType() = 0;
};

template<typename T, typename U = T>
class None: public Aggregator
{
private:
    TypedValue<U> _val;
    Type::type _input_type = T::type_num;
    Type::type _output_type = U::type_num;
public:
    None()
    {
        uint64_t val = 0;
        ByteBuffer value(sizeof(uint64_t), reinterpret_cast<char*>(&val));
        _val = TypedValue<U>(value);
    }
    void input(ByteBuffer &value) override
    {
        _val = TypedValue<U>(value);
    }
    void input(ViewByteBuffer &value) override
    {
        _val = TypedValue<U>(ByteBuffer(value));
    }
    void input(std::unique_ptr<Value> &value) override
    {
        _val = TypedValue<U>(value->getBuffer());
    }
    std::unique_ptr<Value> output() override
    {
        return std::make_unique<TypedValue<U>>(_val);
    }
    void reset() override {}
    Type::type inputType() override { return _input_type; }
    Type::type outputType() override { return _output_type; }
};

template<typename T, typename U = UInt64Type>
class DistinctCounter: public Aggregator
{
private:
    TypedValue<U> _val;
    std::set<ByteBuffer> _set;
    Type::type _input_type = T::type_num;
    Type::type _output_type = U::type_num;
public:
    DistinctCounter()
    {
        uint64_t val = 0;
        ByteBuffer value(sizeof(uint64_t), reinterpret_cast<char*>(&val));
        _val = TypedValue<U>(value);
    }
    void input(ByteBuffer &value) override
    {
        _set.insert(value);
    }
    void input(ViewByteBuffer &value) override
    {
        _set.insert(ByteBuffer(value));
    }
    void input(std::unique_ptr<Value> &value) override
    {
        _set.insert(value->getBuffer());
    }
    std::unique_ptr<Value> output() override
    {
        uint64_t size = _set.size();
        ByteBuffer value(sizeof(uint64_t), reinterpret_cast<char*>(&size));
        _val = TypedValue<U>(value);
        return std::make_unique<TypedValue<U>>(value);
    }
    void reset()
    {
        _val.~Value();
        uint64_t val = 0;
        ByteBuffer value(sizeof(uint64_t), reinterpret_cast<char*>(&val));
        _val = TypedValue<U>(value);
        _set.clear();
    }
    Type::type inputType() override { return _input_type; }
    Type::type outputType() override { return _output_type; }
};

#endif // AGGREGATORS_H
