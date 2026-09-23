#pragma once

#include <iostream>
#include <vector>

using namespace std;

// Overload << for vector
template <typename S>
ostream &operator<<(ostream &os, const vector<S> &vector) {

  // Printing all the elements using <<
  for (auto i : vector)
    os << i << " ";
  return os;
}
