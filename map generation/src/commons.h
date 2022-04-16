#pragma once

#include <SFML/Graphics.hpp> 
#include <SFML/Audio.hpp>
#include <iostream>
#include <thread>
#include <fstream>

#include <string.h>
#include <vector>
#include <array>

#include <random>
#include <cmath>

#include <time.h>


#include "noise.h"
#include "countries.h"
#include "provinces.h"
#include "mapGeneration.h"




sf::Color HSVtoRGB(float H, float S, float V);   // h:0-360.0, s:0.0-1.0, v:0.0-1.0
sf::Vector3f RGBtoHSV(sf::Color RGB);
bool equalToVectu(sf::Vector2u& a, sf::Vector2u& b);

void resizeImage(const sf::Image& originalImage, sf::Image& resizedImage);

float dist(sf::Vector2f a, sf::Vector2f b);

void write(sf::RenderWindow& window, std::string t1, sf::IntRect rect, sf::Color color = sf::Color::Black);

class Mouse {
public:
	bool isPressed;			// any button pressed?
	bool isLeftPressed;
	bool startPressing = false;

	sf::Vector2i pos;		// with the window referencial, not the screen's

	Mouse(bool p1 = false, bool pl = false, sf::Vector2i pos = sf::Vector2i(0,0)) : isPressed(p1), isLeftPressed(pl), pos(pos) {};
};

#include "buttons.h"
