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
    virtual std::unique_ptr<Value> get() = 0;
    virtual void set(std::unique_ptr<Value>& value) = 0;
    virtual ByteBuffer& getBuffer() = 0;
    virtual Type::type getType() = 0;
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
    TypedValue() {}
    explicit TypedValue(ByteBuffer value):_buffer(std::move(value)) {}
    std::unique_ptr<Value> get() override
    {
        return std::make_unique<TypedValue<T>>(_buffer);
    }
    void set(std::unique_ptr<Value>& value) override
    {
        this->_buffer = value->getBuffer();
    }
    ByteBuffer& getBuffer() override
    {
        return this->_buffer;
    }
    Type::type getType() override
    {
        return T::type_num;
    }
    bool operator ==(TypedValue<T>& value)
    {
        typedef typename T::c_type _val;
        return *reinterpret_cast<_val*>(this->_buffer._data) == *reinterpret_cast<_val*>(value.getBuffer()._data);
    }
    bool operator !=(TypedValue<T>& value)
    {
        typedef typename T::c_type _val;
        return *reinterpret_cast<_val*>(this->_buffer._data) != *reinterpret_cast<_val*>(value.getBuffer()._data);
    }
    bool operator <(TypedValue<T>& value)
    {
        typedef typename T::c_type _val;
        return *reinterpret_cast<_val*>(this->_buffer._data) < *reinterpret_cast<_val*>(value.getBuffer()._data);
    }
    bool operator >(TypedValue<T>& value)
    {
        typedef typename T::c_type _val;
        return *reinterpret_cast<_val*>(this->_buffer._data) > *reinterpret_cast<_val*>(value.getBuffer()._data);
    }
    friend bool operator<(const TypedValue<T>& lv, const TypedValue<T>& rv)
    {
        return lv < rv;
    }
};

template<typename T>
class NullableTypedValue: public Value, public isNull
{
private:
    ByteBuffer _buffer;
public:
    NullableTypedValue() {}
    explicit NullableTypedValue(ByteBuffer value):_buffer(std::move(value)) {}
    std::unique_ptr<Value> get() override
    {
        return std::make_unique<NullableTypedValue<T>>(_buffer);
    }
    void set(std::unique_ptr<Value>& value) override
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
    bool operator ==(NullableTypedValue<T>& value)
    {
        typedef typename T::c_type _val;
        return *reinterpret_cast<_val*>(this->_buffer._data) == *reinterpret_cast<_val*>(value.getBuffer()._data);
    }
    bool operator !=(NullableTypedValue<T>& value)
    {
        typedef typename T::c_type _val;
        return *reinterpret_cast<_val*>(this->_buffer._data) != *reinterpret_cast<_val*>(value.getBuffer()._data);
    }
    bool operator <(NullableTypedValue<T>& value)
    {
        typedef typename T::c_type _val;
        return *reinterpret_cast<_val*>(this->_buffer._data) < *reinterpret_cast<_val*>(value.getBuffer()._data);
    }
    bool operator >(NullableTypedValue<T>& value)
    {
        typedef typename T::c_type _val;
        return *reinterpret_cast<_val*>(this->_buffer._data) > *reinterpret_cast<_val*>(value.getBuffer()._data);
    }
    friend bool operator<(const NullableTypedValue<T>& lv, const NullableTypedValue<T>& rv)
    {
        return lv < rv;
    }
};

template<>
class TypedValue<StringType>: public Value
{
private:
    ByteBuffer _buffer;
public:
    TypedValue() {}
    explicit TypedValue(ByteBuffer value):_buffer(std::move(value)) {}
    std::unique_ptr<Value> get() override
    {
        return std::make_unique<TypedValue<StringType>>(_buffer);
    }
    void set(std::unique_ptr<Value>& value) override
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
    bool operator ==(TypedValue<StringType>& value)
    {
        return this->_buffer == value.getBuffer();
    }
    bool operator !=(TypedValue<StringType>& value)
    {
        return this->_buffer != value.getBuffer();
    }
    bool operator <(TypedValue<StringType>& value)
    {
        return this->_buffer < value.getBuffer();
    }
    bool operator >(TypedValue<StringType>& value)
    {
        return this->_buffer > value.getBuffer();
    }
    friend bool operator<(const TypedValue<StringType>& lv, const TypedValue<StringType>& rv)
    {
        return lv < rv;
    }
};

template<>
class NullableTypedValue<StringType>: public Value, public isNull
{
private:
    ByteBuffer _buffer;
public:
    NullableTypedValue() {}
    explicit NullableTypedValue(ByteBuffer value):_buffer(std::move(value)) {}
    std::unique_ptr<Value> get() override
    {
        return std::make_unique<NullableTypedValue<StringType>>(_buffer);
    }
    void set(std::unique_ptr<Value>& value) override
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
    bool operator ==(NullableTypedValue<StringType>& value)
    {
        return this->_buffer == value.getBuffer();
    }
    bool operator !=(NullableTypedValue<StringType>& value)
    {
        return this->_buffer != value.getBuffer();
    }
    bool operator <(NullableTypedValue<StringType>& value)
    {
        return this->_buffer < value.getBuffer();
    }
    bool operator >(NullableTypedValue<StringType>& value)
    {
        return this->_buffer > value.getBuffer();
    }
    friend bool operator<(const NullableTypedValue<StringType>& lv, const NullableTypedValue<StringType>& rv)
    {
        return lv < rv;
    }
};

#endif // VALUE_H
