#ifndef TABLE_H
#define TABLE_H

#include "types.h"
#include "bytebuffer.h"
#include "column.h"
#include "schema.h"

#include "builders.h"

#include "reader.h"
#include "writer.h"

class Table
{
private:
    std::string _tableName;
    Schema _schema;
    std::vector<std::unique_ptr<Column>> _columns;
public:
    Table(std::string tableName, Schema schema):_tableName(tableName),_schema(schema) {}
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
        reader.read();

        return *this;
    }
    Table& WriteCsv(std::string fileName)
    {
        CsvWriter writer(fileName, _schema, &_columns);
        writer.write();

        return *this;
    }
};

#endif // TABLE_H
