#pragma once
#include "commons.h"



sf::Vector2u stayInLimits(sf::Vector2u pos, int dir, sf::Vector2u limits);


class Country;

class Province {
public:
	std::string id;
	sf::Color color;
	std::string name;
	sf::Vector2f posCity; //first pixel of the province placed
	int surfaceArea = 0;
	std::vector <sf::Vector2u> borders;
	std::vector <sf::Vector2u> bordersWithEmpty;
	std::vector <Province*> neighbors;
	std::vector <sf::Vector2u> allPixels;
	Country* controlledBy;
	bool borderWater = false;
	float militaryPower = 1;

	int timeWithoutGrowth = 0; //used when initially conquering pixels, used to check if there is still wilderness around


	void init(sf::Vector2f startingPos);
	void getAllPixels(sf::Image& map);
	void deleteUselessBorders(sf::Vector2u mapsize, sf::Image& map);
	void grow(sf::Image& map);
	void changeColor(sf::Color newColor, sf::Image& map);
	void changeColorAlternating(sf::Color newColor, sf::Image& map);
	bool checkIfBordersWater(sf::Image& map);
	void updateMilitaryPower();
	void checkneighbors(sf::Image& map, std::vector<Province*>& allProvinces);
	void allConnectedProvinesFromSameCountry(int maxDepth, std::vector<Province*>& ConnectedProv);
	bool isConnectedToCapital();
	void changeCountry(Country* newCountry, sf::Image& map);
};



void findPlaceToSpawnProvince(sf::Image& map, Province*& CurrentCountry);

