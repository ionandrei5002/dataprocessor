#ifndef GROUPBY_H
#define GROUPBY_H

#include <chrono>

#include "table.h"
#include "aggregators.h"
#include "builders.h"

class GroupBy
{
private:
    Table* _source;
    Table* _destination;
    std::vector<std::shared_ptr<Aggregator>> _group;
    std::vector<uint64_t> _groupCol;
public:
    GroupBy(Table* source, Table* destination)
        :_source(source),
        _destination(destination) {}
    GroupBy add(std::shared_ptr<Aggregator>& aggregator)
    {
        _group.push_back(aggregator);
        return *this;
    }
    GroupBy column(uint64_t col)
    {
        _groupCol.push_back(col);
        return *this;
    }
    void run()
    {
        std::chrono::time_point<std::chrono::high_resolution_clock> start, end;
        start = std::chrono::high_resolution_clock::now();

        uint64_t size = 0;
        bool found = false;

        std::vector<std::unique_ptr<Value>> _helper;
        std::vector<std::unique_ptr<Comparator>> _comparators;

        for(uint64_t pos = 0; pos < _source->_columns.size(); ++pos)
        {
            _comparators.push_back(ComparatorBuilder(_source->_columns.at(pos).get(), _source->_schema.peek(pos)));
        }

        for(uint64_t pos : _source->_rowsSorting)
        {
            if (size == 0)
            {
                for(uint64_t spos = 0; spos < _source->_columns.size(); spos++)
                {
                    Aggregator* agg = _group[spos].get();

                    agg->input(_source->_columns[spos]->get(pos)->getBuffer());

                    _helper.push_back(_source->_columns[spos]->get(pos));
                }
                size++;
            } else {
                for(uint64_t gpos : _groupCol)
                {
                    Comparator* comp = _comparators[gpos].get();
                    comp->set(_source->_columns[gpos]->getView(pos));

                    if(comp->operator !=(_helper[gpos]->getBuffer()) == true)
                    {
                        found = false;
                        break;
                    } else {
                        found = true;
                    }
                }

                if (found == true)
                {
                    for(uint64_t spos = 0; spos < _source->_columns.size(); spos++)
                    {
                        Aggregator* agg = _group[spos].get();

                        agg->input(_source->_columns[spos]->get(pos)->getBuffer());
                    }
                }

                if (found == false)
                {
                    for(uint64_t spos = 0; spos < _source->_columns.size(); spos++)
                    {
                        Aggregator* agg = _group[spos].get();
                        std::unique_ptr<Value> val = agg->output();
                        _destination->_columns[spos]->put(val);

                        agg->reset();

                        {
                            std::unique_ptr<Value> cell = _source->_columns[spos]->get(pos);
                            agg->input(cell);
                        }
                        {
                            std::unique_ptr<Value> cell = _source->_columns[spos]->get(pos);
                            _helper[spos]->set(cell);
                        }
                    }
                    _destination->_rows++;
                    size++;
                }
            }
        }

        if (found == true)
        {
            for(uint64_t spos = 0; spos < _source->_columns.size(); spos++)
            {
                Aggregator* agg = _group[spos].get();
                std::unique_ptr<Value> val = agg->output();
                _destination->_columns[spos]->put(val);

                agg->reset();
            }
        }
        _destination->_rows++;

        std::cout << "rows groupby " << _destination->_rows << std::endl;
        _destination->buildRowsSorting();

        end = std::chrono::high_resolution_clock::now();
        std::chrono::duration<double> elapsed_time = end - start;

        std::cout << "groupby duration = " << elapsed_time.count() << "s" << std::endl;
    }
};

#endif // GROUPBY_H
