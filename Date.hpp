struct Date {
public:
    ull day_ = 0, month_ = 0, year_ = 0;
    Date();
    Date(Date& d);
    Date(Date&& d);
    Date(ull day, ull month, ull year);
    bool check();
    bool operator==(Date other);
    bool operator!=(Date other);
    bool operator<(Date other);
    bool operator>(Date other);
    bool operator<=(Date other);
    bool operator>=(Date other);
    Date operator++(int);
    Date& operator++();
    Date& operator--();
    Date operator--(int);
    Date operator=(Date other);
    operator std::string() const;
};

ll countDays(ll month, ll year);
