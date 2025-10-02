#include "hero.hpp"

Hero::Hero() {}

Hero::Hero(const Hero& h) : Attacker((Attacker)h), name_(h.name_) {}

Hero::Hero(Hero&& h) : Attacker((Attacker)h), name_(std::move(h.name_)) {}

Hero::Hero(ll health, ll attack, std::wstring name) : Attacker(health, attack), name_(std::move(name)) {}

Hero::Hero(Attacker& a, std::wstring name) : Attacker(a), name_(std::move(name)) {}

Hero::Hero(Attacker&& a, std::wstring name) : Attacker(std::move(a)), name_(std::move(name)) {}

Hero::~Hero() {}