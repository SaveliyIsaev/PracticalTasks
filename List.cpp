#include "List.hpp"


template<typename T>
List<T>::List(List<T>& l) {
  while (begin) erase(l.last);
  for (List<T>::Node x = l.begin; x != nullptr; x = x->next) insert(nullptr, x->val);
}

template<typename T>
List<T>::List(List<T>&& l) {
  std::swap(begin, l.begin);
  std::swap(last, l.last);
}

template<typename T>
List<T>::~List<T>() {
  delete begin;
}

template<typename T>
typename List<T>::Node* List<T>::insert(List<T>::Node *next, T val) {
  ++size;
  if (size == 1) {
    begin = last = new Node(val);
    return begin;
  }
  if (next == begin) {
    begin = new Node(val);
    begin->next = next;
    return next->prev = begin;
  }
  if (next == nullptr) {
    last->next = next = new Node(val);
    next->prev = last;
    return last = next;
  }
  Node *n = new Node(val);
  next->prev->next = n;
  n->prev = next->prev;
  n->next = next;
  return next->prev = n;
}

template<typename T>
typename List<T>::Node* List<T>::insert(Node *next, Node *n) {
  ++size;
  n->next = n->prev = nullptr;
  if (size == 1) {
    begin = last = n;
    return begin;
  }
  if (next == begin) {
    begin = n;
    begin->next = next;
    return next->prev = begin;
  }
  if (next == nullptr) {
    last->next = next = n;
    next->prev = last;
    return last = next;
  }
  next->prev->next = n;
  n->prev = next->prev;
  n->next = next;
  return next->prev = n;
}

template<typename T>
typename List<T>::Node* List<T>::erase(Node *n) {
  if (!n) return nullptr;
  --size;
  if (!size) {
    delete n;
    return begin = last = nullptr;
  }
  if (n == begin) {
    begin = begin->next;
    begin->prev = nullptr;
    n->next = nullptr;
    delete n;
    return begin;
  }
  if (n == last) {
    last = last->prev;
    last->next = nullptr;
    n->prev = nullptr;
    delete n;
    return nullptr;
  }
  n->next->prev = n->prev;
  Node *next = n->prev->next = n->next;
  n->prev = n->next = nullptr;
  delete n;
  return next;
}

template<typename T>
List<T>& List<T>::operator=(List<T>& l) {
  while (begin) erase(last);
  for (List<T>::Node* x = l.begin; x != nullptr; x = x->next) insert(nullptr, x->val);
  return *this;
}

template<typename T>
List<T>& List<T>::operator=(List<T>&& l) {
  std::swap(begin, l.begin);
  std::swap(last, l.last);
  return *this;
}