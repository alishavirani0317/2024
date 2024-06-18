#include "group.hpp"

#include <iostream>

#include "volunteer.hpp"

void Group::AddVolunteer(const Volunteer& vol) {
  if (vol.IsReturning()) {
    returning_members_ += 1;
  }
  volunteers_.push_back(vol);
}

double Group::GetAvgBuildingExp() const {
  double sum = 0;
  double avg = 0;
  if (volunteers_.empty()) {
    return 0.0;
  }
  for (size_t i = 0; i < volunteers_.size(); ++i) {
    sum += volunteers_[i].GetBuildingExperience();
  }
  avg = sum / (double)volunteers_.size();
  return avg;
}

int Group::GetReturningMembers() const {
  int count = 0;
  for (size_t i = 0; i < volunteers_.size(); ++i) {
    if (volunteers_.at(i).IsReturning()) {
      count++;
    }
  }
  return count;
}

double Group::GetAvgStamina() const {
  double sum = 0;
  double avg = 0;
  if (volunteers_.empty()) {
    return 0.0;
  }
  for (size_t i = 0; i < volunteers_.size(); ++i) {
    sum += volunteers_[i].GetPhysicalStamina();
  }
  avg = sum / (double)volunteers_.size();
  return avg;
}

std::vector<Volunteer> Group::GetVolunteer() const { return volunteers_; }

std::ostream& operator<<(std::ostream& os, const Group& g_in) {
  // print group info
  os << "Group:" << std::endl;
  std::vector<Volunteer> vol = g_in.GetVolunteer();
  for (size_t i = 0; i < vol.size(); ++i) {
    os << vol.at(i).GetName() << std::endl;
  }
  return os;
}

Volunteer& Group::GetRandomVolunteer() {
  size_t random_idx = rand() % volunteers_.size();
  return volunteers_[random_idx];
}