#pragma once

#include "attacker.hpp"

class Dragon : public Attacker {
  Dragon();
  Dragon(const Dragon&);
  Dragon(Dragon&&);
  Dragon(ll, ll);
  ~Dragon();
};

#include "dragon.cpp"