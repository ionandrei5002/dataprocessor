#ifndef OPERATORS_H
#define OPERATORS_H

#include <string>
#include <boost/spirit/include/qi_parse.hpp>
#include <boost/spirit/include/qi_numeric.hpp>

#include "types.h"
#include "bytebuffer.h"
#include "value.h"

class UnaryOperator
{
public:
    virtual ~UnaryOperator() {}
    virtual std::shared_ptr<Value> operation(ByteBuffer &value) = 0;
};

template<typename T>
class FromStringCast: public UnaryOperator
{
public:
    std::shared_ptr<Value> operation(ByteBuffer &value) override
    {
        throw;
    }
};

template<>
class FromStringCast<Int8Type>: public UnaryOperator
{
public:
    std::shared_ptr<Value> operation(ByteBuffer &value) override
    {
        std::string str(value._data, value._size);
        int8_t cast_value = static_cast<int8_t>(std::stoi(str));

        return std::make_shared<TypedValue<Int8Type>>(std::move(ByteBuffer(sizeof(int8_t), reinterpret_cast<char*>(&cast_value))));
    }
};

template<>
class FromStringCast<Int16Type>: public UnaryOperator
{
public:
    std::shared_ptr<Value> operation(ByteBuffer &value) override
    {
        std::string str(value._data, value._size);
        int16_t cast_value = static_cast<int16_t>(std::stoi(str));

        return std::make_shared<TypedValue<Int16Type>>(std::move(ByteBuffer(sizeof(int16_t), reinterpret_cast<char*>(&cast_value))));
    }
};

template<>
class FromStringCast<Int32Type>: public UnaryOperator
{
public:
    std::shared_ptr<Value> operation(ByteBuffer &value) override
    {
        std::string str(value._data, value._size);
        int32_t cast_value = std::stoi(str);

        return std::make_shared<TypedValue<Int32Type>>(std::move(ByteBuffer(sizeof(int32_t), reinterpret_cast<char*>(&cast_value))));
    }
};

template<>
class FromStringCast<Int64Type>: public UnaryOperator
{
public:
    std::shared_ptr<Value> operation(ByteBuffer &value) override
    {
        std::experimental::string_view str(value._data, value._size);
        int64_t cast_value = 0;
        boost::spirit::qi::parse(str.begin(), str.end(), boost::spirit::qi::long_, cast_value);

        return std::make_shared<TypedValue<Int64Type>>(std::move(ByteBuffer(sizeof(int64_t), reinterpret_cast<char*>(&cast_value))));
    }
};

template<>
class FromStringCast<FloatType>: public UnaryOperator
{
public:
    std::shared_ptr<Value> operation(ByteBuffer &value) override
    {
        std::string str(value._data, value._size);
        float cast_value = std::stof(str);

        return std::make_shared<TypedValue<FloatType>>(std::move(ByteBuffer(sizeof(float), reinterpret_cast<char*>(&cast_value))));
    }
};

template<>
class FromStringCast<DoubleType>: public UnaryOperator
{
public:
    std::shared_ptr<Value> operation(ByteBuffer &value) override
    {
        std::string str(value._data, value._size);
        double cast_value = std::stod(str);

        return std::make_shared<TypedValue<DoubleType>>(std::move(ByteBuffer(sizeof(double), reinterpret_cast<char*>(&cast_value))));
    }
};

template<>
class FromStringCast<StringType>: public UnaryOperator
{
public:
    std::shared_ptr<Value> operation(ByteBuffer &value) override
    {
        return std::make_shared<TypedValue<StringType>>(std::move(ByteBuffer(value._size, value._data)));
    }
};

template<typename T>
class NullFromStringCast: public UnaryOperator
{
public:
    std::shared_ptr<Value> operation(ByteBuffer &value) override
    {
        throw;
    }
};

template<>
class NullFromStringCast<Int8Type>: public UnaryOperator
{
public:
    std::shared_ptr<Value> operation(ByteBuffer &value) override
    {
        std::string str(value._data, value._size);
        int8_t cast_value = static_cast<int8_t>(std::stoi(str));

        return std::make_shared<NullableTypedValue<Int8Type>>(std::move(ByteBuffer(sizeof(int8_t), reinterpret_cast<char*>(&cast_value))));
    }
};

template<>
class NullFromStringCast<Int16Type>: public UnaryOperator
{
public:
    std::shared_ptr<Value> operation(ByteBuffer &value) override
    {
        std::string str(value._data, value._size);
        int16_t cast_value = static_cast<int16_t>(std::stoi(str));

        return std::make_shared<NullableTypedValue<Int16Type>>(std::move(ByteBuffer(sizeof(int16_t), reinterpret_cast<char*>(&cast_value))));
    }
};

template<>
class NullFromStringCast<Int32Type>: public UnaryOperator
{
public:
    std::shared_ptr<Value> operation(ByteBuffer &value) override
    {
        std::string str(value._data, value._size);
        int32_t cast_value = std::stoi(str);

        return std::make_shared<NullableTypedValue<Int32Type>>(std::move(ByteBuffer(sizeof(int32_t), reinterpret_cast<char*>(&cast_value))));
    }
};

template<>
class NullFromStringCast<Int64Type>: public UnaryOperator
{
public:
    std::shared_ptr<Value> operation(ByteBuffer &value) override
    {
        std::experimental::string_view str(value._data, value._size);
        int64_t cast_value = 0;
        boost::spirit::qi::parse(str.begin(), str.end(), boost::spirit::qi::long_, cast_value);

        return std::make_shared<NullableTypedValue<Int64Type>>(std::move(ByteBuffer(sizeof(int64_t), reinterpret_cast<char*>(&cast_value))));
    }
};

template<>
class NullFromStringCast<FloatType>: public UnaryOperator
{
public:
    std::shared_ptr<Value> operation(ByteBuffer &value) override
    {
        std::string str(value._data, value._size);
        float cast_value = std::stof(str);

        return std::make_shared<NullableTypedValue<FloatType>>(std::move(ByteBuffer(sizeof(float), reinterpret_cast<char*>(&cast_value))));
    }
};

template<>
class NullFromStringCast<DoubleType>: public UnaryOperator
{
public:
    std::shared_ptr<Value> operation(ByteBuffer &value) override
    {
        std::string str(value._data, value._size);
        double cast_value = std::stod(str);

        return std::make_shared<NullableTypedValue<DoubleType>>(std::move(ByteBuffer(sizeof(double), reinterpret_cast<char*>(&cast_value))));
    }
};

template<>
class NullFromStringCast<StringType>: public UnaryOperator
{
public:
    std::shared_ptr<Value> operation(ByteBuffer &value) override
    {
        return std::make_shared<NullableTypedValue<StringType>>(std::move(ByteBuffer(value._size, value._data)));
    }
};

#endif // OPERATORS_H
