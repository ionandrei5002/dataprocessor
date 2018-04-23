#ifndef COLUMN_H
#define COLUMN_H

#include <vector>
#include <set>
#include <string>

#include "types.h"
#include "bytebuffer.h"
#include "value.h"
#include "array.h"

class Column
{
public:
    virtual ~Column() {}
    virtual void put(std::unique_ptr<Value>& value) = 0;
    virtual std::unique_ptr<Value> get(uint64_t position) = 0;
    virtual ViewByteBuffer getView(uint64_t position) = 0;
    virtual void swap(uint64_t lv, uint64_t rv);
};

class IsNullable
{
protected:
    std::vector<char> _nulls;
public:
    virtual ~IsNullable() {}
    virtual void setNull(bool value);
    virtual bool getNull(uint64_t position);
};

class Storage
{
public:
    virtual ~Storage() {}
    virtual uint64_t put(ByteBuffer& value) = 0;
    virtual ByteBuffer get(uint64_t offset, uint64_t type_size) = 0;
    virtual ViewByteBuffer getView(uint64_t offset, uint64_t type_size) = 0;
    virtual void swap(uint64_t lv, uint64_t rv, uint64_t type_size, char* tmp);
};

template<typename T>
class TypeStore: public Storage {};

template<>
class TypeStore<PlainStore>: public Storage
{
private:
    Array _data;
public:
    uint64_t put(ByteBuffer& value) override
    {
        uint64_t offset = _data.size();

        _data.emplace_back(value._size, value._data);

        return offset;
    }
    ByteBuffer get(uint64_t offset, uint64_t type_size) override
    {
        return ByteBuffer(type_size, _data.get(offset));
    }
    ViewByteBuffer getView(uint64_t offset, uint64_t type_size) override
    {
        ViewByteBuffer value(type_size, _data.get(offset));
        return value;
    }
    void swap(uint64_t lv, uint64_t rv, uint64_t type_size, char* tmp) override
    {
//        std::cout << _data.size() << " " << lv << " " << rv << std::endl;
        memcpy(tmp, _data.get(lv), type_size);

        _data.rewrite(lv, type_size, _data.get(rv));
        _data.rewrite(rv, type_size, tmp);
    }
};

template<>
class TypeStore<DictStore>: public Storage
{
private:
    std::vector<ByteBuffer*> _position;
    std::set<ByteBuffer> _set;
public:
    uint64_t put(ByteBuffer& value) override
    {
        uint64_t offset = _position.size();

        auto find = _set.find(value);
        if (_set.end() == find)
        {
            _set.insert(value);
            find = _set.find(value);
        }
        _position.emplace_back(const_cast<ByteBuffer*>(&*find));

        return offset;
    }
    ByteBuffer get(uint64_t offset, uint64_t type_size) override
    {
        return *_position.at(offset);
    }
    ViewByteBuffer getView(uint64_t offset, uint64_t type_size) override
    {
        return ViewByteBuffer(*_position.at(offset));
    }
    void swap(uint64_t lv, uint64_t rv, uint64_t type_size, char* tmp) override
    {}
};

template<typename T, typename U>
class TypedColumn: public Column
{
private:
    T _encoding;
    typename U::c_type _type;
    TypeStore<T> _store;
public:
    explicit TypedColumn() {}
    ~TypedColumn() {}
    void put(std::unique_ptr<Value>& value) override
    {
        _store.put(value->getBuffer());
    }
    std::unique_ptr<Value> get(uint64_t position) override
    {
        uint64_t offset = position * sizeof(_type);
        return std::make_unique<TypedValue<U>>(std::move(_store.get(offset, sizeof(_type))));
    }
    ViewByteBuffer getView(uint64_t position) override
    {
        uint64_t offset = position * sizeof(_type);
        return _store.getView(offset, sizeof(_type));
    }
    void swap(uint64_t lv, uint64_t rv) override
    {
        char tmp[sizeof(_type)];
        _store.swap((lv * sizeof(_type)), (rv * sizeof(_type)), sizeof(_type), &tmp[0]);
    }
};

template<typename T>
class TypedColumn<T, StringType>: public Column
{
private:
    T _encoding;
    typename StringType::c_type _type;
    TypeStore<T> _store;
    std::vector<uint64_t> _offsets;
public:
    explicit TypedColumn() {}
    ~TypedColumn() {}
    void put(std::unique_ptr<Value>& value) override
    {
        ByteBuffer value_size(sizeof(uint64_t), reinterpret_cast<char*>(&value->getBuffer()._size));
        uint64_t offset = _store.put(value_size);
        _store.put(value->getBuffer());

        _offsets.emplace_back(offset);
    }
    std::unique_ptr<Value> get(uint64_t position) override
    {
        uint64_t offset = _offsets[position];

        ByteBuffer value_size = _store.get(offset, sizeof(uint64_t));
        uint64_t size = *reinterpret_cast<uint64_t*>(value_size._data);

        ByteBuffer value = _store.get(offset + sizeof(uint64_t), size);

        return std::make_unique<TypedValue<StringType>>(std::move(value));
    }
    ViewByteBuffer getView(uint64_t position) override
    {
        uint64_t offset = _offsets[position];

        ViewByteBuffer value_size = _store.getView(offset, sizeof(uint64_t));
        uint64_t size = *reinterpret_cast<uint64_t*>(value_size._data);

        ViewByteBuffer value = _store.getView(offset + sizeof(uint64_t), size);

        return value;
    }
    void swap(uint64_t lv, uint64_t rv) override
    {
        uint64_t tmp = _offsets[lv];
        _offsets[lv] = _offsets[rv];
        _offsets[rv] = tmp;
    }
};

template<>
class TypedColumn<DictStore, StringType>: public Column
{
private:
    DictStore _encoding;
    typename StringType::c_type _type;
    TypeStore<DictStore> _store;
    std::vector<uint64_t> _offsets;
public:
    explicit TypedColumn() {}
    ~TypedColumn() {}
    void put(std::unique_ptr<Value>& value) override
    {
        uint64_t offset = _store.put(value->getBuffer());

        _offsets.emplace_back(offset);
    }
    std::unique_ptr<Value> get(uint64_t position) override
    {
        uint64_t offset = _offsets[position];

        ByteBuffer value = _store.get(offset, sizeof(ByteBuffer));

        return std::make_unique<TypedValue<StringType>>(std::move(value));
    }
    ViewByteBuffer getView(uint64_t position) override
    {
        uint64_t offset = _offsets[position];

        ViewByteBuffer value = _store.getView(offset, sizeof(ByteBuffer));

        return value;
    }
    void swap(uint64_t lv, uint64_t rv) override
    {}
};

template<typename T, typename U>
class NullableTypedColumn: public Column, IsNullable
{
private:
    T _encoding;
    typename U::c_type _type;
    TypeStore<T> _store;
public:
    explicit NullableTypedColumn() {}
    ~NullableTypedColumn() {}
    void put(std::unique_ptr<Value>& value) override
    {
        _store.put(value->getBuffer());
        NullableTypedValue<U>* null = static_cast<NullableTypedValue<U>*>(value.get());
        if (null->getNull() == true)
        {
            this->setNull(true);
        } else {
            this->setNull(false);
        }
    }
    std::unique_ptr<Value> get(uint64_t position) override
    {
        uint64_t offset = position * sizeof(_type);
        return std::make_unique<NullableTypedValue<U>>(std::move(_store.get(offset, sizeof(_type))));
    }
    ViewByteBuffer getView(uint64_t position) override
    {
        uint64_t offset = position * sizeof(_type);
        return _store.getView(offset, sizeof(_type));
    }
    void setNull(bool value) override
    {
        _nulls.push_back(value);
    }
    bool getNull(uint64_t position) override
    {
        return _nulls.at(position);
    }
    void swap(uint64_t lv, uint64_t rv) override
    {}
};

template<typename T>
class NullableTypedColumn<T, StringType>: public Column, IsNullable
{
private:
    T _encoding;
    typename StringType::c_type _type;
    TypeStore<T> _store;
    std::vector<uint64_t> _offsets;
public:
    explicit NullableTypedColumn() {}
    ~NullableTypedColumn() {}
    void put(std::unique_ptr<Value>& value) override
    {
        ByteBuffer value_size(sizeof(uint64_t), reinterpret_cast<char*>(&value->getBuffer()._size));
        uint64_t offset = _store.put(value_size);
        _store.put(value->getBuffer());

        _offsets.emplace_back(offset);

        NullableTypedValue<StringType>* null = static_cast<NullableTypedValue<StringType>*>(value.get());
        if (null->getNull() == true)
        {
            this->setNull(true);
        } else {
            this->setNull(false);
        }
    }
    std::unique_ptr<Value> get(uint64_t position) override
    {
        uint64_t offset = _offsets[position];

        ByteBuffer value_size = _store.get(offset, sizeof(uint64_t));
        uint64_t size = *reinterpret_cast<uint64_t*>(value_size._data);

        ByteBuffer value = _store.get(offset + sizeof(uint64_t), size);

        return std::make_unique<NullableTypedValue<StringType>>(std::move(value));
    }
    ViewByteBuffer getView(uint64_t position) override
    {
        uint64_t offset = _offsets[position];

        ViewByteBuffer value_size = _store.getView(offset, sizeof(uint64_t));
        uint64_t size = *reinterpret_cast<uint64_t*>(value_size._data);

        ViewByteBuffer value = _store.getView(offset + sizeof(uint64_t), size);

        return value;
    }
    void setNull(bool value) override
    {
        _nulls.push_back(value);
    }
    bool getNull(uint64_t position) override
    {
        return _nulls.at(position);
    }
    void swap(uint64_t lv, uint64_t rv) override
    {}
};

template<>
class NullableTypedColumn<DictStore, StringType>: public Column, IsNullable
{
private:
    DictStore _encoding;
    typename StringType::c_type _type;
    TypeStore<DictStore> _store;
    std::vector<uint64_t> _offsets;
public:
    explicit NullableTypedColumn() {}
    ~NullableTypedColumn() {}
    void put(std::unique_ptr<Value>& value) override
    {
        uint64_t offset = _store.put(value->getBuffer());

        _offsets.emplace_back(offset);
        NullableTypedValue<StringType>* null = static_cast<NullableTypedValue<StringType>*>(value.get());
        if (null->getNull() == true)
        {
            this->setNull(true);
        } else {
            this->setNull(false);
        }
    }
    std::unique_ptr<Value> get(uint64_t position) override
    {
        uint64_t offset = _offsets[position];

        ByteBuffer value = _store.get(offset, sizeof(ByteBuffer));

        return std::make_unique<NullableTypedValue<StringType>>(std::move(value));
    }
    ViewByteBuffer getView(uint64_t position) override
    {
        uint64_t offset = _offsets[position];

        ViewByteBuffer value = _store.getView(offset, sizeof(ByteBuffer));

        return value;
    }
    void setNull(bool value) override
    {
        _nulls.push_back(value);
    }
    bool getNull(uint64_t position) override
    {
        return _nulls.at(position);
    }
    void swap(uint64_t lv, uint64_t rv) override
    {}
};

#endif // COLUMN_H
