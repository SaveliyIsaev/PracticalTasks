#include <vector>
#include <SFML/Graphics.hpp>
#include <fstream>
#include <iostream>
#include "attacker.hpp"
int main() {
  sf::RenderWindow window(sf::VideoMode({ 1920, 1080 }), "GOYDA");
  sf::RectangleShape rect({100, 20});
  rect.setFillColor(sf::Color::Black);
  int st = clock();
  int mxtime = 1000;
  int sz = 100;
  int time = 50 * mxtime;
  int stt = clock();
  while (window.isOpen() && clock() - stt < mxtime * 1000) {
    while (const std::optional event = window.pollEvent()) {
      if (event->is<sf::Event::Closed>())
        window.close();
      else if (event->is<sf::Event::MouseButtonPressed>()) 
        std::cout << sf::Mouse::getPosition().x - window.getPosition().x << ' ' << sf::Mouse::getPosition().y - window.getPosition().y << '\n';
      window.clear(sf::Color::White);
      rect.setSize({ceil((mxtime * 1000 - (clock() + stt)) / 1000.0 * sz / mxtime), 20});
      window.draw(rect);
      window.display();
    }
    window.clear(sf::Color::White);
    rect.setSize({ceil((mxtime * 1000 - (clock() + stt)) / 1000.0) * sz / mxtime, 20});
    std::cout << (mxtime * 1000 - (clock() + stt)) / 1000 << '\n';
    window.draw(rect);
    window.display();
  }
  std::cout << (clock() - st) / 1000;
}