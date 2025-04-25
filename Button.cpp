#include "Button.hpp"

Button::Button() : font_("Caveat-VariableFont_wght.ttf") {}
Button::Button(sf::Vector2f pos, sf::Vector2f size, std::u32string str = U"") : pos_(pos), size_(size), text_(str), font_("Caveat-VariableFont_wght.ttf") {}
bool Button::isClicked(sf::RenderWindow& win) {
	sf::Vector2i p = sf::Mouse::getPosition();
	p.x -= win.getPosition().x;
	p.y -= win.getPosition().y;
	//std::cout << (p.x - pos_.x) << ' ' << (p.y - pos_.y) << '\n';
	return (p.x - pos_.x - 8) >= 0 && (p.x - pos_.x - 8) <= size_.x && (p.y - pos_.y - 30) >= 0 && (p.y - pos_.y - 30) <= size_.y;
}
void Button::setPosition(sf::Vector2f pos) {
	pos_ = pos;
}
void Button::setSize(sf::Vector2f size) {
	size_ = size;
}

void Button::draw(sf::RenderWindow& window) {
	sf::RectangleShape shape;
	shape.setPosition(pos_);
	shape.setSize(size_);
	shape.setFillColor(color_);
	sf::Text text = sf::Text(font_, text_, 30U);
	text.setPosition({ pos_.x + std::max(0.f, (size_.x / 30 - text_.size()) / 2) * 30, pos_.y - 5 });
	text.setFillColor(text_color_);
	window.draw(shape);
	window.draw(text);
}
