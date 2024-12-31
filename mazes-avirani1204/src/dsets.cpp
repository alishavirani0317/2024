/* Your code here! */
#include "dsets.h"

//creates n unconnected root nodes at the end of the vector.
void DisjointSets::addelements(int num) {
    for (int i = 0; i < num; ++i) {
        vect.push_back(-1);
        //is negative okay?? since each new elem is a root w size 1
    }
}

//should compress paths and works as described in lecture
// make each node pt directly to root
int DisjointSets::find(int elem) {
    if (vect[elem] >= 0) {
        vect[elem] = find(vect[elem]);
        return vect[elem]; 
    } else {
        return elem;
    }
}

// 
void DisjointSets::setunion(int a, int b) {
    //union by size
    int s1 = -vect[find(a)];
    int s2 = -vect[find(b)];

    int r1 = find(a);
    int r2 = find(b);

    if (s1 >= s2) {
        vect[r2] = r1;
        vect[r1] -= s2;
    } else {
        vect[r1] = r2;
        vect[r2] -= s1;
    }
}

//return the number of nodes in the up-tree containing the element.
int DisjointSets::size(int elem) {
    return -vect[find(elem)];
}

//returns the direct parent of the node in the vector 
int DisjointSets::getValue(int elem) const {
    return vect[elem];
}

