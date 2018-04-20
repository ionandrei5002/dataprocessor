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

        std::vector<std::shared_ptr<Comparator>> comparators;
        for(auto it = columns.begin(); it != columns.end(); ++it)
        {
            uint64_t i = *it;
            Node node = this->_schema.peek(i);
            std::shared_ptr<Comparator> comp = ComparatorBuilder(this->_columns.at(i).get(), node);
            comparators.push_back(comp);
        }
        Sorter sorter(comparators);
//        std::sort(_rowsSorting.begin(), _rowsSorting.end(), sorter);
        quickSort(_rowsSorting, 0, _rowsSorting.size() - 1, sorter);

//        for(auto it = columns.rbegin(); it != columns.rend(); ++it)
//        {
//            uint64_t i = *it;
//            Node node = this->_schema.peek(i);
//            std::shared_ptr<Comparator> comp = ComparatorBuilder(this->_columns.at(i).get(), node);
//            std::stable_sort(_rowsSorting.begin(), _rowsSorting.end(), comp->_sort);
//        }

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

    void swap(uint64_t* lv, uint64_t* rv)
    {
        for(auto it = _columns.begin(); it != _columns.end(); ++it)
        {
            (*it)->swap(*lv, *rv);
        }
//        uint64_t t = *lv;
//        *lv = *rv;
//        *rv = t;
    }

    uint64_t partition (std::vector<uint64_t>& arr, uint64_t low, uint64_t high, Sorter& sorter)
    {
        uint64_t pivot = arr[high];    // pivot
        uint64_t i = (low - 1);  // Index of smaller element

        for (uint64_t j = low; j <= high - 1; j++)
        {
            // If current element is smaller than or
            // equal to pivot
//            if (arr[j] <= pivot)
            if (sorter.operator()(arr[j], pivot))
            {
                i++;    // increment index of smaller element
                swap(&arr[i], &arr[j]);
            }
        }
        swap(&arr[i + 1], &arr[high]);
        return (i + 1);
    }

    /* The main function that implements QuickSort
     arr[] --> Array to be sorted,
      low  --> Starting index,
      high  --> Ending index */
    void quickSort(std::vector<uint64_t>& arr, uint64_t low, uint64_t high, Sorter& sorter)
    {
        if (low < high)
        {
            /* pi is partitioning index, arr[p] is now
               at right place */
            uint64_t pi = partition(arr, low, high, sorter);

            // Separately sort elements before
            // partition and after partition
            quickSort(arr, low, pi - 1, sorter);
            quickSort(arr, pi + 1, high, sorter);
        }
    }
};

#endif // TABLE_H
