#ifndef READER_H
#define READER_H

#include <string>
#include <vector>
#include <fstream>
#include <chrono>
#include <algorithm>

#include "schema.h"
#include "column.h"
#include "builders.h"
#include "value.h"

class CsvReader
{
private:
    std::string _fileName;
    Schema _schema;
    std::vector<std::unique_ptr<Column>>* _columns;
public:
    CsvReader(std::string fileName, Schema schema, std::vector<std::unique_ptr<Column>>* columns)
        :_fileName(fileName),_schema(schema),_columns(columns) {}
    void read()
    {
        std::chrono::time_point<std::chrono::high_resolution_clock> start, end;
        start = std::chrono::high_resolution_clock::now();

        std::vector<std::unique_ptr<UnaryOperator>> string2Type;
        for(uint64_t pos = 0; pos < _schema.size(); ++pos)
        {
            string2Type.emplace_back(String2TypeBuilder(_schema.peek(pos)));
        }

        std::fstream input(_fileName);
        std::string line;
        line.reserve(1024 * 1024);
        std::vector<std::experimental::string_view> piece;

        std::getline(input, line);
        while(std::getline(input, line))
        {
            split(piece, line, ',');
            for(size_t pos = 0; pos < string2Type.size(); ++pos)
            {
                std::experimental::string_view* value = &piece[pos];
                ByteBuffer val(value->size(), value->data());
                std::unique_ptr<Value> buffer = string2Type[pos]->operation(val);
                _columns->at(pos)->put(buffer);
            }

            piece.clear();
        }

        end = std::chrono::high_resolution_clock::now();
        std::chrono::duration<double> elapsed_time = end - start;

        std::cout << "read duration = " << elapsed_time.count() << "s" << std::endl;
    }
private:
    void split(std::vector<std::experimental::string_view>& results, std::string const& original, char separator)
    {
        std::string::const_iterator start = original.begin();
        std::string::const_iterator end = original.end();
        std::string::const_iterator next = std::find(start, end, separator);
        while(next != end && *start == '"' && *(next-1) != '"')
        {
            next = std::find(next + 1, end, separator);
        }
        while (next != end) {
            std::experimental::string_view str(start.operator ->(), static_cast<uint64_t>(next.operator ->() - start.operator ->()));
            results.push_back(str);

            start = next + 1;
            next = std::find(start, end, separator);
            while(*start == '"' && *(next-1) != '"' && next != end)
            {
                next = std::find(next + 1, end, separator);
            }
        }

        std::experimental::string_view str(start.operator ->(), static_cast<uint64_t>(next.operator ->() - start.operator ->()));
        results.push_back(str);
    }
};

#endif // READER_H
