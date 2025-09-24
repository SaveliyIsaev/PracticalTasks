#pragma once

#include <random>
#include <ctime>
#include "List.hpp"
using ll = long long;

template<typename T>
class set {
public:
    struct Node {
        T key_;
        Node *l_ = nullptr, *r_ = nullptr;
        Node(Node&& n) {
            std::swap(*this, n);
        }
        Node(Node& n) {
            key_ = n.key_;
            if (n.l_) l_ = new Node(n.l_);
            if (n.r_) r_ = new Node(n.r_);
        }
        Node(Node* n) {
            key_ = n->key_;
            if (n->l_) l_ = new Node(n->l_);
            if (n->r_) r_ = new Node(n->r_);
        }
        Node& operator=(Node& n) {
            key_ = n.key_;
            if (n.l_) l_ = new Node(n.l_);
            if (n.r_) r_ = new Node(n.r_);
            return *this;
        }
        Node& operator=(Node&& n) {
            std::swap(*this, n);
            return *this;
        }
        Node(T key) : key_(key){}
        Node() {}
        ~Node() {
            delete l_;
            delete r_;
        }
        operator List<T>() {
            List<T> l;
            List<T> l1;
            if (r_) {
                l1 = (List<T>)(*r_);
            }
            l = l1;
            l.insert(nullptr, key_);
            if (l_) {
                l1 = (List<T>)(*l_);
                l.last->next = l1.begin;
                l1.begin->prev = l.last;
                l.size += l1.size;
                l.last = l1.last;
                l1.begin = l1.last = nullptr;
            }
            return l;
        }
    };
    Node* mx(Node*);
    T mx();
    void insert(T);
    Node* find(Node*, T);
    bool find(T);
    void erase(T);
    void add(T);
    ll size();
    set<T>& operator=(set<T>& s);
    set<T>& operator=(set<T>&& s);
    set<T>() : rnd(clock()) {}
    set<T>(set<T>& s) : rnd(clock()) {
        if (!s.root) return;
        sz = s.sz;
        root = new Node(s.root);
    }
    set<T>(set<T>&& s) : rnd(clock()) {
        if (!s.root) return;
        std::swap(sz, s.sz);
        std::swap(root, s.root);
    }
    ~set<T>() {
        if (!root) return;
        delete root;
    }
    operator List<T>() {
        if (!root) return List<T>();
        return List<T>(*root);
    }
    using iterator = Node*;
    bool empty() {
        return !root;
    }
private:
    std::mt19937 rnd;
    ll sz = 0;
    Node* root = nullptr;
    Node *merge(Node*, Node*);
    void split(Node*, T, Node*&, Node*&);
};
