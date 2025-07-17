#include "db_table.hpp"

// const unsigned int kRowGrowthRate = 2;

// to resize look at Dynamic allocation of objects on the free store cont.

void DbTable::AddColumn(const std::pair<std::string, DataType>& col_desc) {
  // resize every row array mapped to by rows_
  if (col_descs_.size() == row_col_capacity_) {
    // resize every row mapped to by rows_ to have row col cap * 2
    row_col_capacity_ = row_col_capacity_ * 2;
    unsigned int half_rcc = row_col_capacity_ / 2;
    for (auto& [key, value] : rows_) {
      // new array w updated cap
      void** new_arr = new void*[row_col_capacity_];
      // copy data to new arr
      for (unsigned int i = 0; i < half_rcc; ++i) {
        new_arr[i] = value[i];
      }
      // delete old arr
      delete[] value;
      // update row w new arr
      value = new_arr;
    }
  }
  col_descs_.push_back(col_desc);
  // Iterate through each row and resize the data array
  // for every data mapped by rows: use map
  for (auto const& [key, val] : rows_) {
    if (col_desc.second == DataType::kString) {
      val[col_descs_.size() - 1] = static_cast<void*>(new std::string(""));
    } else if (col_desc.second == DataType::kInt) {
      val[col_descs_.size() - 1] = static_cast<void*>(new int(0));
    } else if (col_desc.second == DataType::kDouble) {
      val[col_descs_.size() - 1] = static_cast<void*>(new double(0.0));
    }
  }
}
void DbTable::DeleteColumnByIdx(unsigned int col_idx) {
  // throw out of range exception if col_dix is outside the range of col_descs
  if (col_idx >= col_descs_.size() || col_idx < 0) {
    throw std::out_of_range("col_idx is outside the range of col_descs_.");
  }
  if (col_descs_.size() == 1 && !rows_.empty()) {
    throw std::runtime_error(
        "any table with at least one row must have at least one column");
  }
  // for every mapped row in rows_:
  for (auto const& [key, val] : rows_) {
    // Deallocates dynam all obj residing at row[col_idx]
    if (col_descs_[col_idx].second == DataType::kString) {
      delete static_cast<std::string*>(val[col_idx]);
    } else if (col_descs_[col_idx].second == DataType::kInt) {
      delete static_cast<int*>(val[col_idx]);
    } else if (col_descs_[col_idx].second == DataType::kDouble) {
      delete static_cast<double*>(val[col_idx]);
    }
    for (unsigned int i = col_idx; i < col_descs_.size() - 1; i++) {
      // Shifts all elements w indexes > col_idx down one index position
      // to close gap caused by "deleted col" at row[col_idx]
      // and to keep parallel relationship w col_descs_
      val[i] = val[i + 1];
    }
  }
  // Erases std::pair at col_idx from col_descs_, effectively decreasing
  col_descs_.erase(std::next(col_descs_.begin(), col_idx));
  // the size of col_descs_ by 1.
}

void DbTable::AddRow(const std::initializer_list<std::string>& col_data) {
  // Ensure the number of items in col_data is equal to the number of columns
  if (col_data.size() != col_descs_.size()) {
    throw std::invalid_argument(
        "num of items in col_data not match number of cols");
  }
  // Create a new entry in rows_
  void** new_row = new void*[row_col_capacity_];
  // Insert dynamically allocated objects storing col_data into new row
  const auto* col_data_iter = col_data.begin();
  for (size_t i = 0; i < col_descs_.size(); ++i) {
    const std::string& data = *col_data_iter;
    if (col_descs_[i].second == DataType::kString) {
      new_row[i] = new std::string(data);
    } else if (col_descs_[i].second == DataType::kInt) {
      new_row[i] = new int(std::stoi(data));
    } else if (col_descs_[i].second == DataType::kDouble) {
      new_row[i] = new double(std::stod(data));
    }
    ++col_data_iter;
  }
  // Insert the new row into rows_
  rows_[next_unique_id_++] = new_row;
}
void DbTable::DeleteRowById(unsigned int id) {
  // if row w id does not exist, throw exception
  if (!rows_.contains(id)) {
    throw std::invalid_argument("row with id does not exist");
  }
  for (size_t i = 0; i < col_descs_.size(); ++i) {
    if (col_descs_[i].second == DataType::kString) {
      delete static_cast<std::string*>(rows_[id][i]);
    } else if (col_descs_[i].second == DataType::kInt) {
      delete static_cast<int*>(rows_[id][i]);
    } else if (col_descs_[i].second == DataType::kDouble) {
      delete static_cast<double*>(rows_[id][i]);
    }
  }
  delete[] rows_[id];
  rows_.erase(id);
}

DbTable::DbTable(const DbTable& rhs): col_descs_(rhs.col_descs_) {
  // copy constructor. implements deep copy for DbTable typed objs
  next_unique_id_ = rhs.next_unique_id_;
  row_col_capacity_ = rhs.row_col_capacity_;
  for (const auto& pair : rhs.rows_) {
    void** new_row_data = new void*[row_col_capacity_];
    for (size_t i = 0; i < col_descs_.size(); ++i) {
      if (col_descs_[i].second == DataType::kString) {
        new_row_data[i] =
            new std::string(*static_cast<std::string*>(pair.second[i]));
      } else if (col_descs_[i].second == DataType::kInt) {
        new_row_data[i] = new int(*static_cast<int*>(pair.second[i]));
      } else if (col_descs_[i].second == DataType::kDouble) {
        new_row_data[i] = new double(*static_cast<double*>(pair.second[i]));
      }
    }
    rows_[pair.first] = new_row_data;
  }
}

DbTable& DbTable::operator=(const DbTable& rhs) {
  if (this == &rhs) {
    return *this;
  }
  // col_descs_ = rhs.col_descs_;
  for (auto& pair : rows_) {
    for (size_t i = 0; i < col_descs_.size(); ++i) {
      if (col_descs_[i].second == DataType::kString) {
        delete static_cast<std::string*>(pair.second[i]);
      } else if (col_descs_[i].second == DataType::kInt) {
        delete static_cast<int*>(pair.second[i]);
      } else if (col_descs_[i].second == DataType::kDouble) {
        delete static_cast<double*>(pair.second[i]);
      }
    }
    delete[] pair.second;
  }
  col_descs_ = rhs.col_descs_;
  next_unique_id_ = rhs.next_unique_id_;
  row_col_capacity_ = rhs.row_col_capacity_;
  rows_.clear();
  for (const auto& pair : rhs.rows_) {
    void** new_row_data = new void*[row_col_capacity_];
    for (size_t i = 0; i < col_descs_.size(); ++i) {
      if (col_descs_[i].second == DataType::kString) {
        new_row_data[i] =
            new std::string(*static_cast<std::string*>(pair.second[i]));
      } else if (col_descs_[i].second == DataType::kInt) {
        new_row_data[i] = new int(*static_cast<int*>(pair.second[i]));
      } else if (col_descs_[i].second == DataType::kDouble) {
        new_row_data[i] = new double(*static_cast<double*>(pair.second[i]));
      }
    }
    rows_[pair.first] = new_row_data;
  }
  return *this;
}

DbTable::~DbTable() {
  for (auto& pair : rows_) {
    for (size_t i = 0; i < col_descs_.size(); ++i) {
      if (col_descs_[i].second == DataType::kString) {
        delete static_cast<std::string*>(pair.second[i]);
      } else if (col_descs_[i].second == DataType::kInt) {
        delete static_cast<int*>(pair.second[i]);
      } else if (col_descs_[i].second == DataType::kDouble) {
        delete static_cast<double*>(pair.second[i]);
      }
    }
    delete[] pair.second;
  }
}

std::ostream& operator<<(std::ostream& os, const DbTable& table) {
  // column names
  for (size_t i = 0; i < table.col_descs_.size(); ++i) {
    os << table.col_descs_[i].first << "(";
    if (table.col_descs_[i].second == DataType::kString) {
      os << "std::string";
    } else if (table.col_descs_[i].second == DataType::kInt) {
      os << "int";
    } else if (table.col_descs_[i].second == DataType::kDouble) {
      os << "double";
    }
    os << ")";
    if (i != table.col_descs_.size() - 1) {
      os << ", ";
    }
  }
  os << std::endl;
  // table rows
  for (const auto& pair : table.rows_) {
    for (size_t i = 0; i < table.col_descs_.size(); ++i) {
      if (table.col_descs_[i].second == DataType::kString) {
        os << *(static_cast<std::string*>(pair.second[i]));
      } else if (table.col_descs_[i].second == DataType::kInt) {
        os << *(static_cast<int*>(pair.second[i]));
      } else if (table.col_descs_[i].second == DataType::kDouble) {
        os << *(static_cast<double*>(pair.second[i]));
      }
      if (i != table.col_descs_.size() - 1) {
        os << ", ";
      }
    }
    os << std::endl;
  }
  return os;
}
