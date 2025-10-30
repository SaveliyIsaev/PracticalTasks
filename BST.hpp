#pragma once

#include <stdint.h>
#include <string>
#include <utility>
using ll = long long;
class BST {
public:
  virtual std::string getInfo() = 0;
  virtual void insert(ll x) = 0;
  virtual void erase(ll x) = 0;
  virtual std::pair<BST*, BST*> split(ll x) = 0;
  virtual BST* rightSubree() = 0;
  virtual BST* leftSubtree() = 0;
};