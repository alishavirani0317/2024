#include <ctime>
#include <fstream>
#include <iostream>
#include <vector>

#include "constants.hpp"
#include "group.hpp"

/*Write your main driver in this file! Remember to seperate
out things into functions to help with readability and reusability.

See specifications for how to get started and
go to office hours if you need help!*/

std::vector<Group> ReadVolunteers(const std::string& filename) {
  std::ifstream ifs(filename);
  if (!ifs.is_open()) {
    exit(EXIT_FAILURE);
  }
  std::vector<Volunteer> vols;
  std::string name;
  int building_exp = 0;
  int stamina = 0;
  int returning = 0;
  while (ifs >> name >> building_exp >> stamina >> returning) {
    Volunteer vol(name, building_exp, stamina, returning != 0);
    vols.push_back(vol);
  }
  std::vector<Group> groups;
  size_t num_groups = vols.size() / kSizeGroups;
  for (size_t i = 0; i < num_groups; ++i) {
    Group group;
    for (size_t j = 0; j < kSizeGroups; ++j) {
      group.AddVolunteer(vols[i * kSizeGroups + j]);
    }
    groups.push_back(group);
  }
  ifs.close();
  return groups;
}

bool IsValid(std::vector<Group>& groups) {
  for (size_t i = 0; i < groups.size(); ++i) {
    if (groups[i].GetAvgBuildingExp() < kMinAvgBuildingExp ||
        groups[i].GetAvgStamina() < kMinAvgStamina ||
        groups[i].GetReturningMembers() < kMinReturning) {
      return false;
    }
  }
  return true;
}

void GroupsToFile(const std::string& filename, std::vector<Group>& groups) {
  std::ofstream ofs(filename);
  if (!ofs.is_open()) {
    // do something
  }

  for (size_t i = 0; i < groups.size(); ++i) {
    const Group& group = groups[i];
    ofs << group << std::endl;
  }
  ofs.close();
}

bool Swap(std::vector<Group>& groups) {
  unsigned int count = 0;  // iterations
  bool valid = IsValid(groups);
  while (count < kMaxIterations && !valid) {
    unsigned int idx1 = rand() % groups.size();
    unsigned int idx2 = rand() % groups.size();
    while (idx1 == idx2) {
      idx2 = rand() % groups.size();
    }

    std::swap(groups[idx1].GetRandomVolunteer(),
              groups[idx2].GetRandomVolunteer());
    valid = IsValid(groups);
    count++;
  }

  std::cout << count << std::endl;

  return valid;
}

int main(int argc, char* argv[]) {
  // DO NOT EDIT THIS LINE or add another srand call.
  srand(time(nullptr));
  std::cout << (argc) << std::endl;
  std::cout << (argv[0]) << std::endl;
  // check if num of com line args is right
  if (argc != 4) {
    return 1;
  }
  std::string if_name = argv[1];
  std::string of_name = argv[3];
  std::vector<Group> groups = ReadVolunteers(if_name);
  for (size_t i = 0; i < groups.size(); ++i) {
    std::cout << groups.at(i) << std::endl;
  }
  bool valid = Swap(groups);
  std::cout << "--------------------------" << std::endl;
  for (size_t i = 0; i < groups.size(); ++i) {
    std::cout << groups.at(i) << std::endl;
  }
  std::cout << valid << std::endl;
  GroupsToFile(of_name, groups);
  return 0;
}