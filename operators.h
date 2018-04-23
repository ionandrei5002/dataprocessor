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
    virtual std::unique_ptr<Value> operation(ByteBuffer &value);
    virtual std::unique_ptr<Value> operation(ViewByteBuffer &value);
};

template<typename T>
class FromStringCast: public UnaryOperator
{
public:
    std::unique_ptr<Value> operation(ByteBuffer &value) override
    {
        throw;
    }
    std::unique_ptr<Value> operation(ViewByteBuffer &value) override
    {
        throw;
    }
};

template<>
class FromStringCast<Int8Type>: public UnaryOperator
{
public:
    std::unique_ptr<Value> operation(ByteBuffer &value) override
    {
        std::string str(value._data, value._size);
        int8_t cast_value = static_cast<int8_t>(std::stoi(str));

        return std::make_unique<TypedValue<Int8Type>>(ByteBuffer(sizeof(int8_t), reinterpret_cast<char*>(&cast_value)));
    }
    std::unique_ptr<Value> operation(ViewByteBuffer &value) override
    {
        std::string str(value._data, value._size);
        int8_t cast_value = static_cast<int8_t>(std::stoi(str));

        return std::make_unique<TypedValue<Int8Type>>(ByteBuffer(sizeof(int8_t), reinterpret_cast<char*>(&cast_value)));
    }
};

template<>
class FromStringCast<Int16Type>: public UnaryOperator
{
public:
    std::unique_ptr<Value> operation(ByteBuffer &value) override
    {
        std::string str(value._data, value._size);
        int16_t cast_value = static_cast<int16_t>(std::stoi(str));

        return std::make_unique<TypedValue<Int16Type>>(ByteBuffer(sizeof(int16_t), reinterpret_cast<char*>(&cast_value)));
    }
    std::unique_ptr<Value> operation(ViewByteBuffer &value) override
    {
        std::string str(value._data, value._size);
        int16_t cast_value = static_cast<int16_t>(std::stoi(str));

        return std::make_unique<TypedValue<Int16Type>>(ByteBuffer(sizeof(int16_t), reinterpret_cast<char*>(&cast_value)));
    }
};

template<>
class FromStringCast<Int32Type>: public UnaryOperator
{
public:
    std::unique_ptr<Value> operation(ByteBuffer &value) override
    {
        std::string str(value._data, value._size);
        int32_t cast_value = std::stoi(str);

        return std::make_unique<TypedValue<Int32Type>>(ByteBuffer(sizeof(int32_t), reinterpret_cast<char*>(&cast_value)));
    }
    std::unique_ptr<Value> operation(ViewByteBuffer &value) override
    {
        std::string str(value._data, value._size);
        int32_t cast_value = std::stoi(str);

        return std::make_unique<TypedValue<Int32Type>>(ByteBuffer(sizeof(int32_t), reinterpret_cast<char*>(&cast_value)));
    }
};

template<>
class FromStringCast<Int64Type>: public UnaryOperator
{
public:
    std::unique_ptr<Value> operation(ByteBuffer &value) override
    {
        std::experimental::string_view str(value._data, value._size);
        int64_t cast_value = 0;
        boost::spirit::qi::parse(str.begin(), str.end(), boost::spirit::qi::long_, cast_value);

        return std::make_unique<TypedValue<Int64Type>>(ByteBuffer(sizeof(int64_t), reinterpret_cast<char*>(&cast_value)));
    }
    std::unique_ptr<Value> operation(ViewByteBuffer &value) override
    {
        std::experimental::string_view str(value._data, value._size);
        int64_t cast_value = 0;
        boost::spirit::qi::parse(str.begin(), str.end(), boost::spirit::qi::long_, cast_value);

        return std::make_unique<TypedValue<Int64Type>>(ByteBuffer(sizeof(int64_t), reinterpret_cast<char*>(&cast_value)));
    }
};

template<>
class FromStringCast<FloatType>: public UnaryOperator
{
public:
    std::unique_ptr<Value> operation(ByteBuffer &value) override
    {
        std::string str(value._data, value._size);
        float cast_value = std::stof(str);

        return std::make_unique<TypedValue<FloatType>>(ByteBuffer(sizeof(float), reinterpret_cast<char*>(&cast_value)));
    }
    std::unique_ptr<Value> operation(ViewByteBuffer &value) override
    {
        std::string str(value._data, value._size);
        float cast_value = std::stof(str);

        return std::make_unique<TypedValue<FloatType>>(ByteBuffer(sizeof(float), reinterpret_cast<char*>(&cast_value)));
    }
};

template<>
class FromStringCast<DoubleType>: public UnaryOperator
{
public:
    std::unique_ptr<Value> operation(ByteBuffer &value) override
    {
        std::string str(value._data, value._size);
        double cast_value = std::stod(str);

        return std::make_unique<TypedValue<DoubleType>>(ByteBuffer(sizeof(double), reinterpret_cast<char*>(&cast_value)));
    }
    std::unique_ptr<Value> operation(ViewByteBuffer &value) override
    {
        std::string str(value._data, value._size);
        double cast_value = std::stod(str);

        return std::make_unique<TypedValue<DoubleType>>(ByteBuffer(sizeof(double), reinterpret_cast<char*>(&cast_value)));
    }
};

template<>
class FromStringCast<StringType>: public UnaryOperator
{
public:
    std::unique_ptr<Value> operation(ByteBuffer &value) override
    {
        return std::make_unique<TypedValue<StringType>>(ByteBuffer(value._size, value._data));
    }
    std::unique_ptr<Value> operation(ViewByteBuffer &value) override
    {
        return std::make_unique<TypedValue<StringType>>(ByteBuffer(value._size, value._data));
    }
};

template<typename T>
class NullFromStringCast: public UnaryOperator
{
public:
    std::unique_ptr<Value> operation(ByteBuffer &value) override
    {
        throw;
    }
    std::unique_ptr<Value> operation(ViewByteBuffer &value) override
    {
        throw;
    }
};

template<>
class NullFromStringCast<Int8Type>: public UnaryOperator
{
public:
    std::unique_ptr<Value> operation(ByteBuffer &value) override
    {
        std::string str(value._data, value._size);
        int8_t cast_value = static_cast<int8_t>(std::stoi(str));

        return std::make_unique<NullableTypedValue<Int8Type>>(ByteBuffer(sizeof(int8_t), reinterpret_cast<char*>(&cast_value)));
    }
    std::unique_ptr<Value> operation(ViewByteBuffer &value) override
    {
        std::string str(value._data, value._size);
        int8_t cast_value = static_cast<int8_t>(std::stoi(str));

        return std::make_unique<NullableTypedValue<Int8Type>>(ByteBuffer(sizeof(int8_t), reinterpret_cast<char*>(&cast_value)));
    }
};

template<>
class NullFromStringCast<Int16Type>: public UnaryOperator
{
public:
    std::unique_ptr<Value> operation(ByteBuffer &value) override
    {
        std::string str(value._data, value._size);
        int16_t cast_value = static_cast<int16_t>(std::stoi(str));

        return std::make_unique<NullableTypedValue<Int16Type>>(ByteBuffer(sizeof(int16_t), reinterpret_cast<char*>(&cast_value)));
    }
    std::unique_ptr<Value> operation(ViewByteBuffer &value) override
    {
        std::string str(value._data, value._size);
        int16_t cast_value = static_cast<int16_t>(std::stoi(str));

        return std::make_unique<NullableTypedValue<Int16Type>>(ByteBuffer(sizeof(int16_t), reinterpret_cast<char*>(&cast_value)));
    }
};

template<>
class NullFromStringCast<Int32Type>: public UnaryOperator
{
public:
    std::unique_ptr<Value> operation(ByteBuffer &value) override
    {
        std::string str(value._data, value._size);
        int32_t cast_value = std::stoi(str);

        return std::make_unique<NullableTypedValue<Int32Type>>(ByteBuffer(sizeof(int32_t), reinterpret_cast<char*>(&cast_value)));
    }
    std::unique_ptr<Value> operation(ViewByteBuffer &value) override
    {
        std::string str(value._data, value._size);
        int32_t cast_value = std::stoi(str);

        return std::make_unique<NullableTypedValue<Int32Type>>(ByteBuffer(sizeof(int32_t), reinterpret_cast<char*>(&cast_value)));
    }
};

template<>
class NullFromStringCast<Int64Type>: public UnaryOperator
{
public:
    std::unique_ptr<Value> operation(ByteBuffer &value) override
    {
        std::experimental::string_view str(value._data, value._size);
        int64_t cast_value = 0;
        boost::spirit::qi::parse(str.begin(), str.end(), boost::spirit::qi::long_, cast_value);

        return std::make_unique<NullableTypedValue<Int64Type>>(ByteBuffer(sizeof(int64_t), reinterpret_cast<char*>(&cast_value)));
    }
    std::unique_ptr<Value> operation(ViewByteBuffer &value) override
    {
        std::experimental::string_view str(value._data, value._size);
        int64_t cast_value = 0;
        boost::spirit::qi::parse(str.begin(), str.end(), boost::spirit::qi::long_, cast_value);

        return std::make_unique<NullableTypedValue<Int64Type>>(ByteBuffer(sizeof(int64_t), reinterpret_cast<char*>(&cast_value)));
    }
};

template<>
class NullFromStringCast<FloatType>: public UnaryOperator
{
public:
    std::unique_ptr<Value> operation(ByteBuffer &value) override
    {
        std::string str(value._data, value._size);
        float cast_value = std::stof(str);

        return std::make_unique<NullableTypedValue<FloatType>>(ByteBuffer(sizeof(float), reinterpret_cast<char*>(&cast_value)));
    }
    std::unique_ptr<Value> operation(ViewByteBuffer &value) override
    {
        std::string str(value._data, value._size);
        float cast_value = std::stof(str);

        return std::make_unique<NullableTypedValue<FloatType>>(ByteBuffer(sizeof(float), reinterpret_cast<char*>(&cast_value)));
    }
};

template<>
class NullFromStringCast<DoubleType>: public UnaryOperator
{
public:
    std::unique_ptr<Value> operation(ByteBuffer &value) override
    {
        std::string str(value._data, value._size);
        double cast_value = std::stod(str);

        return std::make_unique<NullableTypedValue<DoubleType>>(ByteBuffer(sizeof(double), reinterpret_cast<char*>(&cast_value)));
    }
    std::unique_ptr<Value> operation(ViewByteBuffer &value) override
    {
        std::string str(value._data, value._size);
        double cast_value = std::stod(str);

        return std::make_unique<NullableTypedValue<DoubleType>>(ByteBuffer(sizeof(double), reinterpret_cast<char*>(&cast_value)));
    }
};

template<>
class NullFromStringCast<StringType>: public UnaryOperator
{
public:
    std::unique_ptr<Value> operation(ByteBuffer &value) override
    {
        return std::make_unique<NullableTypedValue<StringType>>(ByteBuffer(value._size, value._data));
    }
    std::unique_ptr<Value> operation(ViewByteBuffer &value) override
    {
        return std::make_unique<NullableTypedValue<StringType>>(ByteBuffer(value._size, value._data));
    }
};

template<typename T>
class ToStringCast: public UnaryOperator
{
public:
    std::unique_ptr<Value> operation(ByteBuffer &value) override
    {
        throw;
    }
    std::unique_ptr<Value> operation(ViewByteBuffer &value) override
    {
        throw;
    }
};

template<>
class ToStringCast<Int8Type>: public UnaryOperator
{
public:
    std::unique_ptr<Value> operation(ByteBuffer &value) override
    {
        int8_t cast_value = *reinterpret_cast<int8_t*>(value._data);
        std::string str = std::to_string(cast_value);

        return std::make_unique<TypedValue<Int8Type>>(ByteBuffer(str.size(), str.data()));
    }
    std::unique_ptr<Value> operation(ViewByteBuffer &value) override
    {
        int8_t cast_value = *reinterpret_cast<int8_t*>(value._data);
        std::string str = std::to_string(cast_value);

        return std::make_unique<TypedValue<Int8Type>>(ByteBuffer(str.size(), str.data()));
    }
};

template<>
class ToStringCast<Int16Type>: public UnaryOperator
{
public:
    std::unique_ptr<Value> operation(ByteBuffer &value) override
    {
        int16_t cast_value = *reinterpret_cast<int16_t*>(value._data);
        std::string str = std::to_string(cast_value);

        return std::make_unique<TypedValue<Int16Type>>(ByteBuffer(str.size(), str.data()));
    }
    std::unique_ptr<Value> operation(ViewByteBuffer &value) override
    {
        int16_t cast_value = *reinterpret_cast<int16_t*>(value._data);
        std::string str = std::to_string(cast_value);

        return std::make_unique<TypedValue<Int16Type>>(ByteBuffer(str.size(), str.data()));
    }
};

template<>
class ToStringCast<Int32Type>: public UnaryOperator
{
public:
    std::unique_ptr<Value> operation(ByteBuffer &value) override
    {
        int32_t cast_value = *reinterpret_cast<int32_t*>(value._data);
        std::string str = std::to_string(cast_value);

        return std::make_unique<TypedValue<Int32Type>>(ByteBuffer(str.size(), str.data()));
    }
    std::unique_ptr<Value> operation(ViewByteBuffer &value) override
    {
        int32_t cast_value = *reinterpret_cast<int32_t*>(value._data);
        std::string str = std::to_string(cast_value);

        return std::make_unique<TypedValue<Int32Type>>(ByteBuffer(str.size(), str.data()));
    }
};

template<>
class ToStringCast<Int64Type>: public UnaryOperator
{
public:
    std::unique_ptr<Value> operation(ByteBuffer &value) override
    {
        int64_t cast_value = *reinterpret_cast<int64_t*>(value._data);
        std::string str = std::to_string(cast_value);

        return std::make_unique<TypedValue<Int64Type>>(ByteBuffer(str.size(), str.data()));
    }
    std::unique_ptr<Value> operation(ViewByteBuffer &value) override
    {
        int64_t cast_value = *reinterpret_cast<int64_t*>(value._data);
        std::string str = std::to_string(cast_value);

        return std::make_unique<TypedValue<Int64Type>>(ByteBuffer(str.size(), str.data()));
    }
};

template<>
class ToStringCast<FloatType>: public UnaryOperator
{
public:
    std::unique_ptr<Value> operation(ByteBuffer &value) override
    {
        float cast_value = *reinterpret_cast<float*>(value._data);
        std::string str = std::to_string(cast_value);

        return std::make_unique<TypedValue<FloatType>>(ByteBuffer(str.size(), str.data()));
    }
    std::unique_ptr<Value> operation(ViewByteBuffer &value) override
    {
        float cast_value = *reinterpret_cast<float*>(value._data);
        std::string str = std::to_string(cast_value);

        return std::make_unique<TypedValue<FloatType>>(ByteBuffer(str.size(), str.data()));
    }
};

template<>
class ToStringCast<DoubleType>: public UnaryOperator
{
public:
    std::unique_ptr<Value> operation(ByteBuffer &value) override
    {
        double cast_value = *reinterpret_cast<double*>(value._data);
        std::string str = std::to_string(cast_value);

        return std::make_unique<TypedValue<DoubleType>>(ByteBuffer(str.size(), str.data()));
    }
    std::unique_ptr<Value> operation(ViewByteBuffer &value) override
    {
        double cast_value = *reinterpret_cast<double*>(value._data);
        std::string str = std::to_string(cast_value);

        return std::make_unique<TypedValue<DoubleType>>(ByteBuffer(str.size(), str.data()));
    }
};

template<>
class ToStringCast<StringType>: public UnaryOperator
{
public:
    std::unique_ptr<Value> operation(ByteBuffer &value) override
    {
        return std::make_unique<TypedValue<StringType>>(std::move(value));
    }
    std::unique_ptr<Value> operation(ViewByteBuffer &value) override
    {
        return std::make_unique<TypedValue<StringType>>(ByteBuffer(value));
    }
};

#endif // OPERATORS_H
