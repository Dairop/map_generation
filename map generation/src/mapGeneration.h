#pragma once
#include "commons.h"


void generateCellsMap(int numberOfProvinces, sf::Image& mapImg, std::vector<Province*>& provinces, float& advancement);
sf::Texture generateNoiseHeightmap(sf::Vector2f MAP_SIZE, int resolution, std::vector<Province*> provinces, sf::Image& mapImg);
sf::Texture heightToColors(sf::Image heightmap);
sf::Texture newMap(sf::Vector2f MAP_SIZE, int resolution, int fractalLike /*from 0 to 100*/, float& advancement /*0 to 100*/);