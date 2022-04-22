#include "buttons.h"


void write(sf::RenderWindow& window, std::string t1, sf::IntRect rect, sf::Color color) {
	//load font
	sf::Font font;
	font.loadFromFile("../fonts/BalooPaaji2.ttf");

	//create the text element
	sf::Text text(t1, font);
	text.setStyle(sf::Text::Bold);
	text.setFillColor(sf::Color(0, 0, 0, 150));

	//size and place the text element according to the space available
	text.setCharacterSize(rect.height / 2);
	sf::FloatRect textRect = text.getLocalBounds();
	text.setOrigin(textRect.left + textRect.width / 2.0f, textRect.top + textRect.height / 2.0f);

	text.setPosition(sf::Vector2f(rect.left + rect.width / 2, rect.top + rect.height / 2));
	if (t1 == "Play") {
		text.setPosition(sf::Vector2f(rect.left + rect.width / 2, rect.top + rect.height / 1.8));
	}

	//text.getLocalBounds();

	window.draw(text);
}



void Button::draw(sf::RenderWindow& window) {
	sf::RectangleShape r;
	r.setFillColor(color);
	r.setSize(sf::Vector2f(rectScreen.width, rectScreen.height));
	r.setPosition(sf::Vector2f(rectScreen.left, rectScreen.top));

	window.draw(r);

	if (Button::type == 1) {
		Button::Image[0].position = sf::Vector2f(rectScreen.left + 4, rectScreen.top + 4);
		Button::Image[1].position = sf::Vector2f(rectScreen.left + rectScreen.width - 4, rectScreen.top + 4);
		Button::Image[2].position = sf::Vector2f(rectScreen.left + rectScreen.width - 4, rectScreen.top + rectScreen.height - 4);
		Button::Image[3].position = sf::Vector2f(rectScreen.left + 4, rectScreen.top + rectScreen.height - 4);
		window.draw(Button::Image, 4, sf::Quads);
	}

	write(window, text, rectScreen);
};

void Button::update(sf::Vector2u screenSize, int menuSize) {
	if (mode == "relative") {
		sf::Vector2u center = sf::Vector2u(screenSize.x / 2, screenSize.y / 2);
		rectScreen.width = menuSize * size.x / 100;			// size of the button as px 
		rectScreen.height = menuSize * size.y / 100;		// instead of % of menuSize

		rectScreen.left = center.x - menuSize / 2 + menuSize * pos.x / 100;
		rectScreen.top = center.y - menuSize / 2 + menuSize * pos.y / 100;
	}
	else if (mode == "fixed") {
		sf::Vector2u center = sf::Vector2u(screenSize.x / 2, screenSize.y / 2);
		rectScreen.width = std::min(screenSize.x, screenSize.y) * size.x / 100;			// size of the button as px 
		rectScreen.height = std::min(screenSize.x, screenSize.y) * size.y / 100;		// instead of % of menuSize

		rectScreen.left = pos.x;
		rectScreen.top = pos.y;
	}
	else if (mode == "left") {
		sf::Vector2u center = sf::Vector2u(screenSize.x / 2, screenSize.y / 2);
		rectScreen.width = std::min(screenSize.x, screenSize.y) * size.x / 100;			// size of the button as px 
		rectScreen.height = std::min(screenSize.x, screenSize.y) * size.y / 100;		// instead of % of menuSize

		rectScreen.left = pos.x * screenSize.x / 100;								//relative to the SCREEN WIDTH (and not to the biggest square, in %)
		rectScreen.top = center.y - menuSize / 2 + menuSize * pos.y / 100;		//relative top dist (in %)
	}
};

bool Button::clicked(Mouse& UserMouse) {
	if (UserMouse.startPressing) {

		int x = rectScreen.left; int y = rectScreen.top;
		int xs = rectScreen.width; int ys = rectScreen.height;

		sf::Vector2i mousePos = UserMouse.pos;
		if (std::min(std::max(mousePos.x, x), x + xs) == mousePos.x) {  // x in rect

			//std::cout << "clicked in x"<<std::endl;
			if (std::min(std::max(mousePos.y, y), y + ys) == mousePos.y) {
				//std::cout << "Button " << text <<" clicked" << std::endl;
				return true;
			}
		}
	}
	return false;
}




void Slider::draw(sf::RenderWindow& window) {
	sf::RectangleShape r;
	r.setFillColor(sf::Color(0, 0, 0));
	r.setSize(sf::Vector2f(rectScreen.width + 6, 9));
	r.setPosition(sf::Vector2f(rectScreen.left - 3, rectScreen.top + rectScreen.height / 2 - 4.5));

	window.draw(r);

	r.setFillColor(color);
	r.setSize(sf::Vector2f(rectScreen.width, 3));
	r.setPosition(sf::Vector2f(rectScreen.left, rectScreen.top + rectScreen.height / 2 - 1.5));
	window.draw(r);

	r.setFillColor(sf::Color(0, 0, 0));
	r.setSize(sf::Vector2f(rectScreen.height, rectScreen.height));
	sf::Vector2f p;
	p.y = rectScreen.top;
	p.x = rectScreen.left + ((float)(value - minValue) / (maxValue - minValue)) * rectScreen.width - rectScreen.height/2;
	r.setPosition(p);
	window.draw(r);

	r.setFillColor(color);
	r.setSize(sf::Vector2f(rectScreen.height - 6, rectScreen.height - 6));
	p.y = rectScreen.top + 3;
	p.x = rectScreen.left + ((float)(value - minValue) / (maxValue - minValue)) * rectScreen.width - rectScreen.height / 2 + 3;
	r.setPosition(p);
	window.draw(r);
}

void Slider::update(sf::Vector2u screenSize, int menuSize) {
	if (mode == "relative") {
		sf::Vector2u center = sf::Vector2u(screenSize.x / 2, screenSize.y / 2);
		rectScreen.width = menuSize * size.x / 100;			// size of the Slider as px 
		rectScreen.height = menuSize * size.y / 100;			// instead of % of menuSize

		rectScreen.left = center.x - menuSize / 2 + menuSize * pos.x / 100;
		rectScreen.top = center.y - menuSize / 2 + menuSize * pos.y / 100;
	}
	else if (mode == "fixed") {
		sf::Vector2u center = sf::Vector2u(screenSize.x / 2, screenSize.y / 2);
		rectScreen.width = std::min(screenSize.x, screenSize.y) * size.x / 100;			// size of the Slider as px 
		rectScreen.height = std::min(screenSize.x, screenSize.y) * size.y / 100;			// instead of % of menuSize

		rectScreen.left = pos.x;
		rectScreen.top = pos.y;
	}
};

bool Slider::clicked(Mouse& UserMouse) {
	if (sf::Mouse::isButtonPressed(sf::Mouse::Button::Left)) {

		int x = rectScreen.left; int y = rectScreen.top;
		int xs = rectScreen.width; int ys = rectScreen.height;

		sf::Vector2i mousePos = UserMouse.pos;
		if (std::min(std::max(mousePos.x, x - 5), x + xs +5) == mousePos.x) {  // x in rect or near

			//std::cout << "clicked in x"<<std::endl;
			if (std::min(std::max(mousePos.y, y), y + ys) == mousePos.y) {
				float value_return = ((float)(mousePos.x - x)) / xs * (maxValue - minValue) + minValue;
				if (minValue > value_return)	value_return = minValue;
				if (maxValue < value_return)	value_return = maxValue;

				value = std::round(value_return);
				return true;
			}
		}
	}
	return false;
}




void Check::draw(sf::RenderWindow& window) {
	sf::RectangleShape r;
	r.setFillColor(sf::Color(255, 255, 255));
	r.setSize(sf::Vector2f(rectScreen.width, rectScreen.height));
	r.setPosition(sf::Vector2f(rectScreen.left, rectScreen.top));
	r.setOutlineThickness(3);
	r.setOutlineColor(sf::Color(0, 0, 0));

	window.draw(r);

	if (value) {
		sf::CircleShape c;
		c.setFillColor(sf::Color(0, 0, 0));
		c.setPosition(sf::Vector2f(rectScreen.left, rectScreen.top));
		c.setRadius(rectScreen.width / 2);

		window.draw(c);
	}

}

void Check::update(sf::Vector2u screenSize, int menuSize) {
	if (mode == "relative") {
		sf::Vector2u center = sf::Vector2u(screenSize.x / 2, screenSize.y / 2);
		rectScreen.width = menuSize * size.x / 100;			// size of the button as px 
		rectScreen.height = menuSize * size.y / 100;		// instead of % of menuSize

		rectScreen.left = center.x - menuSize / 2 + menuSize * pos.x / 100;
		rectScreen.top = center.y - menuSize / 2 + menuSize * pos.y / 100;
	}
	else if (mode == "fixed") {
		sf::Vector2u center = sf::Vector2u(screenSize.x / 2, screenSize.y / 2);
		rectScreen.width = std::min(screenSize.x, screenSize.y) * size.x / 100;			// size of the button as px 
		rectScreen.height = std::min(screenSize.x, screenSize.y) * size.y / 100;		// instead of % of menuSize

		rectScreen.left = pos.x;
		rectScreen.top = pos.y;
	}
	else if (mode == "left") {
		sf::Vector2u center = sf::Vector2u(screenSize.x / 2, screenSize.y / 2);
		rectScreen.width = std::min(screenSize.x, screenSize.y) * size.x / 100;			// size of the button as px 
		rectScreen.height = std::min(screenSize.x, screenSize.y) * size.y / 100;		// instead of % of menuSize

		rectScreen.left = pos.x * screenSize.x / 100;								//relative to the SCREEN WIDTH (and not to the biggest square, in %)
		rectScreen.top = center.y - menuSize / 2 + menuSize * pos.y / 100;		//relative top dist (in %)
	}
};

bool Check:: clicked(Mouse& UserMouse) {
	if (UserMouse.startPressing) {//sf::Mouse::isButtonPressed(sf::Mouse::Button::Left)) {

		int x = rectScreen.left; int y = rectScreen.top;
		int xs = rectScreen.width; int ys = rectScreen.height;

		sf::Vector2i mousePos = UserMouse.pos;
		if (std::min(std::max(mousePos.x, x), x + xs) == mousePos.x) {  // x in rect
			if (std::min(std::max(mousePos.y, y), y + ys) == mousePos.y) {
				value = !value;
			}
		}
	}
	return false;
}