#pragma once

#include "math.cpp"

class Attacker {
 public:
  Attacker();
  Attacker(ll, ll);
  Attacker(const Attacker&);
  Attacker(Attacker&&);
  ~Attacker();

  bool isAlive();
  bool attack(Attacker& a);
 private:
  ll health_ = 0;
  ll attack_ = 0;
};

#include "attacker.cpp"