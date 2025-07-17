#include "cord_utilities.hpp"

#include "stdexcept"
SharedPointer<Cord> ConcatCords(SharedPointer<Cord> left_cord,
                                SharedPointer<Cord> right_cord) {
  if (left_cord.Get() == nullptr || right_cord.Get() == nullptr) {
    throw std::invalid_argument("left_cord or right_cord is null");
  }
  // left cord = curr cords left child
  Cord* cord = new Cord();
  cord->left_ = left_cord;
  cord->right_ = right_cord;
  cord->length_ = left_cord->Length() + right_cord->Length();
  SharedPointer<Cord> pointer(cord);
  return pointer;
  // right cord = curr cords right child
  // update length and return the new shared pointer using cord
}

SharedPointer<Cord> SubString(SharedPointer<Cord> curr_cord,
                              unsigned int lower_idx,
                              unsigned int upper_idx) {
  if (lower_idx >= upper_idx || upper_idx > curr_cord->Length()) {
    throw std::invalid_argument("invalid lower or upper index");
  }
  if (lower_idx == 0 && upper_idx == curr_cord->Length()) {
    return curr_cord;
  }
  // current cord is leaf -- substr within leaf cord
  unsigned int substr_length = upper_idx - lower_idx;
  if (curr_cord->Left().Get() == nullptr &&
      curr_cord->Right().Get() == nullptr) {
    char* substr_data = new char[substr_length + 1];
    for (unsigned int i = lower_idx; i < upper_idx; ++i) {
      substr_data[i - lower_idx] = curr_cord->Data()[i];
    }
    substr_data[substr_length] = '\0';

    return {new Cord(substr_data)};
  }
  if (lower_idx < curr_cord->Left()->Length()) {
    auto left_substr =
        SubString(curr_cord->Left(),
                  lower_idx,
                  std::min(upper_idx, curr_cord->Left()->Length()));
    if (upper_idx <= curr_cord->Left()->Length()) {
      // if substring is fully in left child
      return left_substr;
    }
    // substring crosses into right child
    // both left and right child case:
    auto right_substr = SubString(
        curr_cord->Right(), 0, upper_idx - curr_cord->Left()->Length());
    // concat left & right substrings
    return ConcatCords(left_substr, right_substr);
  }
  // substring is within right child (not saying right child is leaf)
  return SubString(curr_cord->Right(),
                   lower_idx - curr_cord->Left()->Length(),
                   upper_idx - curr_cord->Left()->Length());
}
