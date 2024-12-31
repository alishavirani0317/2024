/**
 * @file puzzle.cpp
 * Implementation of puzzle class.
 */
#include "puzzle.h"
#include <queue>
#include <map>
#include<algorithm>
PuzzleState::PuzzleState() {
    for (int i = 0; i < 15; ++i) {
        pzzlstate_[i] = i + 1;
    }
    //at 15 it should be 0
    pzzlstate_[15] = 0;
}

PuzzleState::PuzzleState(const std::array<char, 16> state){
    //just copy in array
    //vect to check freq of each tile val from 0 to 15 
    // std::vector<int> f(16, 0);
    //assume valid to begin with
    bool v = true;
    //DONT NEED TO CHECK THIS
    // for (char value : state) {
    //     if (value < 0 || value > 15) {
    //         valid = false; 
    //         break;
    //     }
    // }
    if (v) {
        //copy into puzzle state
        pzzlstate_ = state;
    } else {
        //reset to represent invalid state (all zeros)
        for (int i = 0; i < 16; ++i) {
            pzzlstate_[i] = 0;
        }
    }
}

std::array<char, 16> PuzzleState::asArray() const {
    return pzzlstate_;
}

bool PuzzleState::operator==(const PuzzleState &rhs) const {
    //go through every item in array --
     for (size_t i = 0; i < 16; ++i) {
        if (pzzlstate_[i] != rhs.pzzlstate_[i]) {
            return false;
        }
    }
    return true;

}
bool PuzzleState::operator!=(const PuzzleState &rhs) const {
    for (size_t i = 0; i < 16; ++i) {
        if (pzzlstate_[i] != rhs.pzzlstate_[i]) {
            return true;
        } 
    }
    return false;
}

 bool PuzzleState::operator<(const PuzzleState &rhs) const {
    for (size_t i = 0; i < 16; ++i) {
        if (pzzlstate_[i] < rhs.pzzlstate_[i]) {
            return true;
        } else if (pzzlstate_[i] > rhs.pzzlstate_[i]) {
            return false;
        }
    }
    return false;
}


PuzzleState PuzzleState::getNeighbor(Direction direction) const {
    //copy of curr state ->mod as neb
    PuzzleState neb(*this);
    //idx of empty tile 
    int idx = 0;

    // find where(pos) of empty tile(0)
    for (int i = 0; i < 16; ++i) {
        if (pzzlstate_[i] == 0) {
            idx = i;
            break;
        }
    }
    //curr row and cols of empty tile
    int r = idx / 4; 
    int c = idx % 4;

    // change row & col based on dirs
    if (direction == Direction::UP){
        r++;
    } else if (direction == Direction::DOWN) {
        r--;
    } else if (direction == Direction::LEFT) {
        c++;
    }else if (direction == Direction::RIGHT) {
        c--;
    }
    //edge case handling
    // check if new position is in bounds
    if (r >= 0 && r < 4 && c >= 0 && c < 4) {
        //newIdx in array
        int newIdx = r * 4 + c;
        //swap tiles
        std::swap(neb.pzzlstate_[idx], neb.pzzlstate_[newIdx]);
    } else {
        // reset neb to invalid state (all zeros)
        for (int i = 0; i < 16; ++i) {
            neb.pzzlstate_[i] = 0;
        }
    }

    return neb;
}



std::vector<PuzzleState> PuzzleState::getNeighbors() const {
    //vector -> valid neb states
    std::vector<PuzzleState> nebs;
    int i = 0;
    //loop through all 4 dirs 
    while (i < 4) {
        //-> get pzzl state from moving in certain dir
        Direction d = static_cast<Direction>(i);
        PuzzleState neb = getNeighbor(d);
        //check if result neb is valid -> add neb to list of nebs
        if (neb.pzzlstate_ != std::array<char, 16>{0}) {
            nebs.push_back(neb);
        }
        //move on to next dir
        ++i;
    }
    return nebs;
}

int PuzzleState::manhattanDistance(const PuzzleState desiredState) const {
     int d = 0;
    //go through each tile in curr state
    for (int i = 0; i < 16; ++i) {
        // Skip the empty tile
        if (pzzlstate_[i] != 0) { 
            int r = i / 4;
            int c = i % 4;
            // get the wanted position of curr tile
            int idx = -1;
            for (int j = 0; j < 16; ++j) {
                if (desiredState.pzzlstate_[j] == pzzlstate_[i]) {
                    //storing idx of tile in wanted state
                    idx = j;
                    break;
                }
            }

            int wantedR = idx / 4;
            int wantedC = idx % 4;
            //add manhat dist for curr tile
            d += std::abs(r - wantedR) + std::abs(c - wantedC);
        }
    }
    return d;
}

std::vector<PuzzleState> solveBFS(const PuzzleState &startState, const PuzzleState &desiredState, size_t *iterations) {
    //initialize iteration counter if needed
    if (iterations) {
        *iterations = 0;
    }

    // track path to reconstruct it later
    std::map<std::array<char, 16>, std::array<char, 16>> track;
    // keep track of visited states
    std::map<std::array<char, 16>, bool> v;
    std::queue<PuzzleState> bfs;
    // vect to store final solution path
    std::vector<PuzzleState> p;
    // add starting state -> queue & mark it visited
    bfs.push(startState);
    v[startState.asArray()] = true;

    // bfs loop
    while (!bfs.empty()) {
        // ++ the iteration counter if needed
        if (iterations) {
            (*iterations)++;
        }
        // get the curr state from front of queue
        PuzzleState curr = bfs.front();
        bfs.pop();
        // If curr state -> match desired state, reconstruct path
        if (curr == desiredState) {
            auto currentArr = curr.asArray();
            auto startArr = startState.asArray();
            // reconstruct path -> tracing back thru track map
            while (currentArr != startArr) {
                p.push_back(PuzzleState(currentArr));
                currentArr = track[currentArr];
            }
            // add start state to path & reverse path -> get correct order
            p.push_back(startState);
            std::reverse(p.begin(), p.end());
            return p;
        }
        // explore all neighbors of the curr state
        for (const PuzzleState &neb : curr.getNeighbors()) {
            //get array rep of neb
            auto nebArr = neb.asArray(); 
            // if neb not visited -> add to queue
            if (!v[nebArr]) {
                bfs.push(neb);
                //mark as visited
                v[nebArr] = true; 
                //keep track of parent state
                track[nebArr] = curr.asArray();
            }
        }
    }
    // empty vector
    return {};
}

//reasons its failing 
//fixed operators but did not help
std::vector<PuzzleState> solveAstar(const PuzzleState &startState, const PuzzleState &desiredState, size_t *iterations) {
    //lambda?? idk if this is right? -> use it for min-heap comparison based on fScore
    struct Compare {
        bool operator()(const std::pair<int, PuzzleState> &a, const std::pair<int, PuzzleState> &b) const {
            return a.first > b.first; 
        }
    };
    // init iteration counter if given
    if (iterations) {
        *iterations = 0;
    }
    // priority queue for A* traversal (min-heap)
    // fScore -> cost, gScore -> dist(from start) 
    std::priority_queue<std::pair<int, PuzzleState>, std::vector<std::pair<int, PuzzleState>>, Compare> bfs;
    std::map<PuzzleState, int> gScore;  
    std::map<PuzzleState, int> fScore;   
    std::map<PuzzleState, PuzzleState> cameFrom;//for reconstructing path
    std::vector<PuzzleState> path;    //vect for sol path
    // To track nodes already in the priority queue
    // std::map<PuzzleState, bool> inQueue;

    // init starting states
    //dist from start to start -> 0
    gScore[startState] = 0;
    //heuristic cost from start to goal 
    fScore[startState] = startState.manhattanDistance(desiredState);
    cameFrom[startState] = startState;
    //add start start to priority queue
    bfs.push({fScore[startState], startState});
    // inQueue[startState] = true; // mark as in priority queue

    // A* loop
    while (!bfs.empty()) {
        // ++ the iteration ctr if given
        if (iterations) {
            (*iterations)++;
        }
        // std::cout <<"fScore: " << bfs.top().first << std::endl;
        // std::cout << "iteration: " << (*iterations) << std::endl;

        // get state w lowest fScore <-priority queue
        PuzzleState curr = bfs.top().second;
        bfs.pop();
        // inQueue[curr] = false; // mark as no longer in priority queue
        // std::cout << "processing node: " << curr << "with fScore: " << fScore[curr] << std::endl;
        // if curr state matches desired state -> reconstruct path
        if (curr == desiredState) {
            // std::cout << "ending bfs" << std::endl;
            // reconstruct path -> tracing back thu cameFrom map
            while (curr != startState) {
                // std::cout << "adding to path " << std::endl;
                path.push_back(curr);
                curr = cameFrom[curr];
            }
            // add start state to path & reverse 
            path.push_back(startState);
            std::reverse(path.begin(), path.end());
            return path;
        }

        // go thru all neighbors of curr state
        for (PuzzleState& neb : curr.getNeighbors()) {
            // dist to neb
            int potCost = gScore[curr] + 1;
            // std::cout << "neighbors potCost: " << potCost << std::endl;
            // std::cout << "neighbors gScore: " << gScore[neb] << std::endl;
            // std::cout << "true/false: " << (gScore.find(neb) == gScore.end()) << std::endl;
            //print out actual neb and see if matches startstate all 4 times, 
            //if it does then issue in getneighbors, 
            //if not print out entire g score map before for loop, and see whats in it should only be single key value pair where key is start state, and value should be gscore should be 0, check if operators r correct
            // // for ()
            // // std::cout << 
            // std::cout << "Start State: " << startState << std::endl;
            // std::cout << "Current State: " << curr << std::endl;

            // // Iterate over neighbors to debug
            // for (const PuzzleState &neb : curr.getNeighbors()) {
            //     std::cout << "Neighbor: " << neb << std::endl;

            //     // Check if the neighbor matches the start state
            //     if (neb == startState) {
            //         std::cout << "Neighbor matches start state." << std::endl;
            //     } else {
            //         std::cout << "Neighbor does NOT match start state." << std::endl;
            //     }
            // }

            // // If no issues are identified, print out the gScore map for further debugging
            // std::cout << "gScore map before processing neighbors:" << std::endl;
            // for (const auto &entry : gScore) {
            //     std::cout << "State: " << entry.first << ", gScore: " << entry.second << std::endl;
            // }
            // if neighbor is unvisited OR shorter path is found
            //ISSUE: neb is in g score map when curr is startState
            if (gScore.find(neb) == gScore.end() || potCost < gScore[neb]) {
                // std::cout << "found valid neb" << std::endl;
                cameFrom[neb] = curr; 
                //update gScore & fScore 
                gScore[neb] = potCost;
                fScore[neb] = gScore[neb] + neb.manhattanDistance(desiredState);

                //check if already in priority queue before pushing
                // if (!inQueue[neb]) {
                    bfs.push({fScore[neb], neb}); 
                    // inQueue[neb] = true; // mark as in priority queue
                // }
            }
        }
    }
    // std::cout << "Finished A star, didn't find desired state" << std::endl;
    return {};
}