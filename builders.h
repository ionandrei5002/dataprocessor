#ifndef BUILDERS_H
#define BUILDERS_H

#include "types.h"
#include "column.h"
#include "schema.h"
#include "operators.h"
#include "comparator.h"
#include "aggregators.h"

static std::unique_ptr<Column> ColumnBuilder(Node node)
{
    Type::type _type = node.getType();
    Encoding::type _encoding = node.getEncoding();
    bool _nullable = node.isNullable();

    if (_nullable == false)
    {
        if (_encoding == Encoding::PLAIN)
        {
            switch (_type) {
            case Type::INT8:
                return std::make_unique<TypedColumn<PlainStore, Int8Type>>();
                break;
            case Type::INT16:
                return std::make_unique<TypedColumn<PlainStore, Int16Type>>();
                break;
            case Type::INT32:
                return std::make_unique<TypedColumn<PlainStore, Int32Type>>();
                break;
            case Type::INT64:
                return std::make_unique<TypedColumn<PlainStore, Int64Type>>();
                break;
            case Type::FLOAT:
                return std::make_unique<TypedColumn<PlainStore, FloatType>>();
                break;
            case Type::DOUBLE:
                return std::make_unique<TypedColumn<PlainStore, DoubleType>>();
                break;
            case Type::STRING:
                return std::make_unique<TypedColumn<PlainStore, StringType>>();
                break;
            default:
                return nullptr;
                break;
            }
        }
        if (_encoding == Encoding::DICTIONARY)
        {
            switch (_type) {
            case Type::INT8:
                return std::make_unique<TypedColumn<DictStore, Int8Type>>();
                break;
            case Type::INT16:
                return std::make_unique<TypedColumn<DictStore, Int16Type>>();
                break;
            case Type::INT32:
                return std::make_unique<TypedColumn<DictStore, Int32Type>>();
                break;
            case Type::INT64:
                return std::make_unique<TypedColumn<DictStore, Int64Type>>();
                break;
            case Type::FLOAT:
                return std::make_unique<TypedColumn<DictStore, FloatType>>();
                break;
            case Type::DOUBLE:
                return std::make_unique<TypedColumn<DictStore, DoubleType>>();
                break;
            case Type::STRING:
                return std::make_unique<TypedColumn<DictStore, StringType>>();
                break;
            default:
                return nullptr;
                break;
            }
        }
    }
    if (_nullable == true)
    {
        if (_encoding == Encoding::PLAIN)
        {
            switch (_type) {
            case Type::INT8:
                return std::make_unique<NullableTypedColumn<PlainStore, Int8Type>>();
                break;
            case Type::INT16:
                return std::make_unique<NullableTypedColumn<PlainStore, Int16Type>>();
                break;
            case Type::INT32:
                return std::make_unique<NullableTypedColumn<PlainStore, Int32Type>>();
                break;
            case Type::INT64:
                return std::make_unique<NullableTypedColumn<PlainStore, Int64Type>>();
                break;
            case Type::FLOAT:
                return std::make_unique<NullableTypedColumn<PlainStore, FloatType>>();
                break;
            case Type::DOUBLE:
                return std::make_unique<NullableTypedColumn<PlainStore, DoubleType>>();
                break;
            case Type::STRING:
                return std::make_unique<NullableTypedColumn<PlainStore, StringType>>();
                break;
            default:
                return nullptr;
                break;
            }
        }
        if (_encoding == Encoding::DICTIONARY)
        {
            switch (_type) {
            case Type::INT8:
                return std::make_unique<NullableTypedColumn<DictStore, Int8Type>>();
                break;
            case Type::INT16:
                return std::make_unique<NullableTypedColumn<DictStore, Int16Type>>();
                break;
            case Type::INT32:
                return std::make_unique<NullableTypedColumn<DictStore, Int32Type>>();
                break;
            case Type::INT64:
                return std::make_unique<NullableTypedColumn<DictStore, Int64Type>>();
                break;
            case Type::FLOAT:
                return std::make_unique<NullableTypedColumn<DictStore, FloatType>>();
                break;
            case Type::DOUBLE:
                return std::make_unique<NullableTypedColumn<DictStore, DoubleType>>();
                break;
            case Type::STRING:
                return std::make_unique<NullableTypedColumn<DictStore, StringType>>();
                break;
            default:
                return nullptr;
                break;
            }
        }
    }
}

static std::unique_ptr<UnaryOperator> String2TypeBuilder(Node node)
{
    Type::type _type = node.getType();
    bool _nullable = node.isNullable();

    if (_nullable == false)
    {
        switch (_type) {
        case Type::INT8:
            return std::make_unique<FromStringCast<Int8Type>>();
            break;
        case Type::INT16:
            return std::make_unique<FromStringCast<Int16Type>>();
            break;
        case Type::INT32:
            return std::make_unique<FromStringCast<Int32Type>>();
            break;
        case Type::INT64:
            return std::make_unique<FromStringCast<Int64Type>>();
            break;
        case Type::FLOAT:
            return std::make_unique<FromStringCast<FloatType>>();
            break;
        case Type::DOUBLE:
            return std::make_unique<FromStringCast<DoubleType>>();
            break;
        case Type::STRING:
            return std::make_unique<FromStringCast<StringType>>();
            break;
        default:
            return nullptr;
            break;
        }
    }

    if (_nullable == true)
    {
        switch (_type) {
        case Type::INT8:
            return std::make_unique<NullFromStringCast<Int8Type>>();
            break;
        case Type::INT16:
            return std::make_unique<NullFromStringCast<Int16Type>>();
            break;
        case Type::INT32:
            return std::make_unique<NullFromStringCast<Int32Type>>();
            break;
        case Type::INT64:
            return std::make_unique<NullFromStringCast<Int64Type>>();
            break;
        case Type::FLOAT:
            return std::make_unique<NullFromStringCast<FloatType>>();
            break;
        case Type::DOUBLE:
            return std::make_unique<NullFromStringCast<DoubleType>>();
            break;
        case Type::STRING:
            return std::make_unique<NullFromStringCast<StringType>>();
            break;
        default:
            return nullptr;
            break;
        }
    }
}

static std::unique_ptr<UnaryOperator> Type2StringBuilder(Node node)
{
    Type::type _type = node.getType();
    bool _nullable = node.isNullable();

    if (_nullable == false)
    {
        switch (_type) {
        case Type::INT8:
            return std::make_unique<ToStringCast<Int8Type>>();
            break;
        case Type::INT16:
            return std::make_unique<ToStringCast<Int16Type>>();
            break;
        case Type::INT32:
            return std::make_unique<ToStringCast<Int32Type>>();
            break;
        case Type::INT64:
            return std::make_unique<ToStringCast<Int64Type>>();
            break;
        case Type::FLOAT:
            return std::make_unique<ToStringCast<FloatType>>();
            break;
        case Type::DOUBLE:
            return std::make_unique<ToStringCast<DoubleType>>();
            break;
        case Type::STRING:
            return std::make_unique<ToStringCast<StringType>>();
            break;
        default:
            return nullptr;
            break;
        }
    }

    if (_nullable == true)
    {
        switch (_type) {
        case Type::INT8:
            return std::make_unique<ToStringCast<Int8Type>>();
            break;
        case Type::INT16:
            return std::make_unique<ToStringCast<Int16Type>>();
            break;
        case Type::INT32:
            return std::make_unique<ToStringCast<Int32Type>>();
            break;
        case Type::INT64:
            return std::make_unique<ToStringCast<Int64Type>>();
            break;
        case Type::FLOAT:
            return std::make_unique<ToStringCast<FloatType>>();
            break;
        case Type::DOUBLE:
            return std::make_unique<ToStringCast<DoubleType>>();
            break;
        case Type::STRING:
            return std::make_unique<ToStringCast<StringType>>();
            break;
        default:
            return nullptr;
            break;
        }
    }
}

static std::unique_ptr<Comparator> ComparatorBuilder(Column* column, Node node)
{
    Type::type _type = node.getType();
    bool _nullable = node.isNullable();

    if (_nullable == false)
    {
        switch (_type) {
        case Type::INT8:
            return std::make_unique<TypedComparator<Int8Type>>(column);
            break;
        case Type::INT16:
            return std::make_unique<TypedComparator<Int16Type>>(column);
            break;
        case Type::INT32:
            return std::make_unique<TypedComparator<Int32Type>>(column);
            break;
        case Type::INT64:
            return std::make_unique<TypedComparator<Int64Type>>(column);
            break;
        case Type::FLOAT:
            return std::make_unique<TypedComparator<FloatType>>(column);
            break;
        case Type::DOUBLE:
            return std::make_unique<TypedComparator<DoubleType>>(column);
            break;
        case Type::STRING:
            return std::make_unique<TypedComparator<StringType>>(column);
            break;
        default:
            return nullptr;
            break;
        }
    }
    if (_nullable == true)
    {
        switch (_type) {
        case Type::INT8:
            return std::make_unique<NullableTypedComparator<Int8Type>>(column);
            break;
        case Type::INT16:
            return std::make_unique<NullableTypedComparator<Int16Type>>(column);
            break;
        case Type::INT32:
            return std::make_unique<NullableTypedComparator<Int32Type>>(column);
            break;
        case Type::INT64:
            return std::make_unique<NullableTypedComparator<Int64Type>>(column);
            break;
        case Type::FLOAT:
            return std::make_unique<NullableTypedComparator<FloatType>>(column);
            break;
        case Type::DOUBLE:
            return std::make_unique<NullableTypedComparator<DoubleType>>(column);
            break;
        case Type::STRING:
            return std::make_unique<NullableTypedComparator<StringType>>(column);
            break;
        default:
            return nullptr;
            break;
        }
    }
}

static std::shared_ptr<Aggregator> AggregatorBuilder(Node node, AggType::type aggregate)
{
    Type::type _type = node.getType();
    switch (aggregate) {
    case AggType::NONE:
        switch (_type) {
        case Type::INT8:
            return std::make_shared<None<Int8Type>>();
            break;
        case Type::INT16:
            return std::make_shared<None<Int16Type>>();
            break;
        case Type::INT32:
            return std::make_shared<None<Int32Type>>();
            break;
        case Type::INT64:
            return std::make_shared<None<Int64Type>>();
            break;
        case Type::FLOAT:
            return std::make_shared<None<FloatType>>();
            break;
        case Type::DOUBLE:
            return std::make_shared<None<DoubleType>>();
            break;
        case Type::STRING:
            return std::make_shared<None<StringType>>();
            break;
        default:
            return nullptr;
            break;
        }
        break;
    case AggType::DISTINCTC_COUNT:
        switch (_type) {
        case Type::INT8:
            return std::make_shared<DistinctCounter<Int8Type>>();
            break;
        case Type::INT16:
            return std::make_shared<DistinctCounter<Int16Type>>();
            break;
        case Type::INT32:
            return std::make_shared<DistinctCounter<Int32Type>>();
            break;
        case Type::INT64:
            return std::make_shared<DistinctCounter<Int64Type>>();
            break;
        case Type::FLOAT:
            return std::make_shared<DistinctCounter<FloatType>>();
            break;
        case Type::DOUBLE:
            return std::make_shared<DistinctCounter<DoubleType>>();
            break;
        case Type::STRING:
            return std::make_shared<DistinctCounter<StringType>>();
            break;
        default:
            return nullptr;
            break;
        }
        break;
    default:
        return nullptr;
        break;
    }
}

#endif // BUILDERS_H
