#include "Front.hpp"

//Button
Button::Button() :font_("Caveat-VariableFont_wght.ttf") {}

Button::Button(sf::Vector2f pos, sf::Vector2f size, std::string text) : size_(size), pos_(pos), font_("Caveat-VariableFont_wght.ttf"), color_(sf::Color(150, 150, 150)), text_(text) {}
bool Button::isClicked(sf::RenderWindow &win) {
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
	text.setPosition({pos_.x + std::max(0.f, (size_.x / 30 - text_.size()) / 2) * 30, pos_.y - 5});
	text.setFillColor(text_color_);
	window.draw(shape);
	window.draw(text);
}

void openNotesWindow(Date d, Tree& notes) {
	
	std::wstring str = notes.value(d);
	sf::RenderWindow win(sf::VideoMode({ 500, 500 }), "GOOOOOOOL");
	std::vector<std::wstring> v;
	for (int i = 0; i < str.size(); ++i) {
		if (str[i] == '\n') v.push_back(L"");
		else v.back() += str[i];
	}
	sf::Font font("Caveat-VariableFont_wght.ttf");
	while (win.isOpen()) {
		while (const std::optional event = win.pollEvent()) {
			if (event->is<sf::Event::Closed>()) win.close();
			else if (auto textEntered = event->getIf<sf::Event::TextEntered>()) {
				str += textEntered->unicode;
			}
		}
		v.clear();
		v.push_back(L"");
		for (int i = 0; i < str.size(); ++i) {
			if (str[i] == '\n') v.push_back(L"");
			else v.back().push_back(str[i]);
		}
		win.clear(sf::Color::White);
		for (int i = 0; i < v.size(); ++i) {
			sf::Text text(font, v[i]);
			text.setPosition({ 30.f, i * 40.f });
			win.draw(text);
		}
		win.display();
	}
	notes.update(d, str);
	std::wcout << str << '\n';
}