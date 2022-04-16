#pragma once
#include "commons.h"



class Button {
public:
	sf::Vector2i pos;		//from 0 to 1000      in % of the max size (1000)
	sf::Vector2i size;		//from 0 to 1000      in % of the max size (1000)

	sf::IntRect rectScreen;

	std::string text;
	sf::Color color = sf::Color(255, 255, 255);
	sf::Vertex Image[4];

	int type; //0 = normal, 1 = filled (using Button.Image)
	std::string mode; //relative (to the biggest square in screen) or fixed (to the top left screen corner)

	Button(sf::Vector2i p, sf::Vector2i s, std::string t, std::string m, int ty) : pos(p), size(s), text(t), mode(m), type(ty) {};

	void draw(sf::RenderWindow& window);
	void update(sf::Vector2u screenSize, int menuSize);
	bool clicked(Mouse& UserMouse);

};



class Slider {
public:
	sf::Vector2f pos;		//from 0 to 1000      in % of the max size (1000)
	sf::Vector2f size;		//from 0 to 1000      in % of the max size (1000)

	sf::IntRect rectScreen;

	int maxValue;
	int minValue;

	std::string text;
	sf::Color color = sf::Color(255, 255, 255);

	std::string mode; //relative (to the biggest square in screen) or fixed (to the top left screen corner)

	Slider(sf::Vector2f p, sf::Vector2f s, std::string m, int i, int a) : pos(p), size(s), mode(m), minValue(i), maxValue(a) {};

	int value = (minValue + maxValue) / 2;

	void draw(sf::RenderWindow& window);
	void update(sf::Vector2u screenSize, int menuSize);
	bool clicked(Mouse& UserMouse);

};



class Check { //radio button
public:
	sf::Vector2i pos;		//from 0 to 1000      in % of the max size (1000)
	sf::Vector2i size;		//from 0 to 1000      in % of the max size (1000)

	sf::IntRect rectScreen;

	std::string text;
	sf::Color color = sf::Color(255, 255, 255);
	bool value;

	std::string mode; //relative (to the biggest square in screen) or fixed (to the top left screen corner)

	Check(sf::Vector2i p, sf::Vector2i s, bool v, std::string m) : pos(p), size(s), value(v), mode(m) {};

	void draw(sf::RenderWindow& window);
	void update(sf::Vector2u screenSize, int menuSize);
	bool clicked(Mouse& UserMouse);

};

