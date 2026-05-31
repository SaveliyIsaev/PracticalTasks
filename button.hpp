#pragma once

#include <SFML/Graphics.hpp>

using ll = int64_t;
struct Button {
	sf::Font font_;
	sf::Vector2f size_ = { 0, 0 };
	sf::Vector2f pos_ = { 0, 0 };
	std::string text_ = "";
	sf::Color color_ = sf::Color(50, 50, 50), text_color_ = sf::Color::Black;
	ll text_size_ = 30;
    void setSize(sf::Vector2f sz) {
        size_ = sz;
    }
    void setPos(sf::Vector2f ps) {
        pos_ = ps;
    }
    void setText(std::string s) {
        text_ = s;
    }
    void setColor(sf::Color color) {
        color_ = color;
    }
    void setTextColor(sf::Color color) {
        text_color_ = color;
    }
    void setTextSize(ll sz) {
        text_size_ = sz;
    }
    sf::Vector2f getPosition() {
        return pos_;
    }
    sf::Vector2f getSize() {
        return size_;
    }
    std::string getText() {
        return text_;
    }
    sf::Color getColor() {
        return color_;
    }
    sf::Color getTextColor() {
        return text_color_;
    }
	ll getTextSize() {
        return text_size_;
    }
    bool isClicked(sf::RenderWindow& win);
	void draw(sf::RenderWindow& window);
	Button();
	Button(sf::Vector2f size, sf::Vector2f pos, std::string text);
};

Button::Button() {
    font_.loadFromFile("BuilderMono-Regular-400_0.otf");
}
Button::Button(sf::Vector2f pos, sf::Vector2f size, std::string str = "") : pos_(pos), size_(size), text_(str) {
    font_.loadFromFile("BuilderMono-Regular-400_0.otf");
}
bool Button::isClicked(sf::RenderWindow& win) {
	sf::Vector2i p = sf::Mouse::getPosition();
	p.x -= win.getPosition().x;
	p.y -= win.getPosition().y;
	//std::cout << (p.x - pos_.x) << ' ' << (p.y - pos_.y) << '\n';
	return (p.x - pos_.x) >= 0 && (p.x - pos_.x) <= size_.x && (p.y - pos_.y - 35) >= 0 && (p.y - pos_.y - 35) <= size_.y;
}
void Button::draw(sf::RenderWindow& window) {
	sf::RectangleShape shape;
	shape.setPosition(pos_);
	shape.setSize(size_);
	shape.setFillColor(color_);
    sf::Text text(text_, font_, text_size_);
	text.setPosition({ pos_.x + 5, pos_.y });
	text.setFillColor(text_color_);
	window.draw(shape);
	window.draw(text);
}