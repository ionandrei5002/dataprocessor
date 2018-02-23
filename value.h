#ifndef VALUE_H
#define VALUE_H

#include "types.h"
#include "bytebuffer.h"

class Value
{
protected:
    Type::type _type;
public:
    virtual ~Value() {}
    virtual std::shared_ptr<Value> get() = 0;
    virtual void set(std::shared_ptr<Value> value) = 0;
    virtual ByteBuffer& getBuffer() = 0;
    virtual Type::type getType() = 0;
    virtual bool operator <(const std::shared_ptr<Value> value) = 0;
    virtual bool operator >(const std::shared_ptr<Value> value) = 0;
};

class isNull
{
protected:
    bool _null = false;
public:
    virtual ~isNull() {}
    virtual void setNull() = 0;
    virtual bool getNull() = 0;
};

template<typename T>
class TypedValue: public Value
{
private:
    ByteBuffer _buffer;
public:
    explicit TypedValue(ByteBuffer value):_buffer(std::move(value)) {}
    std::shared_ptr<Value> get() override
    {
        return std::make_shared<TypedValue<T>>(_buffer);
    }
    void set(std::shared_ptr<Value> value) override
    {
        this->_buffer = value->getBuffer();
    }
    ByteBuffer& getBuffer()
    {
        return this->_buffer;
    }
    Type::type getType() override
    {
        return T::type_num;
    }
    bool operator <(const std::shared_ptr<Value> value) override
    {
        typedef typename T::c_type _val;
        return *reinterpret_cast<_val*>(this->_buffer._data) < *reinterpret_cast<_val*>(value->getBuffer()._data);
    }
    bool operator >(const std::shared_ptr<Value> value) override
    {
        typedef typename T::c_type _val;
        return *reinterpret_cast<_val*>(this->_buffer._data) > *reinterpret_cast<_val*>(value->getBuffer()._data);
    }
};

template<typename T>
class NullableTypedValue: public Value, public isNull
{
private:
    ByteBuffer _buffer;
public:
    explicit NullableTypedValue(ByteBuffer value):_buffer(std::move(value)) {}
    std::shared_ptr<Value> get() override
    {
        return std::make_shared<TypedValue<T>>(_buffer);
    }
    void set(std::shared_ptr<Value> value) override
    {
        this->_buffer = value->getBuffer();
    }
    ByteBuffer& getBuffer()
    {
        return this->_buffer;
    }
    Type::type getType() override
    {
        return T::type_num;
    }
    void setNull() override { this->_null = true; }
    bool getNull() override { return this->_null; }
    bool operator <(const std::shared_ptr<Value> value) override
    {
        typedef typename T::c_type _val;
        return *reinterpret_cast<_val*>(this->_buffer._data) < *reinterpret_cast<_val*>(value->getBuffer()._data);
    }
    bool operator >(const std::shared_ptr<Value> value) override
    {
        typedef typename T::c_type _val;
        return *reinterpret_cast<_val*>(this->_buffer._data) > *reinterpret_cast<_val*>(value->getBuffer()._data);
    }
};

template<>
class TypedValue<StringType>: public Value
{
private:
    ByteBuffer _buffer;
public:
    explicit TypedValue(ByteBuffer value):_buffer(std::move(value)) {}
    std::shared_ptr<Value> get() override
    {
        return std::make_shared<TypedValue<StringType>>(_buffer);
    }
    void set(std::shared_ptr<Value> value) override
    {
        this->_buffer = value->getBuffer();
    }
    ByteBuffer& getBuffer()
    {
        return this->_buffer;
    }
    Type::type getType() override
    {
        return StringType::type_num;
    }
    bool operator <(const std::shared_ptr<Value> value) override
    {
//        return this->_buffer < value->getBuffer();
    }
    bool operator >(const std::shared_ptr<Value> value) override
    {
//        return this->_buffer > value->getBuffer();
    }
};

template<>
class NullableTypedValue<StringType>: public Value, public isNull
{
private:
    ByteBuffer _buffer;
public:
    explicit NullableTypedValue(ByteBuffer value):_buffer(std::move(value)) {}
    std::shared_ptr<Value> get() override
    {
        return std::make_shared<TypedValue<StringType>>(_buffer);
    }
    void set(std::shared_ptr<Value> value) override
    {
        this->_buffer = value->getBuffer();
    }
    ByteBuffer& getBuffer()
    {
        return this->_buffer;
    }
    Type::type getType() override
    {
        return StringType::type_num;
    }
    void setNull() override { this->_null = true; }
    bool getNull() override { return this->_null; }
    bool operator <(const std::shared_ptr<Value> value) override
    {
//        return this->_buffer < value->getBuffer();
    }
    bool operator >(const std::shared_ptr<Value> value) override
    {
//        return this->_buffer > value->getBuffer();
    }
};

#endif // VALUE_H
