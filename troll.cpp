#include "troll.hpp"

Troll::Troll() : time(10) {}

Troll::Troll(const Troll& t) : Attacker(t), time(t.time) {}

Troll::Troll(Troll&& t) : Attacker(t), time(t.time) {}

Troll::Troll(ll health, ll attack, ll time) : Attacker(health, attack), time(time) {}

Troll::~Troll() {}