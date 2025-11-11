#pragma once

#include <random>
#include "BST.hpp"

std::random_device rd;
std::mt19937 rnd(rd());

class Treap : virtual public BST {
public:
  virtual void insert(ll x) override {
    Node *less, *eq, *more;
    split(root, x, less, eq);
    split(eq, x + 1, eq, more);
    if (!eq) eq = new Node(x);
    root = merge(merge(less, eq), more);
  }
  virtual void erase(ll x) override {
    Node *less, *eq, *more;
    split(root, x, less, eq);
    split(eq, x + 1, eq, more);
    if (eq) {
      delete eq;
      eq = 0x0;
    }
    root = merge(less, more);
  }
  virtual std::pair<ll, std::string> getInfo() {
    if (!root) return {0, ""};
    return {root->key, std::to_string(root->key) + "\n" + std::to_string(root->pr) + "0"};
  }
  virtual std::pair<BST*, BST*> split(ll x) override {
    Node *lx, *rx;
    split(root, x, lx, rx);
    BST *lt = new Treap(*lx), *rt = new Treap(*rx);
    root = merge(lx, rx);
    return {lt, rt};
  }
  virtual bool empty() override {
    return !root;
  }
  virtual BST* leftSubtree() override {
    if (!root) return new Treap();
    return new Treap(root->lx);
  }
  virtual BST* rightSubtree() override {
    if (!root) return new Treap();
    return new Treap(root->rx);
  }
  Treap() {}
  private:
  struct Node {
    ll key, pr;
    Node *lx = 0x0, *rx = 0x0;
    Node() : pr(rnd() % 5000) {}
    Node(ll x) : key(x), pr(rnd() % 5000) {}
    Node(Node& n) : key(n.key), pr(n.pr) {
      if (n.lx) lx = new Node(*n.lx);
      if (n.rx) rx = new Node(*n.rx);
    }
    Node(Node&& n) {
      key = n.key;
      pr = n.pr;
      std::swap(lx, n.lx);
      std::swap(rx, n.rx);
    }
    Node& operator=(Node& n) {
      key = n.key;
      pr = n.pr;
      if (n.lx) lx = new Node(*n.lx);
      if (n.rx) rx = new Node(*n.rx);
      return *this;
    }
    Node& operator=(Node&& n) {
      key = n.key;
      pr = n.pr;
      std::swap(lx, n.lx);
      std::swap(rx, n.rx);
      return *this;
    }
  } *root = 0x0;
  Treap(Node* n) {
    std::swap(root, n);
  }
  Treap(Node& n) {
    root = new Node(n);
  }
  Treap(Node&& n) {
    std::swap(*root, n);
  }
  Node *merge(Node *l, Node *r) {
    if (!l) return r;
    if (!r) return l;
    if (l->pr < r->pr) {
      l->rx = merge(l->rx, r);
      return l;
    } else {
      r->lx = merge(l, r->lx);
      return r;
    }
  }
  void split(Node *n, ll x, Node*& l, Node*& r) {
    l = r = 0x0;
    if (!n) return;
    if (n->key < x) {
      Node *lx, *rx;
      split(n->rx, x, lx, rx);
      n->rx = lx;
      l = n;
      r = rx;
    } else {
      Node *lx, *rx;
      split(n->lx, x, lx, rx);
      n->lx = rx;
      r = n;
      l = lx;
    }
  }
};