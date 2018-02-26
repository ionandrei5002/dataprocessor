#ifndef WRITER_H
#define WRITER_H

#include <string>
#include <vector>
#include <fstream>
#include <chrono>

#include "schema.h"
#include "column.h"
#include "builders.h"
#include "value.h"

class CsvWriter
{
private:
    std::string _fileName;
    Schema _schema;
    std::vector<std::unique_ptr<Column>>* _columns;
public:
    CsvWriter(std::string fileName, Schema schema, std::vector<std::unique_ptr<Column>>* columns)
        :_fileName(fileName),_schema(schema),_columns(columns) {}
    void write()
    {
        std::chrono::time_point<std::chrono::high_resolution_clock> start, end;
        start = std::chrono::high_resolution_clock::now();

        std::vector<std::unique_ptr<UnaryOperator>> type2String;
        for(uint64_t pos = 0; pos < _schema.size(); ++pos)
        {
            type2String.emplace_back(Type2StringBuilder(_schema.peek(pos)));
        }

        std::ofstream out(_fileName);

        for(uint64_t pos = 0; pos < 16353154; ++pos)
        {
            for(uint64_t col = 0; col < (_columns->size() - 1); ++col)
            {
                std::unique_ptr<Value> value = _columns->at(col)->get(pos);
                out << type2String.at(col)->operation(value->getBuffer())->getBuffer() << ",";
            }
            std::unique_ptr<Value> value = _columns->at(_columns->size() - 1)->get(pos);
            out << type2String.at(_columns->size() - 1)->operation(value->getBuffer())->getBuffer() << std::endl;
        }

        end = std::chrono::high_resolution_clock::now();
        std::chrono::duration<double> elapsed_time = end - start;

        std::cout << "write duration = " << elapsed_time.count() << "s" << std::endl;
    }
};

#endif // WRITER_H
