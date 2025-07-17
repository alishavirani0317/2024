#include <iostream>

#include "illini_book.hpp"
int main() {
  // instance of illini book and call constructor
  auto book = IlliniBook(
      "/workspaces/mp-7-illini-book-avirani1204/example/persons.csv",
      "/workspaces/mp-7-illini-book-avirani1204/example/relations.csv");

  // 0 is false, 1 is true
  // AreRelated is good
  // failing book.GetRelated(1, 2, 128);

  // std::cout << book.GetRelated(1, 2, "128") << std::endl;
  // // return 0;
  // getsteps
  std::vector<int> steps = book.GetSteps(9, 1);
  for (unsigned long i = 0; i < steps.size(); ++i) {
    std::cout << steps[i] << std::endl;
  }
  // //INFINITE LOOP HERE
}
