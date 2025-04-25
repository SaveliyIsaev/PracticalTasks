struct String {
    char* data_ = new char[1];
    ull size_ = 0, capacity_ = 1;
    String();
    String(String& other);
    String(String&& other);
    String(const char other[]);
    String(ull size, char c);
    String operator=(String other);
    char& operator[](ll ind);
    String operator+=(char c);
    String operator+=(String str);
    operator char* () const;
    void push_back(char c);
    void pop_back();
    void insert(ll ind, char c);
    void insert(ll ind, String str);
    void erase(ll ind);
    void erase(ll lx, ll x);
    void clear();
    void reserve(ll x);
    void reallocate(ll new_capacity);
};

std::istream& operator>>(std::istream& in, String& s);
std::ostream& operator<<(std::ostream& out, String s);
