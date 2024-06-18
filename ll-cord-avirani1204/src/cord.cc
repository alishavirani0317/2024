#include "cord.hpp"

#include <stdexcept>

#include "cord.hpp"
#include "cord_utilities.hpp"

Cord::Cord(std::string_view sv) {
  if (sv.empty()) {
    throw std::invalid_argument("String cannot be empty.");
  }
  data_ = new char[sv.size() + 1];
  for (size_t i = 0; i < sv.size(); ++i) {
    data_[i] = sv[i];
  }
  data_[sv.size()] = '\0';
  length_ = sv.size();
  left_ = nullptr;
  right_ = nullptr;
}

Cord::~Cord() { delete[] data_; }

SharedPointer<char> Cord::ToString() const {
  // allocate memory for new string including null terminator
  // check to see if data exists
  if (data_ != nullptr) {
    char* str = new char[length_ + 1];
    for (unsigned int i = 0; i < length_; ++i) {
      str[i] = data_[i];
    }
    str[length_] = '\0';
    SharedPointer<char> output(str, kSharedPointerType::kArray);
    // make it to shared pointer then return that
    return output;
  }
  SharedPointer<char> right_str = right_->ToString();
  unsigned int right_length = right_->Length();
  SharedPointer<char> left_str = left_->ToString();
  unsigned int left_length = left_->Length();
  char* combined_str = new char[left_length + right_length + 1];
  for (unsigned int i = 0; i < left_length; ++i) {
    combined_str[i] = left_str.Get()[i];
  }
  for (unsigned int i = 0; i < right_length; ++i) {
    combined_str[i + left_length] = right_str.Get()[i];
  }
  combined_str[length_] = '\0';
  SharedPointer<char> out_str(combined_str, kSharedPointerType::kArray);
  return out_str;
}