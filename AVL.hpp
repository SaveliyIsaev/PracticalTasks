#include "BST.hpp"

struct AVL : virtual public BST {
public:
  AVL() {}
  ~AVL() {
    if (root) {
      delete root;
      root = 0x0;
    }
  }
  virtual void insert(ll x) override {
    insert(root, x);
  }
  virtual void erase(ll x) override {
    erase(root, x);
  }
  virtual std::string getInfo() override {
    if (!root) return "";
    return std::to_string(root->key);
  }
  virtual std::pair<BST*, BST*> split(ll x) override {
    Node *lx, *rx;
    split(root, x, lx, rx);
    AVL *lt = new AVL(lx), *rt = new AVL(rx);
    root = merge(lx, rx);
    return {lt, rt};
  }
  virtual BST* rightSubree() override {
    if (!root) return 0x0;
    AVL *a = new AVL;
    a->root = root->rx;
    return a;
  }
  virtual BST* leftSubtree() override {
    if (!root) return 0x0;
    AVL *a = new AVL;
    a->root = root->lx;
    return a;
  }
  private:
  struct Node {
    ll key, h = 1;
    Node* lx = nullptr, *rx = nullptr;
    Node() {}
    Node(ll x) : key(x) {}
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
  AVL(Node*& n) {
    root = new Node(*n);
  }
  AVL(Node*&& n) {
    std::swap(root, n);
  }
  void insert(Node*& n, ll x) {
    if (!n) {
      n = new Node(x);
      return;
    }
    if (n->key == x) return;
    if (n->key < x) insert(n->rx, x);
    else insert(n->lx, x);
    balance(n);
  }
  void erase(Node*& n, ll x) {
    if (!n) return;
    if (n->key == x) {
      if (n->lx && n->rx) {
        ll x = extract_minimum(n->rx);
        n->key = x;
      } else if (!n->lx && !n->rx) {
        delete n;
        n = 0x0;
      } else if (n->lx) {
        Node *t = n->lx;
        *n = *n->lx;
        t->lx = t->rx = 0x0;
        delete t;
        t = 0x0;
      } else if (n->rx) {
        Node *t = n->rx;
        *n = *n->rx;
        t->lx = t->rx = 0x0;
        delete t;
        t = 0x0;
      }
    } else if (n->key < x) erase(n->rx, x);
    else erase(n->lx, x);
    balance(n);
  }
  void balance(Node*& n) {
    if (!n) return;
    if (abs((n->lx ? n->lx->h : 0) - (n->rx ? n->rx->h : 0)) > 1) {
      if ((n->lx ? n->lx->h : 0) > (n->rx ? n->rx->h : 0)) {
        if ((n->lx->rx ? n->lx->rx->h : 0) >= (n->lx->lx ? n->lx->lx->h : 0)) {
          left_big_rotate(n);
        } else {
          left_rotate(n);
        }
      } else {
        if ((n->rx->lx ? n->rx->lx->h : 0) >= (n->rx->rx ? n->rx->rx->h : 0)) {
          right_big_rotate(n);
        } else {
          right_rotate(n);
        }
      }
    }
    calc_height(n);
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
  ll extract_minimum(Node*& n) {
    if (!n) return 1e18;
    ll x = mn(n);
    erase(n, x);
    return x;
  }
  ll extract_maximum(Node*& n) {
    if (!n) return -1e18;
    ll x = mx(n);
    erase(n, x);
    return x;
  }
  Node* merge(Node* l, Node* r) {
    if (!l) return r;
    if (!r) return l;
    ll merger = extract_maximum(l);
    return merge(l, r, merger);
  }
  void split(Node*& n, ll x, Node*& l, Node*& r) {
    l = r = 0x0;
    if (!n) return;
    if (n->key == x) {
      l = n->lx;
      n->lx = 0x0;
      r = n->rx;
      n->rx = 0x0;
      insert(r, x);
      return;
    }
    if (n->key < x) {
      Node *lx, *rx;
      split(n->rx, x, lx, rx);
      n = merge(n->lx, lx, n->key);
      l = n;
      r = rx;
      return;
    } else {
      Node *lx, *rx;
      split(n->lx, x, lx, rx);
      n = merge(rx, n->rx, n->key);
      l = lx;
      r = n;
      return;
    }
  }
  Node* merge(Node* l, Node* r, ll merger) {
    if (!l) {
      insert(r, merger);
      return r;
    }
    if (!r) {
      insert(l, merger);
      return l;
    }
    if (l->h > r->h) {
      if ((l->rx ? l->rx->h : 0) >= r->h) {
        l->rx = merge(l->rx, r, merger);
      } else {
        Node* n = new Node(merger);
        n->lx = l;
        n->rx = r;
        return n;
      }
      balance(l);
      return l;
    } else if (r->h > l->h) {
      if ((r->lx ? r->lx->h : 0) >= l->h) {
        r->lx = merge(l, r->lx, merger);
      } else {
        Node* n = new Node(merger);
        n->lx = l;
        n->rx = r;
        return n;
      }
      balance(r);
      return r;
    } else {
      Node* n = new Node(merger);
      n->lx = l;
      n->rx = r;
      return n;
    }
  }
  void left_rotate(Node*& n) {
    Node* a = n, *b = n->lx;
    n = b;
    a->lx = b->rx;
    b->rx = a;
    calc_height(a);
    calc_height(b);
  }
  void right_rotate(Node*& n) {
    Node* a = n, *b = n->rx;
    n = b;
    a->rx = b->lx;
    b->lx = a;
    calc_height(a);
    calc_height(b);
  }
  void left_big_rotate(Node*& n) {
    Node* a = n, *b = n->lx, *c = n->lx->rx; 
    n = c;
    a->lx = c->rx;
    b->rx = c->lx;
    c->lx = b;
    c->rx = a;
    calc_height(a);
    calc_height(b);
    calc_height(c);
  }
  void right_big_rotate(Node*& n) {
    Node *a = n, *b = n->rx, *c = n->rx->lx;
    n = c;
    a->rx = c->lx;
    b->lx = c->rx;
    c->lx = a;
    c->rx = b;
    calc_height(a);
    calc_height(b);
    calc_height(c);
  }
  bool check(Node *n, ll x) {
    if (!n) return 0;
    if (n->key == x) return 1;
    if (n->key < x) return check(n->rx, x);
    return check(n->lx, x);
  }
  void calc_height(Node* n) {
    if (n) n->h = 1 + std::max((n->lx ? n->lx->h : 0), (n->rx ? n->rx->h : 0));
  }
};
