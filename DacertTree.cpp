#include "DecartTree.hpp"

Tree::Node::Node() {}
Tree::Node::Node(Date key) : key_(key) {}
Tree::Node::~Node() {}
Tree::Node* Tree::merge(Node* lx, Node* rx) {
    if (!lx) return rx;
    if (!rx) return lx;
    if (rnd() > rnd()) {
        lx->r_ = merge(lx->r_, rx);
        return lx;
    }
    else {
        rx->l_ = merge(lx, rx->l_);
        return rx;
    }
}
void Tree::split(Node* n, Date d, Node*& lx, Node*& rx, ll x = 0) {
    if (!d.day_ || !d.month_) throw std::logic_error("Wrong Date");
    if (!n) {
        lx = rx = nullptr;
        return;
    }
    std::cout << x << std::endl;
    if (n->key_ < d) {
        Node* ll = nullptr, * rr = nullptr;
        lx = n;
        split(lx->r_, d, ll, rr, x + 1);
        lx->r_ = ll;
        rx = rr;
        return;
    }
    else {
        Node* ll = nullptr, * rr = nullptr;
        rx = n;
        split(rx->l_, d, ll, rr, x + 1);
        rx->l_ = rr;
        lx = ll;
        return;
    }
}
void Tree::erase(Date d) {
    if (!d.day_ || !d.month_) throw std::logic_error("Wring Date");
    Node* less = nullptr, * eq = nullptr, * more = nullptr;
    split(root, d, less, more);
    split(more, d++, eq, more);
    root = merge(less, more);
    if (eq) delete eq;
}
Tree::Node* Tree::update(Date d, std::wstring str) {
    if (!d.day_ || !d.month_) throw std::logic_error("Wrong Date");
    Node* less = nullptr, * eq = nullptr, * more = nullptr;
    split(root, d, less, more);
    split(root, d++, eq, more);
    if (!eq) eq = new Node(d);
    eq->val_ = str;
    root = merge(less, merge(eq, more));
    return eq;
}
std::wstring Tree::value(Date d) {
    Node* less = nullptr, * eq = nullptr, * more = nullptr;
    split(root, d, less, more);
    split(more, d++, eq, more);
    std::wstring res;
    if (eq) res = eq->val_;
    root = merge(less, merge(eq, more));
    return res;
}
