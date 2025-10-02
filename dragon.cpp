#include "dragon.hpp"
#include <utility>

Dragon::Dragon() {}

Dragon::Dragon(const Dragon& h) : Attacker((Attacker)h) {}

Dragon::Dragon(Dragon&& h) : Attacker((Attacker)h) {}

Dragon::Dragon(ll health, ll attack) : Attacker(health, attack) {}

Dragon::~Dragon() {}