#include "illini_book.hpp"

#include <fstream>

// Your code here!
IlliniBook::IlliniBook(const std::string& people_fpath,
                       const std::string& relations_fpath) {
  // open file
  std::ifstream ppl(people_fpath);
  // check if open?
  if (!ppl.is_open()) {
    throw std::runtime_error("couldnt open ppl file");
  }
  // read in uin -- 1st csv file
  int uin = 0;
  while (ppl >> uin) {
    // populate
    book_[uin] = std::list<std::pair<int, std::string>>();
  }
  ppl.close();

  // 2nd csv file & do same thing
  std::ifstream relationships_list(relations_fpath);
  if (!relationships_list.is_open()) {
    throw std::runtime_error("couldnt open relationships file");
  }
  char comma = 0;
  int uin_first = 0;
  int uin_second = 0;
  std::string relationship;
  while (relationships_list >> uin_first >> comma >> uin_second >> comma >>
         relationship) {
    book_[uin_first].push_back({uin_second, relationship});
    book_[uin_second].push_back({uin_first, relationship});
  }
  relationships_list.close();
}

bool IlliniBook::AreRelated(int uin_1, int uin_2) const {
  return GetRelated(uin_1, uin_2) != -1;
}

bool IlliniBook::AreRelated(int uin_1,
                            int uin_2,
                            const std::string& relationship) const {
  return GetRelated(uin_1, uin_2, relationship) != -1;
}

int IlliniBook::GetRelated(int uin_1, int uin_2) const {
  // bfs
  std::queue<int> queue;
  std::set<int> visited;
  queue.push(uin_1);
  visited.insert(uin_1);
  int distance = 0;
  while (!queue.empty()) {
    auto size = queue.size();
    // nodes at curr level
    for (unsigned long i = 0; i < size; ++i) {
      int curr = queue.front();
      queue.pop();
      // reached uin_2?
      if (curr == uin_2) {
        // shorted path
        return distance;
      }
      // neighbors of curr node
      for (const auto& neighbor : book_.at(curr)) {
        // use .count() to check if uin exists in book_
        if (visited.count(neighbor.first) == 0) {
          queue.push(neighbor.first);
          visited.insert(neighbor.first);
        }
      }
    }
    distance++;
  }
  return -1;
}
// std::map<int, std::list<std::pair<int, std::string>>> book_;
int IlliniBook::GetRelated(int uin_1,
                           int uin_2,
                           const std::string& relationship) const {
  // bfs
  std::queue<int> queue;
  std::set<int> visited;
  queue.push(uin_1);
  visited.insert(uin_1);
  int distance = 0;
  while (!queue.empty()) {
    auto size = queue.size();
    // nodes at curr level
    for (unsigned long i = 0; i < size; ++i) {
      int curr = queue.front();
      queue.pop();
      // reached uin_2?
      if (curr == uin_2) {
        return distance;
      }
      // neighbors of curr node
      for (const auto& neighbor : book_.at(curr)) {
        // use .count() to check if uin exists in book_
        if (visited.count(neighbor.first) == 0 &&
            GetRelatedHelper(neighbor.first, curr, relationship)) {
          queue.push(neighbor.first);
          visited.insert(neighbor.first);
        }
      }
    }
    distance++;
  }
  return -1;
}
// bool helper
bool IlliniBook::GetRelatedHelper(int uin_1,
                                  int curr,
                                  const std::string& relationship) const {
  for (auto const& pair : book_.at(uin_1)) {
    int uin = pair.first;
    std::string str = pair.second;
    if (uin == curr) {
      return str == relationship;
    }
  }
  return false;
}
std::vector<int> IlliniBook::GetSteps(int uin, int n) const {
  // bfs
  // pair of node and distance;
  std::queue<std::pair<int, int>> queue;
  std::set<int> visited;
  std::vector<int> steps;
  // start w initial node and 0 distance
  queue.push({uin, 0});
  visited.insert(uin);
  // int distance = 0;
  // bfs
  while (!queue.empty()) {
    // get curr node and distance from front of queue
    auto [curr, distance] = queue.front();
    // auto size = queue.size();
    queue.pop();
    // for (unsigned long i = 0; i < size; ++i) {
    //   int curr = queue.front();
    //   queue.pop();
    // curr node at distance n?
    if (distance == n) {
      steps.push_back(curr);
    } else if (distance < n) {
      for (const auto& neighbor : book_.at(curr)) {
        if (visited.count(neighbor.first) == 0) {
          queue.push({neighbor.first, distance + 1});
          visited.insert(neighbor.first);
        }
      }
    }
    // neighbors of curr node
    //   for (const auto& neighbor : book_.at(curr)) {
    //     queue.push({neighbor.first, distance + 1});
    //     visited.insert(neighbor.first);
    //   }
    // }
  }
  return steps;
}
size_t IlliniBook::CountGroups() const {
  std::set<int> visited;
  size_t count = 0;

  // all nodes
  for (const auto& entry : book_) {
    int uin = entry.first;
    if (visited.count(uin) == 0) {
      count++;
      std::queue<int> queue;
      queue.push(uin);
      visited.insert(uin);

      // bfs
      while (!queue.empty()) {
        int curr = queue.front();
        queue.pop();
        // neighbors
        for (const auto& neighbor : book_.at(curr)) {
          if (visited.count(neighbor.first) == 0) {
            queue.push(neighbor.first);
            visited.insert(neighbor.first);
          }
        }
      }
    }
  }
  return count;
}

size_t IlliniBook::CountGroups(const std::string& relationship) const {
  std::set<int> visited;
  size_t count = 0;
  // all nodes in graph
  for (const auto& entry : book_) {
    int uin = entry.first;
    if (visited.count(uin) == 0) {
      count++;
      std::queue<int> queue;
      queue.push(uin);
      visited.insert(uin);
      // bfs
      while (!queue.empty()) {
        int curr = queue.front();
        queue.pop();
        // neighbors of curr node w relationship
        for (const auto& neighbor : book_.at(curr)) {
          if (visited.count(neighbor.first) == 0 &&
              neighbor.second == relationship) {
            queue.push(neighbor.first);
            visited.insert(neighbor.first);
          }
        }
      }
    }
  }
  return count;
}

size_t IlliniBook::CountGroups(
    const std::vector<std::string>& relationships) const {
  std::set<int> visited;
  size_t count = 0;

  // set -> store the allowed relationships
  std::set<std::string> allowed_relationships(relationships.begin(),
                                              relationships.end());

  // iterate -> all nodes in graph
  for (const auto& entry : book_) {
    int uin = entry.first;
    // the node not been visited yet -> bfs
    if (visited.count(uin) == 0) {
      count++;
      std::queue<int> queue;
      queue.push(uin);
      visited.insert(uin);

      // bfs -> only edges w specified relationships
      while (!queue.empty()) {
        int current = queue.front();
        queue.pop();
        // neighbors of current node w  specified relationships
        for (const auto& neighbor : book_.at(current)) {
          if (visited.count(neighbor.first) == 0 &&
              allowed_relationships.count(neighbor.second) > 0) {
            queue.push(neighbor.first);
            visited.insert(neighbor.first);
          }
        }
      }
    }
  }
  return count;
}