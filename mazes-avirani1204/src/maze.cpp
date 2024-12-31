/* Your code here! */
#include "maze.h"
#include "dsets.h"
#include <vector>
#include <queue>
#include <algorithm>
#include "cs225/PNG.h"
#include "cs225/HSLAPixel.h"

SquareMaze::SquareMaze() : width_(0), height_(0){}

void SquareMaze::makeMaze(int width, int height) {
  width_ = width;
  height_ = height;
  rWalls_ = std::vector<bool>(width_ * height_, true);
  dWalls_ = std::vector<bool>(width_ * height_, true);
  dSets_.addelements(width_ * height_);
  std::vector<int> vis(width_ * height, 0);
  std::queue<int> queue;
  queue.push(0);
  vis[0] = 1;
  while(!queue.empty()) {
    int c = queue.front(); 
    queue.pop();
    int x_coord = c % width_;
    int y_coord = c / width_;
    std::vector<int> n;
    //r
    if (x_coord + 1 < width_) {
      n.push_back(c + 1);
    }
    //d
    if (y_coord +  1< height_) {
      n.push_back(c + width_);
    }
    //l
    if (x_coord - 1 >= 0) {
      n.push_back(c - 1);
    }
    //u
    if (y_coord - 1 >= 0) {
      n.push_back(c - width_);
    }
    for (int idx = n.size() - 1; idx > 0; --idx) {
      int rand_idx = rand() % (idx + 1);
      std::swap(n[idx], n[rand_idx]);
    }
    for (size_t i = 0; i < n.size(); ++i) {
      int neb = n[i]; 
      if(!vis[neb]) {
        int rC = dSets_.find(c); 
        int rN = dSets_.find(neb); 
        if (rC != rN) {
          dSets_.setunion(rC, rN);
          vis[neb] = 1; 
          queue.push(neb);
          if (neb == c + 1) {
            rWalls_[c] = false;
          } else if (neb == c + width_) {
            dWalls_[c] = false;
          } else if (neb == c - 1) {
            rWalls_[neb] = false;
          } else if (neb == c - width_) {
            dWalls_[neb] = false;
          }
        }
      }
    }
  }
}

bool SquareMaze::canTravel(int x, int y, Direction dir) const {
  if (dir == RIGHT) {
    return x + 1 < width_ && !rWalls_[width_ * y + x];
  } else if (dir == DOWN) {
    return  y + 1 < height_ && !dWalls_[width_ * y + x];
  } else if (dir == LEFT) {
    return x - 1 >= 0 && !rWalls_[(width_ * y + x) - 1];
  } else if (dir == UP) {
    return y - 1 >= 0 && !dWalls_[(width_ * y + x) - width_];
  }
  return false;
}

void SquareMaze::setWall(int x, int y, Direction dir, bool exists) {
  if (dir == RIGHT) {
    rWalls_[y * width_ + x] = exists;
  } else if (dir == DOWN) {
    dWalls_[y * width_ + x] = exists;
  }
}
std::vector<Direction> SquareMaze::solveMaze(int startX) {
  int totC = width_ * height_;
  std::vector<int> dist(totC, -1);
  std::vector<int> par(totC, -1);
  std::vector<Direction> moveF(totC, RIGHT);
  std::queue<int> q;
  int sC = startX;
  dist[sC] = 0;
  q.push(sC);
  while(!q.empty()) {
    int curr = q.front();
    q.pop();
    int x_coord = curr % width_;
    int y_coord = curr / width_;
    int neb = curr + 1;
    if (canTravel(x_coord, y_coord, RIGHT) && dist[neb] == -1) {
        dist[neb] = dist[curr] + 1; 
        par[neb] = curr;
        moveF[neb] = RIGHT;
        q.push(neb);
    }
    if (canTravel(x_coord, y_coord, DOWN)) {
        int neb = curr + width_;
        if (dist[neb] == -1) {
         dist[neb] = dist[curr] + 1;
         par[neb] = curr;
         moveF[neb] = DOWN;
         q.push(neb);
      }
    }
    if (canTravel(x_coord, y_coord, LEFT)) {
      int neb = curr - 1;
      if (dist[neb] == -1) {
         dist[neb] = dist[curr] + 1;
         par[neb] = curr;
         moveF[neb] = LEFT;
         q.push(neb);
      }
    }
    if (canTravel(x_coord, y_coord, UP)) {
      int neb = curr - width_;
      if (dist[neb] == -1) {
        dist[neb] = dist[curr] + 1;
        par[neb] = curr;
        moveF[neb] = UP;
        q.push(neb);
      }
    }
  }
  int mD = -1; 
  int dC = -1; 
  for (int i = 0; i < width_; ++i) {
      int c = (height_ - 1) * width_ + i; 
      if (dist[c] > mD || (dist[c] == mD && i < dC % width_)) {
        mD = dist[c];
        dC = c;
    }
  }
    std::vector<Direction> p; 
    int curr = dC;
    while (curr != sC) {
        p.push_back(moveF[curr]); 
        curr = par[curr];
    }
    std::reverse(p.begin(), p.end());
    return p;
}

cs225::PNG *SquareMaze::drawMaze(int start) const {
    cs225::PNG* mazeImg = new cs225::PNG(width_ * 10 + 1, height_ * 10 + 1);
    for (unsigned int i = 0; i < mazeImg->width(); ++i) {
      if (i < static_cast<unsigned int>(start * 10) || i >= static_cast<unsigned int>((start + 1) * 10)) {
        cs225::HSLAPixel& pxl = mazeImg->getPixel(i, 0);
        pxl.l = 0.0;
      }
    }
    for (unsigned int j = 0; j < mazeImg->height(); ++j) {
      cs225::HSLAPixel& pxl = mazeImg->getPixel(0, j);
      pxl.l = 0.0;
    }
    for (int x = 0; x < height_; ++x) {
      for (int y = 0; y < width_; ++y) {
        int c = y * width_ + x;
        if (rWalls_[c]) {
          for (int i = 0; i <= 10; ++i) {
            cs225::HSLAPixel& pxl = mazeImg->getPixel((x + 1) * 10, y * 10 + i);
            pxl.l = 0.0;
          }
        }
        if (dWalls_[c]) {
          for (int i = 0; i <= 10; ++i) {
            cs225::HSLAPixel& pxl = mazeImg->getPixel(x * 10 + i, (y + 1)* 10);
            pxl.l = 0.0;
          }
        }
      }
    }
  return mazeImg;
}

cs225::PNG *SquareMaze::drawMazeWithSolution(int start) {
    cs225::PNG* mazeImg = drawMaze(start);
    std::vector<Direction> sol = solveMaze(start);
    int x_coord = start * 10 + 5; 
    int y_coord = 5;
    for(Direction dir : sol) {
        // std::cout << "starting at (" << x_coord << ", " << y_coord << "), moving ";
      for (int i = 1; i <= 10; ++i) {
        // std::cout << x_coord << std::endl;
        cs225::HSLAPixel& pxl = mazeImg->getPixel(x_coord, y_coord);
        pxl.h = 0.0;
        pxl.s = 1.0;
        pxl.l = 0.5;
        pxl.a = 1.0;
        if (dir == RIGHT) {
          ++x_coord;
        } else if (dir == DOWN) {
          ++y_coord;
        } else if (dir == LEFT){
          --x_coord;
        } else if (dir == UP) {
          --y_coord;
        }
      }

      cs225::HSLAPixel& lastPxl = mazeImg->getPixel(x_coord, y_coord);
      lastPxl.h = 0.0; 
      lastPxl.s = 1.0;
      lastPxl.l = 0.5;
      lastPxl.a = 1.0;
      // std::cout << "to (" << x_coord << ", " << y_coord << ")" << std::endl;
    }
      int dX = x_coord / 10;
      int dY = (y_coord - 5) / 10;
      // std::cout << "making exit at (" << dX << ", " << dY << ")" <<
      for (int i = 1; i < 10; ++i) {
        if (static_cast<unsigned int>(dX * 10 + i) >= mazeImg->width() || static_cast<unsigned int>((dY + 1) * 10) >= mazeImg->height()) {
          break;
        }
        cs225::HSLAPixel& pxl = mazeImg->getPixel(dX * 10 + i, (dY + 1) * 10);
        pxl.l = 1.0; 
      
    }
    //return a maze png w sol path and exit drawn
    return mazeImg;
}
    
