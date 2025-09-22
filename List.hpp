#pragma once

template<typename T>
struct List {
  struct Node {
    T val;
    Node *next = nullptr, *prev = nullptr;
    Node() {}
    Node(T val_) : val(val_) {}
    ~Node() {
      delete next;
      next = prev = nullptr;
    }
   } *begin = nullptr, *last = nullptr;
  ll size = 0;
  Node *insert(Node*, T);
  Node *insert(Node*, Node*);
  Node *erase(Node*);
  List<T>& operator=(List<T>&);
  List<T>& operator=(List<T>&&);
  List<T>() {}
  List<T>(ll n) {
    for (int i = 0; i < n; ++i) insert(nullptr, new Node());
  }
  List<T>(List<T>&);
  List<T>(List<T>&&);
  ~List<T>();
};