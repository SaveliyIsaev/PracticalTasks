using ll = int64_t;
struct Button {
	sf::Font font_;
	sf::Vector2f size_ = { 0, 0 };
	sf::Vector2f pos_ = { 0, 0 };
	std::string text_ = "";
	sf::Color color_ = sf::Color(50, 50, 50), text_color_ = sf::Color::Black;
	ll text_size_ = 30;
	bool isClicked(sf::RenderWindow& win);
	void draw(sf::RenderWindow& window);
	void setPosition(sf::Vector2f pos);
	void setSize(sf::Vector2f size);
	Button();
	Button(sf::Vector2f size, sf::Vector2f pos, std::string text = "");
};
