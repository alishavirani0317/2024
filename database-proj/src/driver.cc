#include <iostream>

#include "db.hpp"
#include "db_table.hpp"

int main() {
  DbTable table = DbTable();
  std::pair<std::string, DataType> pair;
  pair.first = "Name";
  pair.second = DataType::kString;
  table.AddColumn(pair);

  std::pair<std::string, DataType> pair_two;
  pair_two.first = "NetId";
  pair_two.second = DataType::kInt;
  table.AddColumn(pair_two);

  std::pair<std::string, DataType> pair_three;
  pair_three.first = "GPA";
  pair_three.second = DataType::kDouble;
  table.AddColumn(pair_three);

  table.AddRow({"alisha", "12341", "3.5"});
  table.AddRow({"joe", "451617", "4.0"});
  table.AddColumn({"column", DataType::kInt});

  DbTable table_copy = table; //operator
  DbTable table_constructor = DbTable(table);
  std::cout << table << std::endl;
}