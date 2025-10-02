#include "attacker.hpp"

//Constructors and Destructor
Attacker::Attacker() {}

Attacker::Attacker(ll health, ll attack) : health_(health), attack_(attack) {}

Attacker::Attacker(const Attacker& a) : health_(a.health_), attack_(a.attack_) {}

Attacker::Attacker(Attacker&& a) : health_(a.health_), attack_(a.attack_) {}

Attacker::~Attacker() {}

bool Attacker::isAlive() {
  return health_ > 0;
}

bool Attacker::attack(Attacker& a) {
  return (a.health_ -= attack_) > 0;
}