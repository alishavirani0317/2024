/**
 * @file kdtree.cpp
 * Implementation of KDTree class.
 */

#include <utility>
#include <algorithm>
#include <deque>

using namespace std;

template <int Dim>
bool smallerDimVal(const Point<Dim>& first,
                                const Point<Dim>& second, int curDim)
{
    /**
     * @todo Implement this function!
     */
     if (first[curDim] == second[curDim]) {
      return first < second;
     }
     return first[curDim] < second[curDim];
}

template <int Dim>
bool shouldReplace(const Point<Dim>& target,
                                const Point<Dim>& currentBest,
                                const Point<Dim>& potential)
{
    /**
     * @todo Implement this function!
     */
     //squared distance between target and current Best
     double currDist = 0.0;
     //squared distance between target and potentia
     double potDist = 0.0;

     //use for loop to sum up squared differences across all dimensions
     for (int i = 0; i < Dim; ++i) {
      currDist += (target[i] - currentBest[i]) * (target[i] - currentBest[i]);
      potDist += (target[i] - potential[i]) * (target[i] - potential[i]);
     }
     //if potential point distance = currBest distance, break tie
     if (potDist == currDist) {
      return potential < currentBest;
     }

     return potDist < currDist;
}

template <int Dim>
KDTree<Dim>::KDTree(const vector<Point<Dim>>& newPoints){
  //check if empty
  if (newPoints.empty()) {
    root = nullptr;
    return;
  }
  //use helper to build tree, start dim 0 
  //tree size =num of points
  vector<Point<Dim>> pts = newPoints; 
  root = buildKDTree(pts, 0, pts.size() - 1, 0);
}
template <int Dim>
typename KDTree<Dim>::KDTreeNode* KDTree<Dim>::buildKDTree(vector<Point<Dim>>& points, int left, int right, int dimension) {
  //base case: no points left in range
  if (left > right) {
    return nullptr;
  }
  //find median using quickselect
  int medianIdx = (left + right) / 2;
  // use select to position the median at the correct idx?? how 
  //pass in points.begin points.end points.median into select
  select(points.begin() + left, points.begin() + right + 1, points.begin() + medianIdx, [dimension] (const Point<Dim>& a, const Point<Dim> b) {
    return smallerDimVal(a, b, dimension);
  });
  // create a new node based on median found 

  KDTreeNode* node = new KDTreeNode(points[medianIdx]);
  // build the left and right subtrees 
  node->left = buildKDTree(points, left, medianIdx - 1, (dimension + 1) % Dim);
  node->right = buildKDTree(points, medianIdx + 1, right, (dimension + 1) % Dim);
  return node; 

}
template <int Dim>
KDTree<Dim>::KDTree(const KDTree<Dim>& other) {
   //if KDtree is empty, this one should be too
   if (other.root == nullptr) {
    root = nullptr;
    size = 0;
   } else {
   // otherwise allocate & copy root node
   root = new KDTreeNode(other.root->point);
   size = other.size;
   // copy the left subtree 
      if (other.root->left != nullptr){
        root->left = new KDTreeNode(other.root->left->point);
        //copy its left and right children
        if (other.root->left->left != nullptr) {
          root->left->left = new KDTreeNode(other.root->left->left->point);
        } else {
          root->left->left = nullptr;
        }
        if (other.root->left->right != nullptr) {
          root->left->right = new KDTreeNode(other.root->left->right->point);
        } else {
          root->left->right = nullptr;
        }
      } else {
        root->left = nullptr;
      }
      //copy the right subtree
      if (other.root->right != nullptr){
        root->left = new KDTreeNode(other.root->right->point);
        //copy its left and right children
        if (other.root->right->left != nullptr) {
          root->right->left = new KDTreeNode(other.root->right->left->point);
        } else {
          root->right->left = nullptr;
        }
        if (other.root->right->right != nullptr) {
          root->right->right = new KDTreeNode(other.root->right->right->point);
        } else {
          root->right->right = nullptr;
        }
      } else {
        root->right = nullptr;
      }
    }
}

template <int Dim>
const KDTree<Dim>& KDTree<Dim>::operator=(const KDTree<Dim>& rhs) {
  //check for self assignment
  if (this != &rhs) {
    //create temp copy
    KDTree<Dim> temp(rhs);
    //swap root & size of temp obj w this one
    std::swap(root, temp.root);
    std::swap(size, temp.size);
  }
  return *this;
}

template <int Dim>
KDTree<Dim>::~KDTree() {
  clear(root);
}

//helper for destructor
template <int Dim>
void KDTree<Dim>::clear(KDTreeNode* subtroot) {
  if (subtroot == nullptr) {
    return;
  }
  clear(subtroot->left);
  clear(subtroot->right);
  delete subtroot;
}

/*
video notes
the subtree for reference in the example:
         root
      /        \  
    TL         TR
      \       /   \
    leaf    leaf  leaf

start at root node
since tree is represented by array, use 2 indices(lower & upper) to know where we are in the array
lower = lowest most idx
upper = 1 past upper most idx
median = (lower + upper - 1) / 2
upper and lower should help us know what median (as our node) we currently are at 
check target point 
once we found best possible point (curr best) in that subtree, we exit the recursive call, we do NOT call another function to back up the tree
then we go back up one node and we are back in the recursive call for this node, we check if that node is closer to the target then our current best is 
then we update accordingly (ex, making it our new current best)
when everything is fully checked we go back up to the root of the subtree and check if how its distance compared and update current best distance accordingly
it's the fact that u don't always have to go down the left subtree that makes the kdtree much more efficient that scannign through list to find closet node
*/
template <int Dim>
double KDTree<Dim>::getDistance(const Point<Dim>& a, const Point<Dim>& b) const{
  double distance = 0;
  for (int i = 0; i < Dim; ++i) {
    distance += (a[i] - b[i]) * (a[i] - b[i]);
  }
  return distance;
}
template <int Dim>
Point<Dim> KDTree<Dim>::findNNHelper(KDTreeNode* node, const Point<Dim>& query, int depth) const {
  //if we hit a leaf or theres no subtree
  // if (node == nullptr) {
  //   return Point<Dim>();
  // }
  if (node->left == nullptr && node->right == nullptr) {
    return node->point;
  }
  Point<Dim> currBest;
  KDTreeNode* firstSubtree = nullptr; 
  KDTreeNode* secondSubtree = nullptr;

    if (smallerDimVal(query, node->point, depth % Dim)) {
    // goLeft = true; 
    // query is smaller in curr dim so go to left subtree
    if(node->left != nullptr){
      currBest = findNNHelper(node->left, query, (depth + 1) % Dim);
    }
    firstSubtree = node->left;
    secondSubtree = node->right;
  } else {
    // goLeft = false;
    if (node->right != nullptr) {
      currBest = findNNHelper(node->right, query, (depth + 1) % Dim);
    } 
    firstSubtree = node->right;
    secondSubtree = node->left;

  }
 
  if (shouldReplace(query, currBest, node->point)) {
    currBest = node->point;
  }
  
  double radiusSq = getDistance(query, currBest);

  //calcs squared distance to splitting plane of curr node 
  double distance = ((query[depth % Dim] - node->point[depth % Dim])) * ((query[depth % Dim] - node->point[depth % Dim]));
  

  //if other subtree could have a closer point, search it 
  //if temp should replace curr best then set it to curr best  
  if (secondSubtree != nullptr && distance <= radiusSq) {
    Point<Dim> temp = findNNHelper(secondSubtree, query, (depth + 1) % Dim);
    if (shouldReplace(query, currBest, temp)) {
      currBest = temp; 
    } 
  }
  return currBest;
}

template <int Dim>
Point<Dim> KDTree<Dim>::findNearestNeighbor(const Point<Dim>& query) const
{
    /**
     * @todo Implement this function!
     */

    return findNNHelper(root, query, 0);
}

/*partition(pivot index):
- pull element at pivot point index from array
-put smaller elements on left 
-put larger elements on right
- replace pivot element
exact same function used in quicksort
-
*/
template <typename RandIter, typename Comparator>
RandIter partition(RandIter begin , RandIter end, RandIter pivotIdx, Comparator cmp) {
  // get value of pivot
  auto pivotVal = *pivotIdx;
  // move pivot element to right most element
  //dont forget all the derefencing!!
  auto tmp = *pivotIdx;
  *pivotIdx = *(end - 1);
  *(end - 1) = tmp;

  RandIter storeIdx = begin;
  //iterate over the range begin to end -1
  for (RandIter i = begin; i != end - 1; ++i) {
    //if curr element is less than pivot val, swap it w the store idx
    if (cmp(*i, pivotVal)) {
      //swap *i and *storeIdx
      auto tmp = *i;
      *i = *storeIdx;
      *storeIdx = tmp;
      ++storeIdx; // move forwards
    }
  }
  //move pivot element back to its final place
  tmp = *(end - 1);
  *(end - 1) = *storeIdx;
  *storeIdx = tmp;
  //return idx where pivot ended up
  return storeIdx;
}


/*
video notes
  - we will use median element instead of kth element 
  - find the kth smallest element
  - partition around kth element -- everything smaller than kth element on one side and vice versa

select(left, right, k):
- performs quick select
-start with Select(0, n, k)
- put kth smallest element in kth position
- partition around kth element
- what select is actually doing is perfoming a binary search for this kth element, do this binary search:
- take a guess at the element 
- check ur gues (using partition) and see where it ends
- if it ends up in right place then ur done, 
- ur wrong u can tell which half of that partition ur kth element will actually be in 
*/

template <typename RandIter, typename Comparator>
void select(RandIter start, RandIter end, RandIter k, Comparator cmp) {
  while (start < end) {
    //chose a pivot idx, median right?
    RandIter pivotIdx = start + (end - start) / 2;
    // partition list and get final positon of pivot
    RandIter pivotNewIdx = partition(start, end, pivotIdx, cmp);
    if (pivotNewIdx == k) {
      return; // found the k-th element
    } else if (k < pivotNewIdx) {
      end = pivotNewIdx; // look in left half
    } else {
      start = pivotNewIdx + 1; // look in right half
    }
  }   
}