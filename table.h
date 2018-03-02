#ifndef TABLE_H
#define TABLE_H

#include <chrono>

#include "types.h"
#include "bytebuffer.h"
#include "column.h"
#include "schema.h"

#include "builders.h"

#include "reader.h"
#include "writer.h"

#include "comparator.h"

class GroupBy;

class Table
{
private:
    uint64_t _rows = 0;
    std::string _tableName;
    Schema _schema;
    std::vector<std::unique_ptr<Column>> _columns;
    std::vector<uint64_t> _rowsSorting;
public:
    Table(std::string tableName, Schema schema):_tableName(tableName),_schema(schema) {}
    friend class GroupBy;
    Table& BuildTable()
    {
        for(uint64_t pos = 0; pos < _schema.size(); ++pos)
        {
            _columns.emplace_back(ColumnBuilder(_schema.peek(pos)));
        }

        return *this;
    }
    Table& Read4Csv(std::string fileName)
    {
        CsvReader reader(fileName, _schema, &_columns);
        this->_rows = reader.read();
        this->buildRowsSorting();

        std::cout << "rows read " << this->_rows << std::endl;

        return *this;
    }
    Table& Write2Csv(std::string fileName)
    {
        CsvWriter writer(fileName, _schema, &_columns);
        writer.write(_rowsSorting);

        return *this;
    }
    Table& sortTable(std::vector<uint64_t> columns)
    {
        std::chrono::time_point<std::chrono::high_resolution_clock> start, end;

        start = std::chrono::high_resolution_clock::now();

        for(auto it = columns.rbegin(); it != columns.rend(); ++it)
        {
            uint64_t i = *it;
            Node node = this->_schema.peek(i);
            std::shared_ptr<Comparator> comp = ComparatorBuilder(this->_columns.at(i).get(), node);
            std::stable_sort(_rowsSorting.begin(), _rowsSorting.end(), comp->_sort);
        }

        end = std::chrono::high_resolution_clock::now();
        std::chrono::duration<double> elapsed_time = end - start;

        std::cout << "sort duration = " << elapsed_time.count() << "s" << std::endl;

        return *this;
    }
private:
    void buildRowsSorting()
    {
        for(uint64_t pos = 0; pos < this->_rows; ++pos)
        {
            _rowsSorting.emplace_back(pos);
        }
    }
};

#endif // TABLE_H
