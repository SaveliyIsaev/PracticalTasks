#pragma once

#include "attacker.hpp"
#include <SFML/Graphics.hpp>
class Troll : public Attacker {
 public:
  Troll();
  Troll(const Troll&);
  Troll(Troll&&);
  Troll(ll, ll, ll);
  ~Troll();
 protected:
  ll time;
};

#include "troll.cpp"