#include "color.hpp"

#include <stdexcept>

// Color::Color(): red_(0), green_(0), blue_(0) {}

Color::Color(int r, int g, int b): red_(r), green_(g), blue_(b) {
  // need to ensure valid color has been constructed...
  if (r < kColorValueMin || r > kColorValueMax || g < kColorValueMin ||
      g > kColorValueMax || b < kColorValueMin || b > kColorValueMax) {
    throw std::out_of_range("color component out of range");
  }
}

// do not modify
bool operator==(const Color& rhs, const Color& lhs) {
  return (rhs.Red() == lhs.Red() && rhs.Green() == lhs.Green() &&
          rhs.Blue() == lhs.Blue());
}