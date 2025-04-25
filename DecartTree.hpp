struct Tree {
public:
    std::mt19937 rnd;
    struct Node {
        Date key_;
        std::wstring val_;
        Node* l_ = nullptr, * r_ = nullptr;
        Node();
        Node(Date key);
        ~Node();
    } *root = nullptr;
    Node* merge(Node* lx, Node* rx);
    void split(Node* n, Date d, Node*& lx, Node*& rx, ll);
    void erase(Date d);
    Node* update(Date d, std::wstring str);
    std::wstring value(Date d);
    Tree() {}
};
