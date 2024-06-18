#include "db.hpp"

void Database::CreateTable(const std::string& table_name) {
  tables_[table_name] = new DbTable();
}
void Database::DropTable(const std::string& table_name) {
  if (!tables_.contains(table_name)) {
    throw std::invalid_argument("table_name does not exist in tables_");
  }
  delete tables_[table_name];
  tables_.erase(table_name);
}
DbTable& Database::GetTable(const std::string& table_name) {
  return *(tables_[table_name]);
}

Database::Database(const Database& rhs) {
  for (const auto& pair : rhs.tables_) {
    tables_[pair.first] = new DbTable(*pair.second);
  }
}
Database& Database::operator=(const Database& rhs) {
  if (this != &rhs) {
    // Clear existing data
    for (auto& pair : tables_) {
      delete pair.second;
    }
    tables_.clear();
    // Deep copy each DbTable object
    for (const auto& pair : rhs.tables_) {
      tables_[pair.first] = new DbTable(*pair.second);
    }
  }
  return *this;
}
Database::~Database() {
  for (auto& pair : tables_) {
    delete pair.second;
  }
}
std::ostream& operator<<(std::ostream& os, const Database& db) {
  for (auto const& [key, value] : db.tables_) {
    os << *value << '\n';
  }
  return os;
}