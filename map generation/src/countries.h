#pragma once
#include "commons.h"

class Province;
class Country;


void createCountryFromIsolatedRebels(Province* StartingProv, Country* From, float depthInCountry /*0 to 1, only in accessible provinces*/, std::vector<Country*>& countries, sf::Image& map);



class Country {
public:
	sf::Color color;
	std::string name;
	std::vector<Province*> provinces;
	std::vector<Province*> borderProvinces;
	std::vector<Province*> provincesConnectedToCapital;
	Province* capitalCity;

	int surface = 0;
	float militaryPower = 1;
	int aggressivity = 4 + rand() % 9 + rand() % 9; //20 = attack at each update, 0 = will never attack
	float stability = 0;

	void init(Province*& StartingProvince);
	void calculateSurface();
	void calculatePower();
	void FindBorderProvinces();
	void checkIfStillExists(std::vector<Country*>& countries);
	void checkIfProvinceIsSecessing(Province* P, std::vector<Country*>& countries, sf::Image& map);
	void expand(sf::Image& map, std::vector<Country*>& countries);
};
