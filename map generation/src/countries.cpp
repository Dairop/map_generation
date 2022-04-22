#include "countries.h"





void createCountryFromIsolatedRebels(Province* StartingProv, Country* From, float depthInCountry, std::vector<Country*>& countries, sf::Image& map) {
	Country* C = new Country;
	C->init(StartingProv);
	C->color = HSVtoRGB(rand() % 360, 0.5 + 0.1 * (rand() % 5), 0.7 + 0.05 * (rand() % 6));
	C->capitalCity->changeColor(C->color, map);
	C->FindBorderProvinces();
	countries.push_back(C);

	if (From != nullptr) {
		//gather all provinces of the new country
		std::vector<Province*> provs;
		StartingProv->controlledBy = From;
		C->provinces.clear(); //the capital city will be added again in the function allConnectedProvinesFromSameCountry
		std::cout << 1000 * depthInCountry << "p ";
		StartingProv->allConnectedProvinesFromSameCountry(From->provinces.size() * depthInCountry, C->provinces);

		for (int p = 0; p < C->provinces.size(); p++) {
			C->provinces[p]->changeCountry(C, map);
		}

		From->FindBorderProvinces();


		int maxDepth = From->provinces.size();

		From->provincesConnectedToCapital.clear();
		From->capitalCity->allConnectedProvinesFromSameCountry(maxDepth, From->provincesConnectedToCapital);
		for (int borderi = 0; borderi < C->borderProvinces.size(); borderi++) {
			for (int i = 0; i < From->provinces.size(); i++) {
				Province* P2 = From->provinces[i];
				//Country secedes if it is not easily connected to it capital city
				if (!P2->isConnectedToCapital()) {
					float importance = rand() % 70 + 10;
					createCountryFromIsolatedRebels(P2, From, importance / 100, countries, map);
				}
			}
		}

		return;
	}
}








void Country::init(Province*& StartingProvince) {
	//color = HSVtoRGB(rand() % 360, 0.5 + 0.1 * (rand() % 5), 0.7 + 0.05 * (rand() % 6));
	color = StartingProvince->color;
	color.a = 255;

	name = "";
	std::string chars = "abcdefghijklmnopqrstuvwxyz";
	for (int ch = 0; ch < 5 + rand() % 5; ch++) {
		name += chars[rand() % chars.size()];
	}

	provinces.clear();
	provinces.push_back(StartingProvince);
	borderProvinces.clear();
	borderProvinces.push_back(StartingProvince);

	StartingProvince->controlledBy = this;
	capitalCity = StartingProvince;	
	//std::cout << (int) this->color.r << " " << (int) this->color.g << " " << (int) this->color.b << "\n";
	int surface = StartingProvince->surfaceArea;
	float militaryPower = StartingProvince->surfaceArea;
}


void Country::calculateSurface() {
	surface = 0;
	for (int p = 0; p < provinces.size(); p++) {
		surface += provinces[p]->surfaceArea;
	}
}

void Country::calculatePower() {
	militaryPower /= 1.2;
	
	for (int p = 0; p < provinces.size(); p++) {
		if (provinces[p]->militaryPower < 0) exit;
		militaryPower += provinces[p]->militaryPower / 5;
	}
}

void Country::FindBorderProvinces() {
	borderProvinces.clear();
	for (int p = 0; p < provinces.size(); p++) {
		int num = 0;
		for (int b = 0; b < provinces[p]->neighbors.size(); b++) {
			if (provinces[p]->neighbors[b]->controlledBy != this) {
				num++;
			}
		}
		if (num > 0) {
			borderProvinces.push_back(provinces[p]);
		}
	}
}




void Country::checkIfStillExists(std::vector<Country*>& countries) {
	if (this->provinces.size() > 0) return;
	for (int c = 0; c < countries.size(); c++) {
		if (countries[c]->name == this->name) {
			countries.erase(countries.begin()+c,countries.begin()+c+1);
			return;
		}
	}
}



void Country::checkIfProvinceIsSecessing(Province* P, std::vector<Country*>& countries, sf::Image& map){
	// "this" is the loosing country
	// P is the province that was lost to P->controlledBy

	//check if any of the neighbors is isolated, if so let it create it own country
	int maxDepth = this->provinces.size(); //increasing the denominator makes countries to look more round
	std::vector<Province*> connectedProvs;
	this->provincesConnectedToCapital.clear();
	this->capitalCity->allConnectedProvinesFromSameCountry(maxDepth, this->provincesConnectedToCapital);

	int nbOfNotConnectedProvinces = this->provinces.size() - this->provincesConnectedToCapital.size();

	if (nbOfNotConnectedProvinces > 10) {
		for (int i = 0; i < this->provinces.size(); i++) {
			Province* P2 = this->provinces[i];
			if ((P2->controlledBy == nullptr) or (P2->controlledBy != this)) { continue; }
			//Country secedes if it is not easily connected to it capital city
			if (!P2->isConnectedToCapital()) {
				float importance = rand() % 70 + 10;

				createCountryFromIsolatedRebels(P2, this, importance/100, countries, map);

				//Country* newCountry = P2->controlledBy;
				//if (newCountry->provinces.size() < 5) {
					//;
				//}
				//return;
			}

		}
	} else {
		Country* countryWinningTheProvince;
		for (int i = 0; i < this->provinces.size(); i++) {
			Province* P2 = this->provinces[i];
			if ((P2->controlledBy == nullptr) or (P2->controlledBy != this)) { continue; }
			//Country secedes if it is not easily connected to it capital city
			if (!P2->isConnectedToCapital()) {
				//give it to a neighbor if the new rebel state will be too small to exit. 
				//If no neighbor, give it to the country that won the battle
				countryWinningTheProvince = P->controlledBy;
				for (int nei = 0; ((nei < P2->neighbors.size()) && (countryWinningTheProvince == P->controlledBy)); nei++) {
					if (P2->neighbors[nei]->controlledBy != nullptr && P2->neighbors[nei]->controlledBy != this) {
						countryWinningTheProvince = P2->neighbors[nei]->controlledBy;
						break;
					}
				}
				P2->changeCountry(countryWinningTheProvince, map);
			}
		}
	}

}






void Country::expand(sf::Image& map, std::vector<Country*>& countries) {
	//select a random border province to chose where to start the invasion
	if (borderProvinces.size() == 0) { return; }
	//index in borderProvinces vector of attacking province
	int indexSelectedProv = rand() % borderProvinces.size();

	//border province of attacking country
	Province* Pr = borderProvinces[indexSelectedProv];

	//select which neighboring province to attack
	if (Pr->neighbors.size() == 0) { 
		//if the selected one don't have a neighbor, find another province that do have one
		for (int i = 0; i < borderProvinces.size(); i++) {
			if (borderProvinces[(indexSelectedProv + i) % borderProvinces.size()]->neighbors.size() > 0) {
				indexSelectedProv = (indexSelectedProv + i) % borderProvinces.size();
				i = borderProvinces.size();
				Province* Pr = borderProvinces[indexSelectedProv];
			}
		}
		return; // isolated country, no neighbors
	}
	int indexNei = rand() % borderProvinces[indexSelectedProv]->neighbors.size();
	//attacked province
	Province* Pn = borderProvinces[indexSelectedProv]->neighbors[indexNei];
	Country* C2 = Pn->controlledBy; // country attacked

	Country* Winner; Province* WinnerProv;
	Country* Looser; Province* LooserProv;

	// a country can't attack itself
	if (C2 == this) return;
	//not aggressive countries attack less often
	if (rand() % 20 >= aggressivity) return;

	calculateSurface();
	calculatePower();
	float power2;
	if (C2 != nullptr) {
		C2->calculateSurface();
		C2->calculatePower();
		power2 = C2->militaryPower;
	}
	else {
		//inactive prov playing
		power2 = Pn->surfaceArea;
	}


	//evaluate if a province is easy to capture or not, give a score 'prcBorders' from 0 to 1 accordingly
	int nbBorders = 0;
	for (int i = 0; i < Pn->neighbors.size(); i++) {
		if (Pn->neighbors[i]->controlledBy == this) nbBorders++;
	}
	float prcBorders = (float) nbBorders / Pn->neighbors.size();

	               
	if ((Pn->controlledBy == nullptr)                                                       //don't have a country     
			|| (1 + float (rand()%35)/20) * militaryPower * prcBorders * 1.2 >= power2      //or is stronger (with bonus for the attacking country)
			|| rand()%100 == 0) {                                                           //or by chance (unlock some situations and add more luck into the balance)
		WinnerProv = Pr; Winner = this;
		LooserProv = Pn; Looser = C2;
		
		//std::cout << "   " << Winner;
		LooserProv->changeCountry(Winner, map);

		//check if the country is fully annexed or if any of the annexed province's neightbors are now isolated
		if (Looser != nullptr) {
			if (Looser->militaryPower < Winner->militaryPower / 10) {
				for (int pr = 0; pr < Looser->provinces.size(); pr++) {
					Looser->provinces[pr]->changeCountry(Winner, map);
				}
			}
			else {
				Looser->checkIfProvinceIsSecessing(LooserProv, countries, map);
				//don't loose as much stability because they didn't attack
				Looser->stability -= 1;
			}
			Looser->checkIfStillExists(countries);
		}
		Winner->stability += 1;
	}
	else {

		//calculate if the second country capture the attacking province

		if (C2 == nullptr) return; //not if it is an inactive bot
		//evaluate if a province is easy to capture or not, give a score 'prcBorders' from 0 to 1 accordingly
		prcBorders = 0; nbBorders = 0;
		for (int i = 0; i < Pr->neighbors.size(); i++) {
			if (Pr->neighbors[i]->controlledBy == C2) nbBorders++;
		}
		float prcBorders = nbBorders / Pr->neighbors.size();

		//is the battle won ?                     malus for the defending country
		if ((1 + float(rand() % 25) / 20) * power2 * prcBorders * 0.7 >= militaryPower
				|| rand()%50==0) {	//or win by luck
			LooserProv = Pr; Looser = this;
			WinnerProv = Pn; Winner = C2;
			
			LooserProv->changeCountry(Winner, map);

			//check if the country is fully annexed or if any of the annexed province's neightbors are now isolated
			if (Looser != nullptr) {
				if (Looser->militaryPower < Winner->militaryPower / 10) {
					for (int pr = 0; pr < Looser->provinces.size(); pr++) {
						Looser->provinces[pr]->changeCountry(Winner, map);
					}
				} else {
					Looser->checkIfProvinceIsSecessing(LooserProv, countries, map);
					Looser->stability -= 2;
				}
				Looser->checkIfStillExists(countries);
			}
			//Win more stability because they were defending
			Winner->stability += 2;
		}
	}
}
