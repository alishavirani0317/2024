#include "path.hpp"

#include <stdexcept>
Path::Path(size_t length, size_t starting_row):
    length_(length), starting_row_(starting_row), path_(length) {
  // initialize path vector with length elements
}

size_t Path::Length() const { return length_; }

size_t Path::StartingRow() const { return starting_row_; }

unsigned int Path::EleChange() const { return ele_change_; }

void Path::IncEleChange(unsigned int value) { ele_change_ += value; }

const std::vector<size_t>& Path::GetPath() const { return path_; }

void Path::SetLoc(size_t col, size_t row) {
  // if (col ++ >= length_) {
  //   throw std::out_of_range("Column index out of range");
  // }
  path_[col] = row;
}