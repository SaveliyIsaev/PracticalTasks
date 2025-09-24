#pragma once

#include "set.hpp"

//private
template<typename T>
void set<T>::split(Node *t, T key, Node *&l, Node *&r) {
    if (t == nullptr) {
        l = r = nullptr;
        return;
    }
    if (t->key_ < key) {
        Node *more, *less;
        split(t->r_, key, less, more);
        t->r_ = less;
        l = t;
        r = more;
    } else {
        Node *more, *less;
        split(t->l_, key, less, more);
        t->l_ = more;
        r = t;
        l = less;
    }
}

template<typename T>
typename set<T>::Node *set<T>::merge(Node *lx, Node *rx) {
    if (lx == nullptr) return rx;
    if (rx == nullptr) return lx;
    Node *res = new Node();
    if (rnd() > rnd()) {
        *res = *lx;
        res->r_ = merge(lx->r_, rx);
    } else {
        *res = *rx;
        res->l_ = merge(lx, rx->l_);
    }
    return res;
}

template<typename T>
typename set<T>::iterator set<T>::find(set<T>::iterator v, T n) {
    if (!v) return nullptr;
    if (v->key_ == n) return v;
    if (v->key_ < n) return find(v->r_, n);
    return find(v->l_, n);
}

template<typename T>
typename set<T>::iterator set<T>::mx(set<T>::iterator n) {
    if (!n) return nullptr;
    if (n->r_ == nullptr) return n;
    return mx(n->r_);
}


//public
template<typename T>
T set<T>::mx() {
    if (!root) return T(0);
    return mx(root)->key_;
}

template<typename T>
void set<T>::insert(T key) {
    if (key == T(0)) return;
    Node *less = nullptr, *eq = nullptr, *more = nullptr;
    split(root, key, less, more);
    split(more, nxt(key), eq, more);
    if (eq) eq->key_ = key;
    else {
        eq = new Node(key);
        ++sz;
    }
    if (eq->key_ == T(0)) {
        --sz;
        eq->l_ = eq->r_ = nullptr;
        delete eq;
        eq = nullptr;
    }
    root = merge(less, merge(eq, more));
}

template<typename T>
T nxt(T key) {
    std::vector<ll> pows(26, 0);
    pows[25] = 1;
    return key * T(1, pows);
}

template<typename T>
void set<T>::add(T key) {
    Node *less = nullptr, *eq = nullptr, *more = nullptr;
    split(root, key, less, more);
    split(more, nxt(key), eq, more);
    if (eq) eq->key_ += key;
    else {
        eq = new Node(key);
        ++sz;
    }
    if (eq->key_ == T(0)) {
        --sz;
        eq->l_ = eq->r_ = nullptr;
        delete eq;
        eq = nullptr;
    }
    root = merge(less, merge(eq, more));
}

template<typename T>
bool set<T>::find(T n) {
    return find(root, n);
}

template<typename T>
void set<T>::erase(T s) {
    Node *less = nullptr, *eq = nullptr, *more = nullptr;
    split(root, s, less, more);
    split(more, nxt(s), eq, more);
    if (eq != nullptr) {
        delete eq;
        --sz;
    }
    root = merge(less, more);
}

template<typename T>
ll set<T>::size() {
    return sz;
}

template<typename T>
set<T>& set<T>::operator=(set<T>& s) {
    if (s.empty()) return (*this = set<T>());
    root = new Node(s.root);
    sz = s.sz;
    return *this;
}

template<typename T>
set<T>& set<T>::operator=(set<T>&& s) {
    std::swap(root, s.root);
    std::swap(sz, s.sz);
    return *this;
}
