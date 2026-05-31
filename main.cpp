#include <cstdint>
#include <set>
#include <vector>
#include <algorithm>
#include <utility>
#include <iostream>
#include <string>
#include <optional>
#include <SFML/Graphics.hpp>
#include "button.hpp"
std::vector<int> make_order(int n) {
    std::vector<int> order((1 << n) - 1);
    for (int i = 0; i < order.size(); ++i) order[i] = i + 1;
    std::sort(order.begin(), order.end(), [](int x, int y) {
        if (__builtin_popcount((uint32_t)x) == __builtin_popcount((uint32_t)y)) return x > y;
        return __builtin_popcount((uint32_t)x) < __builtin_popcount((uint32_t)y);
        });
    return order;
}

std::vector<std::vector<int>> color_the_table(int n, uint32_t f) {
    std::vector<int> order = make_order(n);
    std::vector<std::vector<int>> table(1 << n, std::vector<int>(order.size(), 0));
    uint32_t ff = f;
    for (int i = table.size() - 1; i >= 0; --i, ff /= 2) {
        if (ff & 1) continue;
        for (int& x : table[i]) x = 1;
    }
    for (int i = 0; i < order.size(); ++i) {
        std::vector<bool> used(order[i] + 1);
        for (int j = 0; j < table.size(); ++j) {
            if (table[j][i]) used[order[i] & j] = 1;
        }
        for (int j = 0; j < table.size(); ++j) {
            if (!table[j][i] && used[order[i] & j]) table[j][i] = 2;
        }
    }
    for (int i = 0; i < table.size(); ++i) {
        std::vector<int> z;
        for (int j = 0; j < order.size(); ++j) {
            if (!table[i][j]) {
                bool f = 0;
                for (int x : z) {
                    if ((x & order[j]) == x) {
                        table[i][j] = 3;
                        f = 1;
                        break;
                    }
                }
                if (!f) z.push_back(order[j]);
            }
        }
    }
    return table;
}

std::vector<std::vector<int>> get_variants(const std::vector<std::vector<int>>& table, int n) {
    std::vector<std::vector<int>> vars(table.size());
    std::vector<int> order = make_order(n);
    for (int i = 0; i < table.size(); ++i) {
        if (table[i][0] == 1) continue;
        for (int j = 0; j < order.size(); ++j) {
            if (!table[i][j]) {
                int mask = 0;
                for (int k = n - 1; k >= 0; --k) {
                    mask *= 3;
                    if (~order[j] & (1 << k)) mask += 2;
                    else mask += i >> k & 1;
                }
                vars[i].push_back(mask);
            }
        }
    }
    return vars;
}

void backtracking(int i, int n, std::vector<bool> res, std::vector<std::vector<int>>& vars, std::set<std::vector<bool>>& ans, int sz, int& min_size) {
    if (i == vars.size()) {
        if (sz < min_size) {
            min_size = sz;
            ans = { res };
        }
        else if (sz == min_size) ans.insert(res);
        return;
    }
    if (sz > min_size) return;
    if (vars[i].empty()) return backtracking(i + 1, n, res, vars, ans, sz, min_size);
    for (int x : vars[i]) {
        if (res[x]) return backtracking(i + 1, n, res, vars, ans, sz, min_size);
    }
    for (int x : vars[i]) {
        int z = 0;
        int p = x;
        for (int k = 0; k < n; ++k) {
            if (p % 3 != 2) ++z;
            p /= 3;
        }
        res[x] = 1;
        backtracking(i + 1, n, res, vars, ans, sz + z, min_size);
        res[x] = 0;
    }
}

ll fastpow(ll n, ll m) {
    ll a = 1;
    for (ll i = 1; i <= m; i <<= 1) {
        if (m & i) a *= n;
        n *= n;
    }
    return a;
}

std::string decode(int x, int n) {
    std::vector<int> d;
    for (int i = 0; i < n; ++i) {
        d.push_back(x % 3);
        x /= 3;
    }
    std::reverse(d.begin(), d.end());
    std::string s;
    for (int i = 0; i < n; ++i) {
        if (d[i] == 0) {
            s += "~";
            s.push_back('a' + i);
        }
        else if (d[i] == 1) s.push_back('a' + i);
    }
    return s;
}

void fill_the_value_buttons(ll f, std::vector<Button>& vbt) {
    for (int i = vbt.size() - 1; i >= 0; --i) {
        vbt[i].setText(std::string(1, '0' + (f & 1)));
        f /= 2;
    }
}

void fill_cells(std::vector<std::vector<sf::Text>>& cells, int n) {
    std::vector<int> order = make_order(n);
    for (int i = 0; i < cells.size(); ++i) {
        for (int j = 0; j < cells[i].size(); ++j) {
            if (i >= (1 << n) + 1 || j >= (1 << n) - 1) {
                cells[i][j].setFillColor(sf::Color(0x20, 0x20, 0x28));
                cells[i][j].setPosition({1000000, 10000000});
                continue;
            }
            cells[i][j].setFillColor(sf::Color::White);
            if (!i) {
                if (!j) {
                    cells[i][j].setString("a");
                    cells[i][j].setPosition(70, 200);
                } else {
                    cells[i][j].setPosition(cells[i][j - 1].getPosition().x + 10 + __builtin_popcount(order[j - 1]) * 20, 200);
                    std::string s;
                    int p = order[j];
                    for (int i = n - 1; i >= 0; --i) {
                        if (p & 1) s.push_back(i + 'a');
                        p >>= 1;
                    }
                    std::reverse(s.begin(), s.end());
                    cells[i][j].setString(s);
                }
            } else {
                if (!j) {
                    cells[i][j].setString(std::string(1, '0' + (i - 1 >> n - 1)));
                    cells[i][j].setPosition(70, 205 + i * 45);
                } else {
                    cells[i][j].setPosition(cells[i][j - 1].getPosition().x + 10 + __builtin_popcount(order[j - 1]) * 20, 205 + i * 45);
                    std::string s;
                    int p = order[j];
                    int k = i - 1;
                    for (int i = n - 1; i >= 0; --i) {
                        if (p & 1) s.push_back('0' + (k & 1));
                        p >>= 1;
                        k >>= 1;
                    }
                    std::reverse(s.begin(), s.end());
                    cells[i][j].setString(s);
                }
            }
        }
    }
}

void color_cells(std::vector<std::vector<int>>& table, std::vector<std::vector<sf::Text>>& cells) {
    for (int i = 0; i < table.size(); ++i) {
        for (int j = 0; j < table[i].size(); ++j) {
            if (table[i][j] == 1) cells[i + 1][j].setFillColor(sf::Color::Red);
            else if (table[i][j] == 2) cells[i + 1][j].setFillColor(sf::Color(0xff, 0x80, 0));
            else if (table[i][j] == 3) cells[i + 1][j].setFillColor(sf::Color::Yellow);
            else cells[i + 1][j].setFillColor(sf::Color::White);
        }
    }
}

void chf(int n, int &vshl, int& vshr, int& hshl, int& hshr, ll f, Button& fbt, std::vector<Button>& vbt, std::vector<std::vector<int>>& table, std::vector<std::vector<sf::Text>>& cells, std::vector<std::vector<int>>& vars, int& min_size, std::set<std::vector<bool>>& ans, std::vector<std::string>& anses) {
    vshl = hshl = vshr = hshr = 0;
    table = color_the_table(n, f);
    vars = get_variants(table, n);
    anses.clear();
    fill_the_value_buttons(f, vbt);
    fill_cells(cells, n);
    color_cells(table, cells);
    fbt.setText("f = " + std::to_string(f));
    if (f == 0 || f == (1ll << (1 << n)) - 1) {
        anses.push_back("Minimal DNF does not exist");
        return;
    }
    min_size = 1e9;
    backtracking(0, n, std::vector<bool>(fastpow(3, n + 1)), vars, ans, 0, min_size); 
    for (auto &x : ans) {
        std::string res;
        for (int i = 0; i < x.size(); ++i) {
            if (!x[i]) continue;
            res += decode(i, n) + " v ";
        }
        res.pop_back();
        res.pop_back();
        res.pop_back();
        anses.push_back(res);
    }
}

void chn(int n, int &vshl, int& vshr, int& hshl, int &hshr, std::vector<Button>& vbt, Button& fbt, ll& f, sf::Text& ntxt, std::vector<std::vector<sf::Text>>& cells, std::vector<std::vector<int>>& table, std::vector<std::vector<int>>& vars, int& min_size, std::set<std::vector<bool>>& ans, std::vector<std::string>& anses) {
    ntxt.setString("N = " + std::to_string(n));
    sf::Font font;
    font.loadFromFile("BuilderMono-Regular-400_0.otf");
    vbt.resize(1 << n);
    for (int i = 0; i < vbt.size(); ++i) {
        vbt[i].setSize({30, 40});
        vbt[i].setPos({35, 250 + i * 45});
        vbt[i].setColor(sf::Color(0x20, 0x20, 0x28));
        vbt[i].setTextColor(sf::Color::White);
    }
    chf(n, vshl, vshr, hshl, hshr, f = 1, fbt, vbt, table, cells, vars, min_size, ans, anses);
}

int main() {
    sf::RenderWindow window(sf::VideoMode(1800, 1000), "ADHD");
    std::vector<Button> nch(5);
    for (int i = 0; i < 5; ++i) {
        nch[i].setSize({30, 40});
        nch[i].setColor(sf::Color(0x2f, 0x2f, 0x3a));
        nch[i].setText(std::to_string(i + 1));
        nch[i].setTextColor(sf::Color::White);
        nch[i].setPos({40 + i * 40, 50});
    }
    sf::Font font;
    font.loadFromFile("BuilderMono-Regular-400_0.otf");
    int n = 3;
    ll f = 1;
    std::vector<Button> vbt(1 << n);
    for (int i = 0; i < vbt.size(); ++i) {
        vbt[i].setSize({30, 40});
        vbt[i].setPos({35, 250 + i * 45});
        vbt[i].setColor(sf::Color(0x20, 0x20, 0x28));
        vbt[i].setTextColor(sf::Color::White);
    }
    int vshl = 0, hshl = 0, vshr = 0, hshr = 0;
    sf::Text fltr("f", font);
    fltr.setPosition(40, 200);
    fltr.setFillColor(sf::Color::White);
    fill_the_value_buttons(f, vbt);
    Button fbt;
    fbt.setColor(sf::Color(0x20, 0x20, 0x28));
    fbt.setPos({35, 140});
    fbt.setSize({250, 40});
    bool is_entering = 0;
    fbt.setText("f = " + std::to_string(f));
    fbt.setTextColor(sf::Color::White);
    sf::Text ntxt("N = " + std::to_string(n), font);
    ntxt.setFillColor(sf::Color::White);
    ntxt.setPosition({40, 100});
    std::vector<std::vector<sf::Text>> cells((1 << 5) + 1, std::vector<sf::Text>((1 << 5) - 1, sf::Text("", font)));
    fill_cells(cells, n);
    std::vector<std::vector<int>> table = color_the_table(n, f);
    std::vector<std::vector<int>> vars = get_variants(table, n);
    int min_size = 1e9;
    std::set<std::vector<bool>> ans;
    backtracking(0, vars.size(), std::vector<bool>(800), vars, ans, 0, min_size);
    color_cells(table, cells);
    std::vector<std::string> anses;
    for (auto &x : ans) {
        std::string res;
        for (int i = 0; i < x.size(); ++i) {
            if (!x[i]) continue;
            res += decode(i, n) + " v ";
        }
        res.pop_back();
        res.pop_back();
        res.pop_back();
        anses.push_back(res);
    }
    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) window.close();
            else if (event.type == sf::Event::MouseButtonPressed) {
                if (is_entering) is_entering = 0;
                else if (fbt.isClicked(window)) is_entering = 1;
                for (int i = 0; i < vbt.size(); ++i) {
                    if (vbt[i].isClicked(window)) {
                        vbt[i].setText(std::string(1, vbt[i].getText()[0] ^ 1));
                        chf(n, vshl, vshr, hshl, hshr, f ^= 1ll << vbt.size() - 1 - i, fbt, vbt, table, cells, vars, min_size, ans, anses);
                    }
                }
                for (int i = 0; i < 5; ++i) {
                    if (nch[i].isClicked(window)) {
                        chn(n = i + 1, vshl, vshr, hshl, hshr, vbt, fbt, f, ntxt, cells, table, vars, min_size, ans, anses);
                    }
                }
            } else if (event.type == sf::Event::KeyPressed) {
                if (sf::Keyboard::isKeyPressed(sf::Keyboard::Num0) || sf::Keyboard::isKeyPressed(sf::Keyboard::Numpad0)) {
                    if (f * 10 < (1ll << (1 << n))) f *= 10;
                    chf(n, vshl, vshr, hshl, hshr, f, fbt, vbt, table, cells, vars, min_size, ans, anses);
                } else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Num1) || sf::Keyboard::isKeyPressed(sf::Keyboard::Numpad1)) {
                    if (f * 10  + 1 < (1ll << (1 << n))) f = f * 10 + 1;
                    chf(n, vshl, vshr, hshl, hshr, f, fbt, vbt, table, cells, vars, min_size, ans, anses);
                } else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Num2) || sf::Keyboard::isKeyPressed(sf::Keyboard::Numpad2)) {
                    if (f * 10 + 2 < (1ll << (1 << n))) f = f * 10 + 2;
                    chf(n, vshl, vshr, hshl, hshr, f, fbt, vbt, table, cells, vars, min_size, ans, anses);
                } else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Num3) || sf::Keyboard::isKeyPressed(sf::Keyboard::Numpad3)) {
                    if (f * 10  + 3 < (1ll << (1 << n))) f = f * 10 + 3;
                    chf(n, vshl, vshr, hshl, hshr, f, fbt, vbt, table, cells, vars, min_size, ans, anses);
                } else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Num4) || sf::Keyboard::isKeyPressed(sf::Keyboard::Numpad4)) {
                    if (f * 10  + 4 < (1ll << (1 << n))) f = f * 10 + 4;
                    chf(n, vshl, vshr, hshl, hshr, f, fbt, vbt, table, cells, vars, min_size, ans, anses);
                } else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Num5) || sf::Keyboard::isKeyPressed(sf::Keyboard::Numpad5)) {
                    if (f * 10  + 5 < (1ll << (1 << n))) f = f * 10 + 5;
                    chf(n, vshl, vshr, hshl, hshr, f, fbt, vbt, table, cells, vars, min_size, ans, anses);
                } else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Num6) || sf::Keyboard::isKeyPressed(sf::Keyboard::Numpad6)) {
                    if (f * 10  + 6 < (1ll << (1 << n))) f = f * 10 + 6;
                    chf(n, vshl, vshr, hshl, hshr, f, fbt, vbt, table, cells, vars, min_size, ans, anses);
                } else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Num7) || sf::Keyboard::isKeyPressed(sf::Keyboard::Numpad7)) {
                    if (f * 10  + 7 < (1ll << (1 << n))) f = f * 10 + 7;
                    chf(n, vshl, vshr, hshl, hshr, f, fbt, vbt, table, cells, vars, min_size, ans, anses);
                } else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Num8) || sf::Keyboard::isKeyPressed(sf::Keyboard::Numpad8)) {
                    if (f * 10  + 8 < (1ll << (1 << n))) f = f * 10 + 8;
                    chf(n, vshl, vshr, hshl, hshr, f, fbt, vbt, table, cells, vars, min_size, ans, anses);
                } else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Num9) || sf::Keyboard::isKeyPressed(sf::Keyboard::Numpad9)) {
                    if (f * 10  + 9 < (1ll << (1 << n))) f = f * 10 + 9;
                    chf(n, vshl, vshr, hshl, hshr, f, fbt, vbt, table, cells, vars, min_size, ans, anses);
                } else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Backspace) || sf::Keyboard::isKeyPressed(sf::Keyboard::Delete)) {
                    f /= 10;
                    chf(n, vshl, vshr, hshl, hshr, f, fbt, vbt, table, cells, vars, min_size, ans, anses);
                } else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up)) {
                    int mp = sf::Mouse::getPosition().x;
                    mp -= window.getPosition().x;
                    if (mp <= 870) vshl = std::max(0, vshl - 1);
                    else vshr = std::max(0, vshr - 1);
                } else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down)) {
                    int mp = sf::Mouse::getPosition().x;
                    mp -= window.getPosition().x;
                    if (mp <= 870) vshl = std::max(0, std::min((1 << n) - 14, vshl + 1));
                    else vshr = std::max(0, std::min<int>(vshr + 1, anses.size() - 20));
                }  else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left)) {
                    int mp = sf::Mouse::getPosition().x;
                    mp -= window.getPosition().x;
                    if (mp <= 870) hshl = std::max(0, hshl - 1);
                    else hshr = std::max(0, hshr - 1);
                }  else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right)) {
                    int mp = sf::Mouse::getPosition().x;
                    mp -= window.getPosition().x;
                    if (mp <= 870) hshl = std::max(0, std::min<int>(hshl + 1, cells[0][(1 << n) - 2].getPosition().x / 10 + n * 2 - 81));
                    else {
                        int mx = 0;
                        for (int i = vshr; i < std::min<int>(vshr + 20, anses.size()); ++i) mx = std::max<int>(mx, anses[i].size());
                        hshr = std::max(0, std::min(hshr + 1, mx - 40));
                    }
                } 
            }
        }
        window.clear(sf::Color(0x20, 0x20, 0x28));
        // window.draw(fltr);
        sf::Text txt = fltr;
        txt.setPosition(fltr.getPosition().x - hshl * 10, fltr.getPosition().y - vshl * 45);
        window.draw(txt);
        for(int i = 0; i < vbt.size(); ++i) {
            Button txt = vbt[i];
            txt.setPos({vbt[i].getPosition().x - hshl * 10, vbt[i].getPosition().y - vshl * 45});
            txt.draw(window);
        }
        for (int i = 0; i < cells.size(); ++i) {
            for (int j = 0; j < cells[i].size(); ++j) {
                sf::Text txt = cells[i][j];
                txt.setFillColor(cells[i][j].getFillColor());
                txt.setPosition(cells[i][j].getPosition().x - hshl * 10, cells[i][j].getPosition().y - vshl * 45);
                window.draw(txt);
                // window.draw(cells[i][j]);
            }
        }
        sf::RectangleShape shape({2, 1000});
        sf::RectangleShape rbrd({1200, 1000});
        rbrd.setFillColor(sf::Color(0x20, 0x20, 0x28));
        rbrd.setPosition({860, 0});
        sf::RectangleShape lbrd({40, 1000});
        lbrd.setFillColor(sf::Color(0x20, 0x20, 0x28));
        lbrd.setPosition({0, 0});
        sf::RectangleShape dbrd({1500, 130});
        dbrd.setFillColor(sf::Color(0x20, 0x20, 0x28));
        dbrd.setPosition(0, 870);
        sf::RectangleShape ubrd({1500, 200});
        ubrd.setPosition({0, 0});
        ubrd.setFillColor(sf::Color(0x20, 0x20, 0x28));
        shape.setFillColor(sf::Color::White);
        shape.setPosition({870, 0});
        window.draw(rbrd);
        window.draw(dbrd);
        window.draw(lbrd);
        window.draw(ubrd);
        window.draw(ntxt);
        window.draw(shape);
        fbt.draw(window);
        for (int i = 0; i < 5; ++i) nch[i].draw(window);
        for (int i = vshr; i < std::min<int>(vshr + 20, anses.size()); ++i) {
            std::string str;
            for (int j = std::max(0, std::min<int>(anses[i].size() - 40, hshr)); j < std::min<int>(anses[i].size(), std::max(0, std::min<int>(anses[i].size() - 40, hshr)) + 40); ++j) str += anses[i][j];
            sf::Text txt(str, font);
            txt.setFillColor(sf::Color::White);
            txt.setPosition({900, 50 + i * 45});
            window.draw(txt);
        }
        window.display();
    }
}
