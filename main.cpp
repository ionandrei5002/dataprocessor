#include <iostream>

#include "schema.h"
#include "table.h"
#include "builders.h"

int main()
{
    Schema schema;
    schema.push(Node("udid", Type::INT64, true)).push(Node("date", Type::STRING, true));

    Table table("MC5Dau", schema);
    table.BuildTable().Read4Csv("/home/andrei/Desktop/MC5Dau.csv");

    return 0;
}
