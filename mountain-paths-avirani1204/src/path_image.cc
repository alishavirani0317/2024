#include "path_image.hpp"

#include <fstream>
#include <iostream>

#include "path_image.hpp"

constexpr int kRedR = 252;
constexpr int kGreenR = 25;
constexpr int kBlueR = 63;
const Color kColorRed(kRedR, kGreenR, kBlueR);

constexpr int kRedG = 31;
constexpr int kGreenG = 253;
constexpr int kBlueG = 13;
const Color kColorGreen(kRedG, kGreenG, kBlueG);
void PathImage::HandleFirstRow(size_t& row,
                               size_t col,
                               Path& path,
                               const ElevationDataset& dataset) {
  unsigned int straight_change =
      std::abs(dataset.DatumAt(row, col) - dataset.DatumAt(row, col + 1));
  unsigned int down_change =
      std::abs(dataset.DatumAt(row, col) - dataset.DatumAt(row + 1, col + 1));
  if (straight_change <= down_change) {
    path.SetLoc(col + 1, row);
    path.IncEleChange(straight_change);
  } else {
    path.SetLoc(col + 1, row + 1);
    row++;
    path.IncEleChange(down_change);
  }
}

// Helper function to handle cases when current row is the last row
void PathImage::HandleLastRow(size_t& row,
                              size_t col,
                              Path& path,
                              const ElevationDataset& dataset) {
  unsigned int straight_change =
      std::abs(dataset.DatumAt(row, col) - dataset.DatumAt(row, col + 1));
  unsigned int up_change =
      std::abs(dataset.DatumAt(row, col) - dataset.DatumAt(row - 1, col + 1));
  if (up_change < straight_change) {
    path.SetLoc(col + 1, row - 1);
    path.IncEleChange(up_change);
    row--;
  } else {
    path.SetLoc(col + 1, row);
    path.IncEleChange(straight_change);
  }
}

void PathImage::HandleOtherRows(size_t& row,
                                size_t col,
                                Path& path,
                                const ElevationDataset& dataset) {
  unsigned int straight_change =
      std::abs(dataset.DatumAt(row, col) - dataset.DatumAt(row, col + 1));
  unsigned int up_change =
      std::abs(dataset.DatumAt(row, col) - dataset.DatumAt(row - 1, col + 1));
  unsigned int down_change =
      std::abs(dataset.DatumAt(row, col) - dataset.DatumAt(row + 1, col + 1));

  if (up_change < straight_change && up_change < down_change) {
    path.SetLoc(col + 1, row - 1);
    path.IncEleChange(up_change);
    row--;
    // path_image_[row][col + 1] = kColorRed;
  } else if (straight_change <= up_change && straight_change <= down_change) {
    path.SetLoc(col + 1, row);
    path.IncEleChange(straight_change);

  } else {
    path.SetLoc(col + 1, row + 1);
    path.IncEleChange(down_change);
    row++;
    // path_image_[row][col + 1] = kColorRed;
  }
}
PathImage::PathImage(const GrayscaleImage& image,
                     const ElevationDataset& dataset) {
  width_ = image.Width();
  height_ = image.Height();
  path_image_ = image.GetImage();
  size_t row = 0;
  size_t best_path_row = 0;
  Path path(width_, row);
  path.SetLoc(0, row);
  for (size_t row = 0; row < height_; ++row) {
    Path path(width_, row);
    size_t curr_row = row;
    path.SetLoc(0, curr_row);
    for (size_t col = 0; col < width_ - 1; ++col) {
      if (curr_row == 0) {
        HandleFirstRow(curr_row, col, path, dataset);
      } else if (curr_row == height_ - 1) {
        HandleLastRow(curr_row, col, path, dataset);
      } else {
        HandleOtherRows(curr_row, col, path, dataset);
      }
    }
    paths_.push_back(path);
  }
  for (size_t row = 0; row < height_; row++) {
    if (paths_.at(row).EleChange() < paths_.at(best_path_row).EleChange()) {
      best_path_row = row;
    }
  }
  for (size_t row = 0; row < height_; row++) {
    for (size_t col = 0; col < width_; col++) {
      path_image_.at(paths_.at(row).GetPath().at(col)).at(col) = kColorRed;
    }
  }
  for (size_t col = 0; col < width_; col++) {
    path_image_.at(paths_.at(best_path_row).GetPath().at(col)).at(col) =
        kColorGreen;
  }
}

size_t PathImage::Width() const { return width_; }

size_t PathImage::Height() const { return height_; }

unsigned int PathImage::MaxColorValue() const { return kMaxColorValue; }

const std::vector<Path>& PathImage::Paths() const { return paths_; }

const std::vector<std::vector<Color>>& PathImage::GetPathImage() const {
  return path_image_;
}

void PathImage::ToPpm(const std::string& name) const {
  std::ofstream ppm_ofs(name);
  if (!ppm_ofs.is_open()) {
    throw std::runtime_error("failed to open file");
  }
  ppm_ofs << "P3"
          << " \n"
          << width_ << " " << height_ << std::endl;
  ppm_ofs << kMaxColorValue << std::endl;
  for (size_t row = 0; row < height_; ++row) {
    // loop through each column of image
    for (size_t col = 0; col < width_; ++col) {
      // get the color at current position in image
      const Color& color = path_image_.at(row).at(col);
      // write RGB values of  color to the file
      ppm_ofs << color.Red() << " " << color.Green() << " " << color.Blue()
              << " ";
    }
    ppm_ofs << std::endl;  // end the line after writing all columns in a row
  }
}