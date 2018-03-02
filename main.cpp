#include <iostream>

#include "schema.h"
#include "table.h"
#include "builders.h"
#include "aggregators.h"
#include "groupby.h"

int main()
{
    Schema schema;
    schema.push(Node("udid", Type::INT64)).push(Node("date", Type::STRING));
    
    Table table("MC5Dau", schema);
    table.BuildTable()
            .Read4Csv("/home/andrei/Desktop/MC5Dau.csv")
            .sortTable(std::vector<uint64_t>({0,1}));
//            .Write2Csv("/home/andrei/Desktop/output.csv");

    Schema _aggSchema;
    _aggSchema.push(Node("udid", Type::INT64)).push(Node("nb", Type::INT64));
    Table destination("MC5Agg", _aggSchema);
    destination.BuildTable();

    GroupBy group(&table, &destination);
    auto agg1 = AggregatorBuilder(_aggSchema.peek(0), AggType::NONE);
    auto agg2 = AggregatorBuilder(_aggSchema.peek(1), AggType::DISTINCTC_COUNT);
    group.add(agg1)
            .add(agg2)
            .column(0)
            .run();

    destination.Write2Csv("/home/andrei/Desktop/output.csv");

    return 0;
}
