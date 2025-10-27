#pragma once

#include <stdint.h>
#include <utility>
using ll = long long;
class BST {
public:
  virtual void insert(ll x) {
    default_insert(root, x);
  }
  virtual void erase(ll x) {
    default_erase(root, x);
  }
protected:
  struct Node {
    ll key;
    Node *lx = nullptr, *rx = nullptr;
    Node() {}
    Node(ll x) : key(x) {}
    Node(const Node& n) : key(n.key) {
      if (n.lx) lx = new Node(*n.lx);
      if (n.rx) rx = new Node(*n.rx);
    }
    Node(Node&& n) : key(std::move(n.key)), lx(std::exchange(n.lx, nullptr)), rx(std::exchange(n.rx, nullptr)) {}
    ~Node() {
      if (lx) delete lx;
      if (rx) delete rx;
    }
    Node& operator=(const Node& n) {
      key = n.key;
      if (n.lx) lx = new Node(*n.lx);
      if (n.rx) rx = new Node(*n.rx);
      return *this;
    }
    Node& operator=(Node&& n) {
      key = std::move(n.key);
      lx = std::exchange(n.lx, 0x0);
      rx = std::exchange(n.rx, 0x0);
      return *this;
    }
  } *root = nullptr;
  virtual Node* merge(Node* l, Node*r) = 0;
  virtual void split(Node* n, ll x, Node*& l, Node*& r) = 0;
private:
  Node* default_insert(Node*& n, ll x) {
    if (!n) return n = new Node(x);
    if (n->key == x) return nullptr;
    if (n->key < x) return default_insert(n->rx, x);
    return default_insert(n->lx, x);
  }
  ll extract_minimum(Node*& n) {
    if (!n) return INT64_MIN;
    if (!n->lx) {
      ll ans = n->key;
      erase(ans);
      return ans;
    }
    return extract_minimum(n->lx);
  }
  Node* default_erase(Node*& n, ll x) {
    if (!n) return nullptr;
    if (n->key > x) {
      Node* p = default_erase(n->lx, x);
      return (p == (Node*)0x0 + 1 ? n : p);
    }
    if (n->key < x) {
      Node* p = default_erase(n->rx, x);
      return (p == (Node*)0x0 + 1 ? n : p);
    }
    if (!n->lx && !n->rx) {
      delete n;
      n = nullptr;
      return (Node*)0x0 + 1;
    }
    if (!n->lx) {
      Node *x = n->rx;
      *n = *n->rx;
      x->lx = x->rx = 0x0;
      delete x;
      return n;
    }
    if (!n->rx) {
      Node *x = n->lx;
      *n = *n->lx;
      x->lx = x->rx = 0x0;
      delete x;
      return n;
    }
    ll x = extract_minimum(n->rx);
    n->key = x;
    return n;
  }
};