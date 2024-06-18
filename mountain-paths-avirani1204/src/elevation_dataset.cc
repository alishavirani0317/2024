#include "elevation_dataset.hpp"

#include <cassert>
#include <cstdlib>
#include <fstream>
#include <stdexcept>
#include <string>
#include <vector>
ElevationDataset::ElevationDataset(const std::string& filename,
                                   size_t width,
                                   size_t height):
    width_(width), height_(height), data_(height, std::vector<int>(width)) {
  std::ifstream ifs(filename);  // open file
  if (!ifs.is_open()) {
    throw std::runtime_error("fails to open file");
  }
  bool start = false;
  size_t row = 0;
  size_t col = 0;
  int val = 0;
  while (ifs >> val) {
    if (row >= height || col >= width) {
      throw std::runtime_error("too many data points in file");
    }
    data_[row][col] = val;  // populate the data with read values
    if (!start) {
      min_ele_ = val;
      max_ele_ = val;
      start = true;
    }
    if (val > max_ele_) {
      max_ele_ = val;  // update max_ele
    }
    if (val < min_ele_) {
      min_ele_ = val;  // update min_ele
    }
    col++;  // move to next col
    if (col == width) {
      col = 0;
      row++;  // move to next row if end of current row
    }
  }
  if (row != height || col != 0) {
    throw std::runtime_error(
        "not enough data points in file");  // check if too few data pts in file
  }
}

size_t ElevationDataset::Width() const { return width_; }

size_t ElevationDataset::Height() const { return height_; }

int ElevationDataset::MaxEle() const { return max_ele_; }

int ElevationDataset::MinEle() const { return min_ele_; }

int ElevationDataset::DatumAt(size_t row, size_t col) const {
  // assert(row < height_);
  // assert(col < width_);
  //segmentation error
  return data_[row][col];
}
const std::vector<std::vector<int>>& ElevationDataset::GetData() const {
  return data_;
}