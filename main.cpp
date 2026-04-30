#include "BigNum.hpp"

int main() {
    for (;;) {
        std::string s;
        ll p1, p2;
        std::cin >> s >> p1 >> p2;
        std::cout << convert(s, p1, p2) << std::endl;
    }
}