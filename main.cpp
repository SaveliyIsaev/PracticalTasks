#include <vector>
#include <SFML/Graphics.hpp>
#include <fstream>
#include "DecartTree.hpp"
#include "Date.hpp"
#include "Button.hpp"


void openNotesWindow(Date d, Tree& notes) {

    std::wstring str = notes.value(d);
    sf::RenderWindow win(sf::VideoMode({ 500, 500 }), "GOOOOOOOL");
    std::vector<std::wstring> v;
    for (int i = 0; i < str.size(); ++i) {
        if (str[i] == '\n') v.push_back(L"");
        else v.back() += str[i];
    }
    sf::Font font("Caveat-VariableFont_wght.ttf");
    while (win.isOpen()) {
        while (const std::optional event = win.pollEvent()) {
            if (event->is<sf::Event::Closed>()) win.close();
            else if (auto textEntered = event->getIf<sf::Event::TextEntered>()) {
                str += textEntered->unicode;
            }
        }
        v.clear();
        v.push_back(L"");
        for (int i = 0; i < str.size(); ++i) {
            if (str[i] == '\n') v.push_back(L"");
            else v.back().push_back(str[i]);
        }
        win.clear(sf::Color::White);
        for (int i = 0; i < v.size(); ++i) {
            sf::Text text(font, v[i]);
            text.setPosition({ 30.f, i * 40.f });
            win.draw(text);
        }
        win.display();
    }
    notes.update(d, str);
    std::wcout << str << '\n';
}

sf::Font font("Caveat-VariableFont_wght.ttf");
const int butt_size = 40;

int main() {
    Tree notes;
    std::ifstream fin("Deutsch.txt");
    std::vector<std::string> names(20);
    for (std::string& s : names) fin >> s;
    fin.close();
    ll year = 2025;
    Button year_butt({ 125 + 4 * (8 * butt_size + 10), 50 }, { 200, 60 }, std::to_string(year));
    Button incr({ 335 + 4 * (8 * butt_size + 10), 55 }, { 50, 50 }, "incr"), decr({ (105 + 4 * (8 * butt_size)), 55 }, { 50, 50 }, "decr");
    sf::RenderWindow window(sf::VideoMode({ 1920, 1080 }), "GOYDA");
    std::vector<std::vector<std::vector<sf::RectangleShape>>> shapes(3, std::vector<std::vector<sf::RectangleShape>>(4));
    std::vector<std::vector<std::vector<sf::Text>>> texts(3, std::vector<std::vector<sf::Text>>(4, std::vector<sf::Text>(0, sf::Text(font))));
    std::vector<std::vector<std::vector<std::vector<Button>>>> days(3, std::vector<std::vector<std::vector<Button>>>(4, std::vector<std::vector<Button>>(7, std::vector<Button>(6))));
    std::vector<std::vector<std::vector<Date>>> dates = Year(year);
    for (int row = 0; row < 3; ++row) {
        for (int col = 0; col < 4; ++col) {
            for (int i = 0; i < 7; ++i) {
                for (int j = 0; j < 6; ++j) {
                    days[row][col][i][j] = Button({ i * butt_size + col * (butt_size * 8.f + 10) + 28, 50 + butt_size * (j + 1) + row * (butt_size * 8.f + 10) + 3 }, { butt_size - 5, butt_size - 5 }, std::to_string(dates[row * 4 + col + 1][i][j].day_));
                    if (dates[row * 4 + col + 1][i][j].month_ != row * 4 + col + 1) {
                        days[row][col][i][j].color_ = sf::Color::White;
                        days[row][col][i][j].text_color_ = sf::Color(150, 150, 150);
                    }
                    else {
                        days[row][col][i][j].color_ = sf::Color::White;
                    }
                }
            }
        }
    }
    for (int row = 0; row < 3; ++row) {
        for (int col = 0; col < 4; ++col) {
            shapes[row][col].resize(16);
            for (int i = 0; i < 8; ++i) {
                shapes[row][col][i] = sf::RectangleShape(sf::Vector2f(butt_size * 7 + 1, 2));
                shapes[row][col][i].setFillColor(sf::Color::Black);
                shapes[row][col][i].setPosition(sf::Vector2f(col * (butt_size * 8 + 10) + 25, 50 + butt_size * i + row * (butt_size * 8 + 10)));
            }
            for (int i = 0; i < 8; ++i) {
                shapes[row][col][i + 8] = sf::RectangleShape(sf::Vector2f(2, butt_size * 7 + 1));
                shapes[row][col][i + 8].setFillColor(sf::Color::Black);
                shapes[row][col][i + 8].setPosition(sf::Vector2f(i * butt_size + col * (butt_size * 8 + 10) + 25, 50 + row * (butt_size * 8 + 10)));
            }
            texts[row][col].resize(8, sf::Text(font));
            texts[row][col][0] = sf::Text(font, names[row * 4 + col + 1]);
            texts[row][col][0].setPosition(sf::Vector2f(col * (butt_size * 8 + 10) + 25, 10 + row * (butt_size * 8 + 10)));
            texts[row][col][0].setFillColor(sf::Color::Black);
            for (int i = 1; i <= 7; ++i) {
                texts[row][col][i] = sf::Text(font, names[i + 12], 22);
                texts[row][col][i].setPosition({ col * (8 * butt_size + 10.f) + butt_size * (i - 1) + 28, row * (8 * butt_size + 10.f) + 20 + butt_size });
                texts[row][col][i].setFillColor(sf::Color::Black);
            }
        }
    }
    while (window.isOpen()) {
        while (const std::optional event = window.pollEvent()) {
            if (event->is<sf::Event::Closed>())
                window.close();
            else if (event->is<sf::Event::MouseButtonPressed>()) {
                std::cout << sf::Mouse::getPosition().x - window.getPosition().x << ' ' << sf::Mouse::getPosition().y - window.getPosition().y << '\n';
                if (incr.isClicked(window)) {
                    year = std::min((ll)2e9, year + 1);
                    year_butt.text_ = std::to_string(year);
                    dates = Year(year);
                    for (int row = 0; row < 3; ++row) {
                        for (int col = 0; col < 4; ++col) {
                            for (int i = 0; i < 7; ++i) {
                                for (int j = 0; j < 6; ++j) {
                                    days[row][col][i][j] = Button({ i * butt_size + col * (butt_size * 8.f + 10) + 28, 50 + butt_size * (j + 1) + row * (butt_size * 8.f + 10) + 3 }, { butt_size - 5, butt_size - 5 }, std::to_string(dates[row * 4 + col + 1][i][j].day_));
                                    if (dates[row * 4 + col + 1][i][j].month_ != row * 4 + col + 1) {
                                        days[row][col][i][j].color_ = sf::Color::White;
                                        days[row][col][i][j].text_color_ = sf::Color(150, 150, 150);
                                    }
                                    else {
                                        days[row][col][i][j].color_ = sf::Color::White;
                                    }
                                }
                            }
                        }
                    }
                }
                else if (decr.isClicked(window)) {
                    year = std::max(1ll, year - 1);
                    year_butt.text_ = std::to_string(year);
                    dates = Year(year);
                    for (int row = 0; row < 3; ++row) {
                        for (int col = 0; col < 4; ++col) {
                            for (int i = 0; i < 7; ++i) {
                                for (int j = 0; j < 6; ++j) {
                                    days[row][col][i][j] = Button({ i * butt_size + col * (butt_size * 8.f + 10) + 28, 50 + butt_size * (j + 1) + row * (butt_size * 8.f + 10) + 3 }, { butt_size - 5, butt_size - 5 }, std::to_string(dates[row * 4 + col + 1][i][j].day_));
                                    if (dates[row * 4 + col + 1][i][j].month_ != row * 4 + col + 1) {
                                        days[row][col][i][j].color_ = sf::Color::White;
                                        days[row][col][i][j].text_color_ = sf::Color(150, 150, 150);
                                    }
                                    else {
                                        days[row][col][i][j].color_ = sf::Color::White;
                                    }
                                }
                            }
                        }
                    }
                }
                else {
                    for (int row = 0; row < 3; ++row) {
                        for (int col = 0; col < 4; ++col) {
                            for (int i = 0; i < 7; ++i) {
                                for (int j = 0; j < 6; ++j) {
                                    if (days[row][col][i][j].isClicked(window)) openNotesWindow(dates[row * 4 + col + 1][i][j], notes);
                                }
                            }
                        }
                    }
                }
            }
        }
        window.clear(sf::Color::White);
        for (int row = 0; row < 3; ++row)
            for (int col = 0; col < 4; ++col) {
                for (int i = 0; i < 16; ++i) window.draw(shapes[row][col][i]);
                for (int i = 0; i < 8; ++i) window.draw(texts[row][col][i]);
            }
        for (int row = 0; row < 3; ++row) {
            for (int col = 0; col < 4; ++col) {
                for (int i = 0; i < 7; ++i) {
                    for (int j = 0; j < 6; ++j) {
                        days[row][col][i][j].draw(window);
                    }
                }
            }
        }
        year_butt.draw(window);
        incr.draw(window);
        decr.draw(window);
        window.display();
    }
}
