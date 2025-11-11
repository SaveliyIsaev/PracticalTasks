#include "BST.hpp"

class Splay : virtual public BST {
public:
  virtual void insert(ll x) override {
    Node *lx, *rx;
    split(root, x, lx, rx);
    if (rx && rx->key == x) {
      rx->lx = lx;
      root = rx;
      return;
    }
    Node *n = new Node(x);
    n->lx = lx;
    n->rx = rx;
    root = n;
  }
  virtual void erase(ll x) override {
    if (!root) return;
    splay(root, x);
    if (root->key != x) return;
    Node *t = root;
    root = merge(root->lx, root->rx);
    t->lx = t->rx = 0x0;
    delete t;
  }
  virtual std::pair<BST*, BST*> split(ll x) override {
    Node *lx, *rx;
    split(root, x, lx, rx);
    Splay* lt = new Splay(lx), *rt = new Splay(rx);
    root = merge(lx, rx);
  }
  virtual std::pair<ll, std::string> getInfo() override {
    if (!root) return {0, ""};
    return {root->key, std::to_string(root->key) + "0"};
  }
  virtual BST* rightSubtree() override {
    if (!root) return new Splay();
    return new Splay(root->rx);
  }
  virtual BST* leftSubtree() override {
    if (!root) return new Splay();
    return new Splay(root->lx);
  }
  virtual bool empty() override {
    return !root;
  }
  Splay() {}
  ll next(ll x) {
    Node *lx, *rx;
    split(root, x + 1, lx, rx);
    if (!rx) x = -1e18;
    else {
      splaynear(rx, mn(rx));
      x = rx->key;
    }
    root = merge(lx, rx);
    return x;
  }
  ll prev(ll x) {
    Node *lx, *rx;
    split(root, x, lx, rx);
    if (!lx) x = 1e18;
    else {
      splay(lx, mx(lx));
      x = lx->key;
    }
    root = merge(lx, rx);
    return x;
  }
  bool check(ll x) {
    return splay(root, x);
  }
private:
  struct Node {
    ll key;
    Node *lx = 0x0, *rx = 0x0;
    Node() {}
    Node (ll x) : key(x) {}
    Node(Node& n) : key(n.key) {
      if (n.lx) lx = new Node(*n.lx);
      if (n.rx) rx = new Node(*n.rx);
    }
    Node(Node&& n) {
      key = n.key;
      std::swap(lx, n.lx);
      std::swap(rx, n.rx);
    }
    Node& operator=(Node& n) {
      key = n.key;
      if (n.lx) lx = new Node(*n.lx);
      if (n.rx) rx = new Node(*n.rx);
      return *this;
    }
    Node& operator=(Node&& n) {
      key = n.key;
      std::swap(lx, n.lx);
      std::swap(rx, n.rx);
      return *this;
    }
    ~Node() {
      if (lx) {
        delete lx;
        lx = 0x0;
      }
      if (rx) {
        delete rx;
        rx = 0x0;
      }
    }
  } *root = 0x0;
  void left_rotate(Node*& n) {
    Node* a = n, *b = n->lx;
    n = b;
    a->lx = b->rx;
    b->rx = a;
  }
  void right_rotate(Node*& n) {
    Node* a = n, *b = n->rx;
    n = b;
    a->rx = b->lx;
    b->lx = a;
  }
  void left_big_rotate(Node*& n) {
    Node* a = n, *b = n->lx, *c = n->lx->rx; 
    n = c;
    a->lx = c->rx;
    b->rx = c->lx;
    c->lx = b;
    c->rx = a;
  }
  void right_big_rotate(Node*& n) {
    Node *a = n, *b = n->rx, *c = n->rx->lx;
    n = c;
    a->rx = c->lx;
    b->lx = c->rx;
    c->lx = a;
    c->rx = b;
  }
  bool splaynear(Node *&n, ll x) {
    if (!n) return 0x0;
    if (n->key == x) return 1;
    if (n->key < x) {
      if (!splaynear(n->rx, x)) return 0;
      right_rotate(n);
      return 1;
    } else {
      if (!splaynear(n->lx, x)) return 1;
      left_rotate(n);
      return 1;
    }
  }
  bool splay(Node*& n, ll x) {
    if (!n) return 0x0;
    if (n->key == x) return 1;
    if (n->key < x) {
      if (!splay(n->rx, x)) return 0;
      right_rotate(n);
      return 1;
    } else {
      if (!splay(n->lx, x)) return 0;
      left_rotate(n);
      return 1;
    }
  }
  void split(Node *&n, ll x, Node *&l, Node *&r) {
    l = r = 0x0;
    if (!n) return;
    if (!splaynear(n, x)) {
      l = n;
      return;
    }
    l = n->lx;
    n->lx = 0x0;
    r = n;
  }
  Node *merge(Node* l, Node *r) {
    if (!l) return r;
    if (!r) return l;
    splaynear(r, INT64_MIN);
    r->lx = l;
    return r;
  } 
  Splay(Node *n) {
    root = new Node(*n);
  }
  ll mn(Node* n) {
    if (!n) return 1e18;
    if (!n->lx) return n->key;
    return mn(n->lx);
  }
  ll mx(Node* n) {
    if (!n) return -1e18;
    if (!n->rx) return n->key;
    return mx(n->rx);
  }
  
};