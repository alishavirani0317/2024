#include "grayscale_image.hpp"

#include <cmath>
#include <fstream>
#include <stdexcept>
#include <string>
#include <vector>

#include "elevation_dataset.hpp"
GrayscaleImage::GrayscaleImage(const ElevationDataset& dataset) {
  height_ = dataset.Height();
  width_ = dataset.Width();

  if (height_ == 0 || width_ == 0) {
    throw std::invalid_argument("invalid dataset dimensions");
  }
  // resize image vector so it matches dimensions
  // populate vector with Color representation of each elevation datum;
  image_ = std::vector<std::vector<Color>>(height_, std::vector<Color>(width_));
  int max_ele = dataset.MaxEle();
  int min_ele = dataset.MinEle();

  double range = max_ele - min_ele;
  for (size_t row = 0; row < height_; ++row) {
    for (size_t col = 0; col < width_; ++col) {
      int elev = dataset.DatumAt(row, col);
      int shade = 0;
      if (range != 0) {
        shade = static_cast<int>(
            std::round((elev - min_ele) / range * kMaxColorValue));
      }
      image_[row][col] = Color(shade, shade, shade);
    }
  }
}
GrayscaleImage::GrayscaleImage(const std::string& filename,
                               size_t width,
                               size_t height) {
  ElevationDataset dataset(filename, width, height);
  height_ = dataset.Height();
  width_ = dataset.Width();
  if (height_ == 0 || width_ == 0) {
    throw std::invalid_argument("invalid dataset dimensions");
  }
  image_ = std::vector<std::vector<Color>>(height_, std::vector<Color>(width_));

  int max_ele = dataset.MaxEle();
  int min_ele = dataset.MinEle();
  double range = max_ele - min_ele;

  for (size_t row = 0; row < height_; ++row) {
    for (size_t col = 0; col < width_; ++col) {
      int elev = dataset.DatumAt(row, col);
      int shade = 0;
      if (range != 0) {
        shade = static_cast<int>(
            std::round((elev - min_ele) / range * kMaxColorValue));
      }
      image_[row][col] = Color(shade, shade, shade);
    }
  }
}

size_t GrayscaleImage::Width() const { return width_; }
size_t GrayscaleImage::Height() const { return height_; }
unsigned int GrayscaleImage::MaxColorValue() const { return kMaxColorValue; }

const std::vector<std::vector<Color>>& GrayscaleImage::GetImage() const {
  return image_;
}

void GrayscaleImage::ToPpm(const std::string& name) const {
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
      const Color& color = image_[row][col];
      // write RGB values of  color to the file
      ppm_ofs << color.Red() << " " << color.Green() << " " << color.Blue()
              << " ";
    }
    ppm_ofs << std::endl;  // end the line after writing all columns in a row
  }
}