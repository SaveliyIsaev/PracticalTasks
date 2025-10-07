#include "troll.hpp"

Troll::Troll() : txt("img/troll.png"), time(10) {}

Troll::Troll(const Troll& t) : txt("img/troll.png"), Attacker(t), time(t.time) {}

Troll::Troll(Troll&& t) : txt("img/troll.png"), Attacker(t), time(t.time) {}

Troll::Troll(ll health, ll attack, ll time) : txt("img/troll.png"), Attacker(health, attack), time(time) {}

Troll::~Troll() {}

void Troll::draw(sf::RenderWindow& w) {
  w.draw(sf::Sprite(txt, sf::IntRect(sf::Vector2i(0, 0), sf::Vector2i(100, 100))));
}