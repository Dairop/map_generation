#include "provinces.h"



void Province::init(sf::Vector2f startingPos) {
	color = HSVtoRGB(rand() % 360, 0.5 + 0.01 * (float)(rand() % 50), 0.7 + 0.005 * (float)(rand() % 60));
	color.a = rand() % 150;

	surfaceArea = 1;
	controlledBy = nullptr;

	posCity = startingPos;

	name = "";
	std::string chars = "abcdefghijklmnopqrstuvwxyz";

	for (int ch = 0; ch < 6 + rand() % 5; ch++) {
		name += chars[rand() % chars.size()];
	}

	id = name + std::to_string((int) startingPos.x) + std::to_string((int) startingPos.y);


	borders.push_back(sf::Vector2u(startingPos.x, startingPos.y));
	allPixels.push_back(sf::Vector2u(startingPos.x, startingPos.y));
	neighbors.clear();
}



void findPlaceToSpawnProvince(sf::Image& map, Province*& CurrentProvince) {
	sf::Vector2f pos;
	sf::Vector2u mapSize = map.getSize();
	//pos = sf::Vector2f(rand() % mapSize.x, rand() % mapSize.y);
	pos = sf::Vector2f(rand() % mapSize.x, rand() % mapSize.y);

	while (map.getPixel(pos.x, pos.y) != sf::Color::White) {
		pos = sf::Vector2f(rand() % mapSize.x, rand() % mapSize.y);
	}

	CurrentProvince->init(pos);
	map.setPixel(pos.x, pos.y, CurrentProvince->color);
	CurrentProvince->surfaceArea = 1;
	CurrentProvince->controlledBy = nullptr;
};





void Province::getAllPixels(sf::Image& map) {
	allPixels.clear();
	std::vector<sf::Vector2u> px;			//pixels neighboring checked ones
	px = borders;
	allPixels = borders;
	allPixels = px;


	sf::Vector2u p;
	int max = map.getSize().x;
	while (px.size() > 0) {
		p = px[0];
		//add p to all pixels vector if it's not already there
		//if p isn't in the borders, then add it to the px vector
		p.x += -1 * (p.x != 0);
		if (map.getPixel(p.x, p.y) == color) {
			if (std::find_if(allPixels.begin(), allPixels.end(), [&](sf::Vector2u a) { return (a.x == p.x & a.y == p.y); }) == allPixels.end()) {
				allPixels.push_back(p);
				px.push_back(p);
			}
		}

		// right
		p.x += 1;
		if (map.getPixel(p.x, p.y) == color) {
			if (std::find_if(allPixels.begin(), allPixels.end(), [&](sf::Vector2u a) { return (a.x == p.x & a.y == p.y); }) == allPixels.end()) {
				allPixels.push_back(p);
				px.push_back(p);
			}
		}

		//top
		if (map.getPixel(p.x, p.y) == color) {
			if (std::find_if(allPixels.begin(), allPixels.end(), [&](sf::Vector2u a) { return (a.x == p.x & a.y == p.y); }) == allPixels.end()) {
				allPixels.push_back(p);
				px.push_back(p);
			}
		}

		//bottom
		p.y += 1;
		if (map.getPixel(p.x, p.y) == color) {
			if (std::find_if(allPixels.begin(), allPixels.end(), [&](sf::Vector2u a) { return (a.x == p.x & a.y == p.y); }) == allPixels.end()) {
				allPixels.push_back(p);
				px.push_back(p);
			}
		}

		//if (px[0].x > 0 & px[0].y < 500 & px[0].y > 0 & px[0].x < 500)		map.setPixel(px[0].x, px[0].y, sf::Color::Red);
		px.erase(px.begin());
	}
}




sf::Vector2u stayInLimits(sf::Vector2u pos, int dir, sf::Vector2u limits) {
	// 0 = left ; 1 = top ; 2 = right ; 3 = bottom

	switch (dir) {
	case 0:
		pos.x += -1 * (pos.x != 0);
		break;
	case 1:
		pos.y += -1 * (pos.y != 0);
		break;
	case 2:
		pos.x += !(pos.x >= limits.x - 1);
		break;
	case 3:
		pos.y += !(pos.y >= limits.y - 1);
		break;
	}
	/*
	if (dir == 0) {
		pos.x += -1 * (pos.x != 0);
	}
	else if (dir == 1) {
		pos.y += -1 * (pos.y != 0);
	}
	else if (dir == 2) {
		pos.x += !(pos.x >= limits.x - 1);
	}
	else if (dir == 3) {
		pos.y += !(pos.y >= limits.y - 1);
	}
	*/
	return sf::Vector2u(pos.x, pos.y);
}


void Province::deleteUselessBorders(sf::Vector2u mapsize, sf::Image& map) {
	sf::Vector2u pos;
	for (int b = 0; b < borders.size(); b++) {
		pos = borders[b];

		sf::Vector2u le, to, ri, bo;
		le = stayInLimits(pos, 0, mapsize);
		to = stayInLimits(pos, 1, mapsize);
		ri = stayInLimits(pos, 2, mapsize);
		bo = stayInLimits(pos, 3, mapsize);

		// anything which is not the country itself around ?
		int test = (map.getPixel(le.x, le.y) != color) + (map.getPixel(to.x, to.y) != color)
			+ (map.getPixel(ri.x, ri.y) != color) + (map.getPixel(bo.x, bo.y) != color);

		//delete a specific vector

		if (test == 0) {
			borders.erase(borders.begin() + b);
		}
	}
}

bool conquerPixel(sf::Vector2u p, int difficultyToExpend, sf::Color color, sf::Color colorToConquer, sf::Image& map) {
	if (color == colorToConquer) return true;
	if (map.getPixel(p.x, p.y) == colorToConquer && rand() % difficultyToExpend > 1) {
		map.setPixel(p.x, p.y, color);
		return true;
	}
	return false;
}

void Province::grow(sf::Image& map) {
	sf::Vector2u pos, p;
	sf::Vector2u mapsize = map.getSize();

	int initialBorders = borders.size();
	for (int b = 0; b < initialBorders; b++) {
		pos = borders[b];
		// bigger difficulty = easier because I decided it was
		int difficultyToExpend = 10;//5+rand()%4;

		//expend left
		p = stayInLimits(pos, 0, mapsize);
		if (conquerPixel(p, difficultyToExpend, color, sf::Color::White, map) && p != pos) {
			borders.push_back(p);
			if (std::find_if(allPixels.begin(), allPixels.end(), [&](sf::Vector2u a) { return ((a.x == p.x) & (a.y == p.y)); }) == allPixels.end()) {
				surfaceArea++;
				allPixels.push_back(p);
			}
		}

		// right
		p = stayInLimits(pos, 2, mapsize);
		if (conquerPixel(p, difficultyToExpend, color, sf::Color::White, map) && p != pos) {
			borders.push_back(p);
			if (std::find_if(allPixels.begin(), allPixels.end(), [&](sf::Vector2u a) { return (a.x == p.x & a.y == p.y); }) == allPixels.end()) {
				surfaceArea++;
				allPixels.push_back(p);
			}
		}

		//top
		p = stayInLimits(pos, 1, mapsize);
		if (conquerPixel(p, difficultyToExpend, color, sf::Color::White, map) && p != pos) {
			borders.push_back(p);
			if (std::find_if(allPixels.begin(), allPixels.end(), [&](sf::Vector2u a) { return (a.x == p.x & a.y == p.y); }) == allPixels.end()) {
				surfaceArea++;
				allPixels.push_back(p);
			}
		}

		//bottom
		p = stayInLimits(pos, 3, mapsize);
		if (conquerPixel(p, difficultyToExpend, color, sf::Color::White, map) && p != pos) {
			borders.push_back(p);
			if (std::find_if(allPixels.begin(), allPixels.end(), [&](sf::Vector2u a) { return (a.x == p.x & a.y == p.y); }) == allPixels.end()) {
				surfaceArea++;
				allPixels.push_back(p);
			}
		}
	}

	//delete useless borders
	if (rand() % 5 == 0) {
		deleteUselessBorders(mapsize, map);
	}
}


bool Province::checkIfBordersWater(sf::Image& map) {
	sf::Color waterColors[2] = { sf::Color::Black, sf::Color(0, 70, 150)};
	sf::Vector2u mapsize = map.getSize();

	for (int b = 0; b < this->borders.size(); b++) {
		for (int dir = 0; dir < 4; dir++) {
			sf::Vector2u pos = this->borders[b];
			sf::Vector2u p = stayInLimits(pos, 0, mapsize);
			if (p != pos) {
				sf::Color colorAtTestedPix = map.getPixel(p.x, p.y);
				if (colorAtTestedPix == waterColors[0] || colorAtTestedPix == waterColors[1]) {
					this->borderWater = true;
					return true;
				}
			}
		}
	}
	this->borderWater == false;
	return false;
}


void Province::changeColor(sf::Color newColor, sf::Image& map) {
	color = newColor;
	for (int i = 0; i < allPixels.size(); i++) {
		//std::cout << i << "  " << allPixels.size() << "       " << allPixels[i].y;// << " " << allPixels[i].y << "   ";
		map.setPixel(allPixels[i].x, allPixels[i].y, newColor);
	}

	//map.setPixel(posCity.x, posCity.y, sf::Color::Black);
}


void Province::changeColorAlternating(sf::Color newColor, sf::Image& map){
	for (int i = 0; i < allPixels.size(); i++) {
		if ((allPixels[i].x + allPixels[i].y) % 2) map.setPixel(allPixels[i].x, allPixels[i].y, newColor);
	}
}



void Province::updateMilitaryPower() {
	this->militaryPower = this->allPixels.size();
	this->militaryPower *= 0.5 + this->borderWater;
}





void Province::checkneighbors(sf::Image& map, std::vector<Province*>& allProvinces) {
	deleteUselessBorders(map.getSize(), map);
	sf::Vector2u left, top, right, bottom;
	sf::Vector2u limits = map.getSize();
	std::vector<sf::Color> borderingColors;
	sf::Color c;
	for (int b = 0; b < borders.size(); b++) {
		left = stayInLimits(borders[b], 0, limits);
		top = stayInLimits(borders[b], 1, limits);
		right = stayInLimits(borders[b], 2, limits);
		bottom = stayInLimits(borders[b], 3, limits);

		c = map.getPixel(left.x, left.y);
		//not empty land or water
		if (c != sf::Color::White && c != sf::Color(0, 70, 150) && c != color) {
			if (std::find(borderingColors.begin(), borderingColors.end(), c) == borderingColors.end()) {
				borderingColors.push_back(c);
			}
		}

		c = map.getPixel(top.x, top.y);
		//not empty land or water
		if (c != sf::Color::White && c != sf::Color(0, 70, 150) && c != color) {
			if (std::find(borderingColors.begin(), borderingColors.end(), c) == borderingColors.end()) {
				borderingColors.push_back(c);
			}
		}

		c = map.getPixel(right.x, right.y);
		//not empty land or water
		if (c != sf::Color::White && c != sf::Color(0, 70, 150) && c != color) {
			if (std::find(borderingColors.begin(), borderingColors.end(), c) == borderingColors.end()) {
				borderingColors.push_back(c);
			}
		}

		c = map.getPixel(bottom.x, bottom.y);
		//not empty land or water
		if (c != sf::Color::White && c != sf::Color(0, 70, 150) && c != color) {
			if (std::find(borderingColors.begin(), borderingColors.end(), c) == borderingColors.end()) {
				borderingColors.push_back(c);
			}
		}
	}

	// add all neighbors to the list, 
	// they have to all been initialized with a color of
	// sf::Color(i % 254 + 1, (i / 254)%254 + 1, (i / 254 / 254) % 254 + 1)
	// to recognize them (i is their index in the provinces vector)
	for (int i = 0; i < borderingColors.size(); i++) {
		c = borderingColors[i];
		int indexNei = c.r - 1 + (c.g - 1) * 254 + (c.b - 1) * 254 * 254;
		if (indexNei < allProvinces.size()) { // if something is wrong like the water color
			if (std::find(neighbors.begin(), neighbors.end(), allProvinces[indexNei]) == neighbors.end()) {
				neighbors.push_back(allProvinces[indexNei]);
			}
		}
	}
	//std::cout << name << " a une longueur frontaliere de: " << borders.size() << " avec " << neighbors.size() << " provinces voisines, "<<" et une surface de "<< surfaceArea << " = "<<allPixels.size() << "\n";
}





void Province::allConnectedProvinesFromSameCountry(int maxDepth, std::vector<Province*>& ConnectedProv) {
	ConnectedProv.push_back(this);

	//distance to capital city, we will only search in 
	//provinces closer to the capital city than the actual province (with a factor to it dist)
	//hence countries have to be somewhat linear/convex to be considered connected
	int i = 0;
	while ((i < ConnectedProv.size()) and (maxDepth > 0)) {
		Province* tested = ConnectedProv[i];
		for (int n = 0; n < tested->neighbors.size(); n++) {
			// if don't have a country
			if (tested->neighbors[n]->controlledBy == nullptr) continue;
			// if not already in list and in the same country
			if ((std::find(ConnectedProv.begin(), ConnectedProv.end(), tested->neighbors[n]) == ConnectedProv.end()
				&& (this->controlledBy->name == tested->neighbors[n]->controlledBy->name))) {
		
				ConnectedProv.push_back(tested->neighbors[n]);
			}
		}
		maxDepth-=1;
		i+=1;
	}
}



bool Province::isConnectedToCapital() {
	if (this->controlledBy == nullptr) return true;
	//allConnectedProvinesFromSameCountry(maxDepth, tested);

	return (std::find(this->controlledBy->provincesConnectedToCapital.begin(), this->controlledBy->provincesConnectedToCapital.end(), this) != this->controlledBy->provincesConnectedToCapital.end());
}


void Province::changeCountry(Country* newCountry, sf::Image& map) {
	Province* LooserProv = this;
	Country* Looser = this->controlledBy;
	Country* Winner = newCountry;

	if (Looser != nullptr) {
		Looser->provinces.erase(std::remove(Looser->provinces.begin(), Looser->provinces.end(), LooserProv), Looser->provinces.end());
		Looser->FindBorderProvinces();

		if (Looser->capitalCity == LooserProv) {
			if (Looser->provinces.size() > 0) {
				Looser->capitalCity = Looser->provinces[0];
			}
		}

		int ls = Looser->provinces.size() + 1;
		Looser->militaryPower -= std::fmin(abs(Looser->militaryPower - Winner->militaryPower), Looser->militaryPower / ls);
		Winner->militaryPower += std::fmin(abs(Looser->militaryPower - Winner->militaryPower), Looser->militaryPower / ls);
	}

	//change color and control
	LooserProv->controlledBy = Winner;
	Winner->provinces.push_back(LooserProv);
	LooserProv->changeColor(Winner->color, map);

	Winner->FindBorderProvinces();
}


