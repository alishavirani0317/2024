#include "seam_carver.hpp"

#include "image_ppm.hpp"
// implement the rest of SeamCarver's functions here

// given functions below, DO NOT MODIFY

SeamCarver::SeamCarver(const ImagePPM& image): image_(image) {
  height_ = image.GetHeight();
  width_ = image.GetWidth();
}

void SeamCarver::SetImage(const ImagePPM& image) {
  image_ = image;
  width_ = image.GetWidth();
  height_ = image.GetHeight();
}

const ImagePPM& SeamCarver::GetImage() const { return image_; }

int SeamCarver::GetHeight() const { return height_; }

int SeamCarver::GetWidth() const { return width_; }

// edge cases:
//  if row = 0 then for the left value wrap around, row = height -1;
//  corner : if row = 0, col = height - 1, then wrap around the top value and
//  the right value

int SeamCarver::GetEnergy(int row, int col) const {
  int ri_col = 0, l_col = 0, u_row = 0, d_row = 0;
  if (row == 0) {
    u_row = height_ - 1;
    d_row = row + 1;
  } else if (row == height_ - 1) {
    u_row = row - 1;
    d_row = 0;
  } else {
    u_row = row - 1;
    d_row = row + 1;
  }
  if (col == 0) {
    l_col = width_ - 1;
    ri_col = col + 1;
  } else if (col == width_ - 1) {
    l_col = col - 1;
    ri_col = 0;
  } else {
    l_col = col - 1;
    ri_col = col + 1;
  }
  int r_col = image_.GetPixel(row, l_col).GetRed() -
              image_.GetPixel(row, (ri_col)).GetRed();
  int g_col = image_.GetPixel(row, l_col).GetGreen() -
              image_.GetPixel(row, (ri_col)).GetGreen();
  int b_col = image_.GetPixel(row, l_col).GetBlue() -
              image_.GetPixel(row, (ri_col)).GetBlue();
  int change_in_col = (r_col * r_col) + (g_col * g_col) + (b_col * b_col);
  int r_row = image_.GetPixel(u_row, col).GetRed() -
              image_.GetPixel((d_row), col).GetRed();
  int g_row = image_.GetPixel(u_row, col).GetGreen() -
              image_.GetPixel((d_row), col).GetGreen();
  int b_row = image_.GetPixel(u_row, col).GetBlue() -
              image_.GetPixel((d_row), col).GetBlue();
  int change_in_row = (r_row * r_row) + (g_row * g_row) + (b_row * b_row);
  return change_in_col + change_in_row;
}

// helper function to get 2d dynamic array of energy, delete array in functions
// u call it in
int** SeamCarver::GetEnergyArr() const {
  int** energy = new int*[height_];
  for (int row = 0; row < height_; ++row) {
    energy[row] = new int[width_];
    for (int col = 0; col < width_; ++col) {
      energy[row][col] = GetEnergy(row, col);
    }
  }
  return energy;
}
// find min path vertical
int** SeamCarver::FindMinVertPath(int** energy) const {
  int** dp = new int*[height_];
  for (int row = 0; row < height_; ++row) {
    dp[row] = new int[width_];
  }
  for (int col = 0; col < width_; ++col) {
    dp[height_ - 1][col] = energy[height_ - 1][col];
  }
  for (int row = height_ - 2; row >= 0; --row) {
    for (int col = 0; col < width_; ++col) {
      int down_col = std::min(width_ - 1, col + 1);
      int down_left_col = std::max(0, col - 1);

      int min_val = dp[row + 1][col];
      if (dp[row + 1][down_col] < min_val) {
        min_val = dp[row + 1][down_col];
      }
      if (dp[row + 1][down_left_col] < min_val) {
        min_val = dp[row + 1][down_left_col];
      }
      dp[row][col] = energy[row][col] + min_val;
    }
  }
  return dp;
}

// when calculating energies -- helper functions to accumulate energies
// findminpathhorizontal

int** SeamCarver::FindMinHorPath(int** energy) const {
  int** dp = new int*[height_];
  for (int row = 0; row < height_; ++row) {
    dp[row] = new int[width_];
  }
  // Initialize the last column of dp with energy values
  for (int row = 0; row < height_; ++row) {
    dp[row][width_ - 1] = energy[row][width_ - 1];
  }
  // Calculate the minimum energy path
  for (int col = width_ - 2; col >= 0; --col) {
    for (int row = 0; row < height_; ++row) {
      int left_row = std::max(0, row - 1);
      int right_row = std::min(height_ - 1, row + 1);
      int min_val = dp[row][col + 1];
      if (dp[left_row][col + 1] < min_val) {
        min_val = dp[left_row][col + 1];
      }
      if (dp[right_row][col + 1] < min_val) {
        min_val = dp[right_row][col + 1];
      }
      dp[row][col] = energy[row][col] + min_val;
    }
  }
  return dp;
}
int* SeamCarver::GetHorizontalSeam() const {
  if (width_ == 0 || height_ == 0) throw std::invalid_argument("");
  int* seam = new int[width_];    // height_
  int** energy = GetEnergyArr();  // Get energy array
  int** dp = FindMinHorPath(energy);
  int min_energy = dp[0][0];
  int min_row = 0;
  for (int row = 1; row < height_; ++row) {
    if (dp[row][0] < min_energy) {
      min_energy = dp[row][0];
      min_row = row;
    }
  }
  seam[0] = min_row;
  for (int col = 1; col < width_; ++col) {
    int up_row = std::max(0, min_row - 1);
    int down_row = std::min(height_ - 1, min_row + 1);
    int min_val = dp[min_row][col];
    if (dp[up_row][col] < min_val) {
      min_val = dp[up_row][col];
    }
    if (dp[down_row][col] < min_val) {
      min_val = dp[down_row][col];
    }
    if (min_val != dp[min_row][col]) {
      if (min_val == dp[up_row][col]) {
        min_row = up_row;
      } else if (min_val == dp[down_row][col]) {
        min_row = down_row;
      }
    }
    seam[col] = min_row;  // out of bounds error
  }
  for (int i = 0; i < height_; ++i) delete[] dp[i];
  delete[] dp;
  for (int i = 0; i < height_; ++i) {
    delete[] energy[i];
  }
  delete[] energy;
  return seam;
}

// if failing test cases revisit tie breaking
//  if (dp[row][down_col] < min_val) {
//    min_val = dp[row][down_col];
//  }
// line 205
// helper w dp and energy and delete them
int* SeamCarver::GetVerticalSeam() const {
  if (width_ == 0 || height_ == 0) throw std::invalid_argument("");
  int* seam = new int[width_];
  int** energy = GetEnergyArr();
  int** dp = FindMinVertPath(energy);
  int min_energy = dp[0][0];
  int min_col = 0;
  for (int col = 1; col < width_; ++col) {
    if (dp[0][col] < min_energy) min_energy = dp[0][col], min_col = col;
  }
  seam[0] = min_col;
  for (int row = 1; row < height_; ++row) {
    int down_col = min_col, min_val = dp[row][down_col];
    int down_left_col = std::max(0, min_col - 1);
    int down_right_col = std::min(width_ - 1, min_col + 1);
    if (dp[row][down_left_col] < min_val) {
      min_val = dp[row][down_left_col], min_col = down_left_col;
    }
    if (dp[row][down_right_col] < min_val) {
      min_col = down_right_col;
    }
    seam[row] = min_col;
  }
  for (int i = 0; i < height_; ++i) delete[] dp[i];
  delete[] dp;
  for (int i = 0; i < height_; ++i) {
    delete[] energy[i];
  }
  delete[] energy;
  return seam;
  for (int i = 0; i < height_; ++i) {
    delete[] dp[i];
  }
  delete[] dp;
  for (int i = 0; i < height_; ++i) {
    delete[] energy[i];
  }
  delete[] energy;
  return seam;
}
// 2 helper functions within image ppm.cc -- making a new 2d pixels array --
// copy over pixel vals (avoid values on the seam), height = og height - 1 pass
// in seam as parameters int* seam = GetVerticalSeam() helpers -- return a new
// instance of image ppm (creating new image ppm obj, set pixels data to new
// pixels array) destroy old data within current image obj (call image
// destructor on image_) -- not in helpers set image_ to new image that u get
// after removing removing horiz seam to be able to directly access the pixels

void SeamCarver::RemoveHorizontalSeam() {
  int* seam = GetHorizontalSeam();
  auto* pixel = new Pixel*[height_ - 1];
  for (int row = 0; row < height_ - 1; row++) {
    pixel[row] = new Pixel[width_];
  }
  for (int col = 0; col < width_; col++) {
    int new_row = 0;
    for (int row = 0; row < height_; row++) {
      if (seam[col] == row) {
        new_row++;
      } else {
        pixel[row - new_row][col] = image_.GetPixel(row, col);
      }
    }
  }
  image_.RemoveVerticalSeam(pixel, height_ - 1, width_);
  height_ = height_ - 1;
  delete[] seam;
}
// ImagePPM new_image(width_, height_ - 1, image_.GetMaxColorValue());  //
// Create a new image by copying the original one

// // Remove seam from new image

// for (int row = 0; row < height_; ++row) {
//   pixel[row] = new Pixel*[width_];
//   for (int col  = 0; col < width_ - 1; ++col) {
//     if (col == seam[row]) {
//       ++col;  // Skip the column in the seam
//     }
//     new_image.pixels_[row][col] = image_.GetPixel(row, col);
//   }
// }

// // Update the image and its dimensions
// SetImage(new_image);
// width_ = new_image.GetWidth();

void SeamCarver::RemoveVerticalSeam() {
  int* seam = GetVerticalSeam();
  auto* pixel = new Pixel*[height_];
  for (int row = 0; row < height_; row++) {
    pixel[row] = new Pixel[width_ - 1];
  }

  for (int row = 0; row < height_; row++) {
    int new_col = 0;
    for (int col = 0; col < width_; col++) {
      if (seam[row] == col) {
        new_col++;
      } else {
        pixel[row][col - new_col] = image_.GetPixel(row, col);
      }
    }
  }
  image_.RemoveVerticalSeam(pixel, height_, width_ - 1);
  width_ = width_ - 1;
  delete[] seam;
}
// ImagePPM new_image =
//     image_;  // Create a new image by copying the original one
// // double pointer pixels?

// // Remove seam from new image
// // flip cols and rows, and seams
// for (int row = 0; row < height_ - 1; ++row) {
//   int seam_col = seam[row];
//   for (int col = 0; col < width_; ++col) {
//     int seam_row = seam[col];
//     // check if current
//     // path of a col will give u a row
//     // for pixel in row
//     //
//     if (row > seam_col) {
//       // move it all up
//       new_image.pixels_[row][col] = image_.GetPixel(row + 1, col);
//     } else {
//       new_image.pixels_[row][col] = image_.GetPixel(row, col);
//     }
//   }
// }

// Update the image and its dimensions
//   SetImage(new_image);
//   height_ = new_image.GetHeight();
// }

std::ostream& operator<<(std::ostream& os, const ImagePPM& image) {
  os << "P3" << std::endl;
  os << image.GetWidth() << " " << image.GetHeight() << std::endl;
  os << image.GetMaxColorValue() << std::endl;
  for (int row = 0; row < image.GetHeight(); ++row) {
    for (int col = 0; col < image.GetWidth(); ++col) {
      Pixel pixel = image.GetPixel(row, col);
      os << pixel.GetRed() << std::endl;
      os << pixel.GetGreen() << std::endl;
      os << pixel.GetBlue() << std::endl;
    }
  }
  return os;
}