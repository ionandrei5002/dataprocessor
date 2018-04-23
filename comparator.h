#ifndef COMPARATOR_H
#define COMPARATOR_H

#include "types.h"
#include "value.h"
#include "column.h"

class Comparator {
public:
    Comparator() = delete;
    Comparator(Column* column):_sort(Sort(column, this)) {}
    Comparator(const Comparator& ot):_sort(Sort(ot._sort._column, ot._sort._children)) {}
    Comparator& operator=(const Comparator& ot)
    {
        this->_sort = Sort(ot._sort._column, ot._sort._children);
        return *this;
    }
    Comparator(Comparator&& ot):_sort(Sort(ot._sort._column, ot._sort._children)) {}
    Comparator& operator=(Comparator&& ot) = delete;
    virtual ~Comparator() {}
    virtual Type::type getType();
    virtual void set(const ViewByteBuffer& lv) = 0;
    virtual bool operator<(const ViewByteBuffer& rv) = 0;
    virtual bool operator>(const ViewByteBuffer& rv) = 0;
    virtual bool operator==(const ViewByteBuffer& rv) = 0;
    virtual bool operator!=(const ViewByteBuffer& rv) = 0;
    virtual bool operator()(const ViewByteBuffer& lv, const ViewByteBuffer& rv) = 0;
    struct Sort
    {
    public:
        Column* _column = nullptr;
        Comparator* _children = nullptr;
    public:
        Sort() = delete;
        Sort(Column* column, Comparator* children):_column(column),_children(children) {}
        Sort(const Sort& ot)
        {
            _column =  ot._column;
            _children = ot._children;
        }
        Sort& operator=(const Sort& ot)
        {
            _column =  ot._column;
            _children = ot._children;

            return *this;
        }
        Sort(Sort&& ot)
        {
            _column =  ot._column;
            _children = ot._children;
        }
        Sort& operator=(Sort&& ot)
        {
            _column =  ot._column;
            _children = ot._children;

            return *this;
        }
        inline bool differit(const uint64_t lv, const uint64_t rv)
        {
            ViewByteBuffer vlv = _column->getView(lv);
            ViewByteBuffer vrv = _column->getView(rv);

            _children->set(vlv);

            return _children->operator!=(vrv);
        }
        inline bool operator()(const uint64_t lv, const uint64_t rv)
        {
            ViewByteBuffer vlv = _column->getView(lv);
            ViewByteBuffer vrv = _column->getView(rv);

            return _children->operator ()(vlv, vrv);
        }
        inline void swap(const uint64_t lv, const uint64_t rv)
        {
            _column->swap(lv, rv);
        }
    } _sort;
};

template<typename T>
class TypedComparator: public Comparator
{
private:
    ViewByteBuffer _lv;
public:
    TypedComparator() = delete;
    TypedComparator(Column* column):Comparator(column) {}
    TypedComparator(const TypedComparator<T>& oth):Comparator(oth._sort._column) {}
    TypedComparator& operator=(const TypedComparator<T>& oth) = delete;
    Type::type getType() override
    {
        return T::type_num;
    }
    void set(const ViewByteBuffer& lv) override
    {
        _lv = lv;
    }
    bool operator<(const ViewByteBuffer& rv) override
    {
        return _lv < rv;
    }
    bool operator>(const ViewByteBuffer& rv) override
    {
        return _lv > rv;
    }
    bool operator==(const ViewByteBuffer& rv) override
    {
        return _lv == rv;
    }
    bool operator!=(const ViewByteBuffer& rv) override
    {
        return _lv != rv;
    }
    bool operator()(const ViewByteBuffer& lv, const ViewByteBuffer& rv) override
    {
        return lv < rv;
    }
};

template<typename T>
class NullableTypedComparator: public Comparator
{
private:
    ViewByteBuffer _lv;
public:
    NullableTypedComparator() = delete;
    NullableTypedComparator(Column* column):Comparator(column) {}
    NullableTypedComparator(const NullableTypedComparator<T>& oth):Comparator(oth._sort._column) {}
    NullableTypedComparator& operator=(const NullableTypedComparator<T>& oth) = delete;
    Type::type getType() override
    {
        return T::type_num;
    }
    void set(const ViewByteBuffer& lv) override
    {
        _lv = lv;
    }
    bool operator<(const ViewByteBuffer& rv) override
    {
        return _lv < rv;
    }
    bool operator>(const ViewByteBuffer& rv) override
    {
        return _lv > rv;
    }
    bool operator==(const ViewByteBuffer& rv) override
    {
        return _lv == rv;
    }
    bool operator!=(const ViewByteBuffer& rv) override
    {
        return _lv != rv;
    }
    bool operator()(const ViewByteBuffer& lv, const ViewByteBuffer& rv) override
    {
        return lv < rv;
    }
};

class Sorter {
private:
    std::vector<std::shared_ptr<Comparator>> _comparators;
public:
    Sorter(std::vector<std::shared_ptr<Comparator>> comparators):_comparators(comparators) {}
    inline bool operator()(const uint64_t lv, const uint64_t rv) {
        bool _comp = false;

        for(auto it = _comparators.begin(); it != _comparators.end(); ++it)
        {
            if ((*it)->_sort.differit(lv,rv))
            {
                _comp = (*it)->_sort.operator ()(lv, rv);

                return _comp;
            }
        }

        return _comp;
    }
};

#endif // COMPARATOR_H
