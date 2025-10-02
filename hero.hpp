#pragma once

#include <string>
#include "attacker.hpp"

class Hero : public Attacker {
  Hero();
  Hero(const Hero&);
  Hero(Hero&&);
  Hero(ll, ll, std::wstring);
  Hero(Attacker&, std::wstring);
  Hero(Attacker&&, std::wstring);
  ~Hero();
 protected:
  std::wstring name_;
};

#include "hero.cpp"