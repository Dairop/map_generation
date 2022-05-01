#include "commons.h"



//global variables
int windowWidth = 1920;
int windowHeight = 1080;

bool showBordersAtTheEnd = true;

sf::Clock deltaClock;
sf::Time dt = deltaClock.restart();

sf::RenderWindow window(sf::VideoMode(windowWidth, windowHeight), "Name");

int generationMode = 0; // 0 = generate map ; 1 = use existing map 

bool pause = true;
float zoom = 1;
sf::Vector2f SIZE = sf::Vector2f(1920 / 1, 1080 / 1); // size of the map
int mapRes = 1;
sf::Vector2f camPos = sf::Vector2f(SIZE.x * 1.5, SIZE.y * 1.5);
int nbProvinces = 3000;
int nbCountries = 3000;
int nbFinalCountries = 120;
bool rebelsWhenTooBig = true;
int rebelsFrequency = 1;	// from 0 to 4, when 0 rebels never spawn 
							//when 1 rebels spawn when the country control (5-1)*20=80% of the world
							//when 4 rebels spawn when the country controls 80% of the world

float genSpeed = 1;  //from 1 to 100

Mouse UserMouse;






void printAdvancement(float& adv) {
	while (adv != 100) {
		sf::sleep(sf::milliseconds(50));
		std::cout << "\n" << adv << "%";
	}
}







void growCountries(std::vector<Province*>& provinces, std::vector<Country*>& countries, sf::RenderWindow& window, sf::Image& map, sf::Image& heightmap) {
	while (countries.size() > nbFinalCountries){
		//check if end of thread
		if (!window.isOpen()) return;

		/*
		//https://www.desmos.com/calculator/i8cje9jsdf
		float x1 = ((float)(-genSpeed + 100)) * (0.017/(sqrt(genSpeed)+2));
		float x2 = (genSpeed/-20)*(genSpeed/20) + 1;
		std::cout << "\n" << fmax(x1, x2) * 10000.0;
		sf::sleep(sf::microseconds(fmax(x1, x2) * 10000.0));// * (0.1/sqrt(genSpeed))));
		*/

		//if (rand() % 100 >= genSpeed) sf::sleep(sf::microseconds(((float)(100 - genSpeed))*100));

		//can't get sf::sleep to wait under the ms area so we wait by doing some calculations
		for (int i = 0; i < 100 - genSpeed; i++) {
			bool wait1 = false;
			while (wait1 != true) {
				wait1 = (rand() % 10000 <= genSpeed*countries.size()/700);
			}
		}

		while (pause) {
			sf::sleep(sf::milliseconds(100));
		}



		int n = rand() % countries.size();
		
		if (rand() % 20 + 1 <= countries[n]->aggressivity) {
			countries[n]->expand(map, countries);
		}

		if (countries[n]->stability < 0 or (rebelsWhenTooBig &&     countries[n]->provinces.size() > (provinces.size()*(51-rebelsFrequency)/50) )) {
			//create rebels in province
			if (countries[n]->provinces.size() < 10) {
				if (countries[n]->provinces.size() == 0) countries[n]->checkIfStillExists(countries);

			} else {
				int nbReb = std::min((int)countries[n]->provinces.size() - 5, rand() % 4 + 1);
				for (int rebn = 0; rebn < nbReb; rebn++) {
					int nbProvLeft = countries[n]->provinces.size();
					if (nbProvLeft == 0) break;
					int indexProv = rand() % nbProvLeft;
					float importance = (1 / (-abs(countries[n]->stability) - 1)) + 1;
					importance *= ((float) (1+rand() % 20)) / 100;
					createCountryFromIsolatedRebels(countries[n]->provinces[indexProv], countries[n], importance, countries, map);
				}
			}
		}


		countries[n]->stability *= 1 - countries[n]->aggressivity/50; //stability tends to 0 for most countries if no attack is made
	}

	//conquests ended
	

	for (int c = 0; c < countries.size(); c++) {
		//check if end of thread
		if (!window.isOpen()) return;

		if (showBordersAtTheEnd) {
			for (int p = 0; p < countries[c]->provinces.size(); p++) {
				for (int px = 0; px < countries[c]->provinces[p]->borders.size(); px++) {
					map.setPixel(countries[c]->provinces[p]->borders[px].x,
						countries[c]->provinces[p]->borders[px].y,
						sf::Color::Black);
				}
			}
		}

		//fill half of the pixels from the capital to a darker variant
		ColorHSV newColorForCapital = RgbToHsv(countries[c]->color);
		newColorForCapital.v = fmax(0, newColorForCapital.v - 0.3);
		countries[c]->capitalCity->changeColorAlternating(HSVtoRGB(newColorForCapital.h, 
																	newColorForCapital.s, 
																	newColorForCapital.v),
															map);

		//draw a white cross of 5pixels on the capital city
		sf::Vector2f pos = countries[c]->capitalCity->posCity;
		map.setPixel(pos.x, pos.y, sf::Color::White);
		sf::Vector2u pos2 = stayInLimits(sf::Vector2u(pos.x, pos.y), 0, map.getSize());
		map.setPixel(pos2.x, pos2.y, sf::Color::White);
		pos2 = stayInLimits(sf::Vector2u(pos.x, pos.y), 1, map.getSize());
		map.setPixel(pos2.x, pos2.y, sf::Color::White);
		pos2 = stayInLimits(sf::Vector2u(pos.x, pos.y), 2, map.getSize());
		map.setPixel(pos2.x, pos2.y, sf::Color::White);
		pos2 = stayInLimits(sf::Vector2u(pos.x, pos.y), 3, map.getSize());
		map.setPixel(pos2.x, pos2.y, sf::Color::White);



	}



	map.saveToFile("../img/map.png");
	//end of generation
}


void growProvince(std::vector<Province*>& provinces, std::vector<Country*>& countries, sf::RenderWindow&window, sf::Image& map, sf::Image& heightmap) {
	std::vector<Province*> provincesToGrow = provinces;

	bool growAtDoubleSpeed = (map.getSize().x * map.getSize().y) > 4000;

	while (provincesToGrow.size() != 0) {
		//provinces in the first indexes haven't been selected for a longer time
		//so we give them a higher change to get selected
		int n = rand() % (1+ rand() % (1 + rand() % provincesToGrow.size()));
		int bef = provincesToGrow[n]->allPixels.size();

		provincesToGrow[n]->grow(map);
		if (growAtDoubleSpeed) { 
			provincesToGrow[n]->grow(map); 
			provincesToGrow[n]->grow(map);
		}

		if (bef == provincesToGrow[n]->allPixels.size()) {
			provincesToGrow[n]->timeWithoutGrowth++;
		} else {
			provincesToGrow[n]->timeWithoutGrowth = 0;
		}
			
		if (provincesToGrow[n]->timeWithoutGrowth == 10) {
			provincesToGrow.erase(provincesToGrow.begin() + n);
			continue;
		}

		//allow for more parity
		provincesToGrow.push_back(provincesToGrow[n]);
		provincesToGrow.erase(provincesToGrow.begin() + n);

		//check if end of thread
		if (!window.isOpen()) return;
	}



	sf::Vector2u mapsize = map.getSize();
	for (int i = 0; i < provinces.size(); i++) {
		sf::Color c = sf::Color(i % 254 + 1, (i / 254)%254 + 1, (i / 254 / 254) % 254 + 1);
		c.a = 255;
		provinces[i]->changeColor(c, map);

		//delete all artifacts left during the generation
		for (int n = 0; n < 5; n++) {
			provinces[i]->deleteUselessBorders(mapsize, map);
		}

		provinces[i]->checkIfBordersWater(map);
		provinces[i]->updateMilitaryPower();

		//check if end of thread
		if (!window.isOpen()) return;
	}

	for (int i = 0; i < provinces.size(); i++) {
		provinces[i]->checkneighbors(map, provinces);
	}

	for (int i = 0; i < countries.size(); i++) {
		countries[i]->provinces[0]->changeColor(countries[i]->color, map);
	}

	//color the unused provinces in dark green
	for (int i = 0; i < provinces.size(); i++) {
		if (provinces[i]->controlledBy == nullptr) {
			provinces[i]->changeColor(sf::Color(15, 70, 40), map);
			provinces[i]->changeColorAlternating(sf::Color(30, 140, 80), map);
		}
	}

	growCountries(provinces, countries, window, map, heightmap);
	std::cout << "\nend";
}






void menuEvents() {
	sf::Event event;
	UserMouse.startPressing = false;
	while (window.pollEvent(event)) {
		if (event.type == sf::Event::Closed) {
			window.close();
			exit;
		}
		else if (event.type == sf::Event::Resized) {
			// update the view to the new size of the window
			sf::FloatRect visibleArea(0, 0, event.size.width, event.size.height);
			window.setView(sf::View(visibleArea));
			windowWidth = visibleArea.width;
			windowHeight = visibleArea.height;
		}
		else if (event.type == sf::Event::MouseButtonPressed) {
			UserMouse.isPressed = true;
			if (event.mouseButton.button == sf::Mouse::Left) {
				UserMouse.isLeftPressed = true;
				UserMouse.startPressing = true;
			}
		}


		else if (event.type == sf::Event::MouseButtonReleased) {
			UserMouse.isPressed = false;
			UserMouse.pos = sf::Vector2i(event.mouseButton.x, event.mouseButton.y);
			if (event.mouseButton.button == sf::Mouse::Left) {
				UserMouse.isLeftPressed = false;
			}
		}

		else if (event.type == sf::Event::MouseMoved) {
			UserMouse.pos = sf::Mouse::getPosition(window);
		}
	}
}



void generationEvents(std::thread& threadUpdateGrowth, sf::RectangleShape& map) {
	//Events
	sf::Event event;
	UserMouse.startPressing = false;
	while (window.pollEvent(event)) {
		if (event.type == sf::Event::Closed) {
			window.close();
			threadUpdateGrowth.join();
			exit;
		}
		if (event.type == sf::Event::Resized) {
			// update the view to the new size of the window
			sf::FloatRect visibleArea(0, 0, event.size.width, event.size.height);
			window.setView(sf::View(visibleArea));
			windowWidth = visibleArea.width;
			windowHeight = visibleArea.height;
		}
		else if (event.type == sf::Event::MouseWheelMoved) {
			int mdt = std::max(-1, std::min(event.mouseWheel.delta, 1));
			zoom *= 1 + ((float)mdt) / 15;

			map.setSize(sf::Vector2f(SIZE.x * zoom, SIZE.y * zoom));
			map.setPosition(sf::Vector2f(SIZE.x * zoom - camPos.x * zoom + windowWidth / 2, SIZE.y * zoom - camPos.y * zoom + windowHeight / 2));
		}
		else if (event.type == sf::Event::MouseButtonPressed) {
			UserMouse.isPressed = true;
			if (event.mouseButton.button == sf::Mouse::Left) {
				UserMouse.isLeftPressed = true;
				UserMouse.startPressing = true;
			}
		}

		else if (event.type == sf::Event::MouseButtonReleased) {
			UserMouse.isPressed = false;
			UserMouse.pos = sf::Vector2i(event.mouseButton.x, event.mouseButton.y);
			if (event.mouseButton.button == sf::Mouse::Left) {
				UserMouse.isLeftPressed = false;
			}
		}

		else if (event.type == sf::Event::MouseMoved) {
			UserMouse.pos = sf::Mouse::getPosition(window);
		}

		else if (event.type == sf::Event::KeyPressed) {
			if (event.key.code == sf::Keyboard::Space) {
				pause = !pause;
			}
			else if (event.key.code == sf::Keyboard::Up) {
				camPos.y -= 20 / zoom;
			}
			else if (event.key.code == sf::Keyboard::Down) {
				camPos.y += 20 / zoom;
			}
			else if (event.key.code == sf::Keyboard::Left) {
				camPos.x -= 20 / zoom;
			}
			else if (event.key.code == sf::Keyboard::Right) {
				camPos.x += 20 / zoom;
			}
			map.setSize(sf::Vector2f(SIZE.x * zoom, SIZE.y * zoom));
			map.setPosition(sf::Vector2f(SIZE.x * zoom - camPos.x * zoom + windowWidth / 2, SIZE.y * zoom - camPos.y * zoom + windowHeight / 2));
		}
	}
}


int startGeneration() {
	if (!window.isOpen()) return 0;


	// init
	srand(std::time(nullptr));

	sf::Image mapIm, heightMapIm;
	sf::Texture mapTx;


	if (generationMode == 1 /*map from user*/) {
		mapIm.loadFromFile("../img/base_map.png");
		SIZE.x = mapIm.getSize().x; SIZE.y = mapIm.getSize().y;
		mapTx.loadFromImage(mapIm);//loadFromFile("../img/france.png");
	}
	else { //generate fictional map
		float advancement = 0;
		std::thread trackOfAdvancement(printAdvancement, std::ref(advancement));
		heightMapIm = newMap(SIZE, mapRes, 10, std::ref(advancement)).copyToImage();
		trackOfAdvancement.join();
		mapTx = heightToColors(heightMapIm);
		mapIm = mapTx.copyToImage();
		mapIm.saveToFile("../img/map.png");
	}

	camPos = sf::Vector2f(SIZE.x * 1.5, SIZE.y * 1.5);

	sf::RectangleShape map;
	map.setSize(sf::Vector2f(SIZE.x * zoom, SIZE.y * zoom));
	map.setPosition(sf::Vector2f(SIZE.x * zoom - camPos.x * zoom + windowWidth / 2, SIZE.y * zoom - camPos.y * zoom + windowHeight / 2));


	map.setTexture(&mapTx);
	map.setOrigin(sf::Vector2f(0, 0));

	std::vector <Province*> provinces;
	std::vector <Country*> countries;

	if (!window.isOpen()) return 0;

	for (int i = 0; i < nbProvinces; i++) {
		Province* newProv = new Province;
		findPlaceToSpawnProvince(mapIm, newProv);
		provinces.push_back(newProv);
		if (nbProvinces > 100 && i % (nbProvinces / 100) == 0) std::cout << "New province spawned" << i << "\n";
	}

	for (int i = 0; i < std::min(nbProvinces, nbCountries); i++) {
		Country* newCountry = new Country;
		countries.push_back(newCountry);
		countries[i]->init(provinces[i]);
	}


	mapTx.loadFromImage(mapIm);
	map.setSize(SIZE);

	camPos = sf::Vector2f(SIZE.x * 1.5, SIZE.y * 1.5);

	std::thread threadUpdateGrowth(growProvince, std::ref(provinces), std::ref(countries), std::ref(window), std::ref(mapIm), std::ref(heightMapIm));


	Slider Speed(sf::Vector2f(10, 95), sf::Vector2f(30, 2), "relative", 1, 100);
	Button Pause(sf::Vector2i(0, 92), sf::Vector2i(7, 7), ">", "relative", 0) ;
	Speed.value = genSpeed;

	while (window.isOpen()) {
		generationEvents(threadUpdateGrowth, map);

		window.clear(sf::Color(50, 50, 0));

		mapTx.loadFromImage(mapIm);
		window.draw(map);

		Speed.update(window.getSize(), windowHeight);
		Pause.update(window.getSize(), windowHeight);
		Pause.rectScreen.left = 15;
		Speed.rectScreen.left = windowWidth / 15;
		if (UserMouse.isLeftPressed) {
			if (Speed.clicked(UserMouse)) { genSpeed = Speed.value; }
			else if (Pause.clicked(UserMouse)) { 
				pause = !pause;
				if (pause) {
					Pause.text = ">";
				} else {
					Pause.text = "||";
				}
			}
		}
		Speed.draw(window);
		Pause.draw(window);

		dt = deltaClock.restart();
		window.display();
	}


	return 0;

}



void menu() {
	sf::Texture backgroundTexture;
	backgroundTexture.create(1920, 1080);
	backgroundTexture.loadFromFile("../img/background.png");
	sf::RectangleShape backgroundImage;
	backgroundImage.setTexture(&backgroundTexture);
	backgroundImage.setSize(sf::Vector2f(1920, 1080));
	backgroundImage.setPosition(sf::Vector2f(0, 0));




	Button Start(sf::Vector2i(10, 85), sf::Vector2i(80, 10), "Start", "relative", 0);
	Button Mode(sf::Vector2i(7, 5), sf::Vector2i(60, 7), "Generate new map", "relative", 0);
	if (generationMode == 0) {	Mode.text = "Generate new map"; } else { Mode.text = "Use existing map"; }

	Button SizePannel(sf::Vector2i(7, 17), sf::Vector2i(40, 17), "", "relative", 0);
	Slider XSize(sf::Vector2f(9, 25), sf::Vector2f(36, 2), "relative", 600, 3500);
	Slider YSize(sf::Vector2f(9, 29.5), sf::Vector2f(36, 2), "relative", 600, 2500);
	XSize.value = SIZE.x; YSize.value = SIZE.y;

	Button ResPannel(sf::Vector2i(54, 17), sf::Vector2i(40, 12), "", "relative", 0);
	Slider Res(sf::Vector2f(57, 25), sf::Vector2f(34, 2), "relative", 1, 10);
	Res.value = 1;

	Button ProvAndCountPannel(sf::Vector2i(7, 41), sf::Vector2i(86, 23), "", "relative", 0);
	Slider Provs(sf::Vector2f(9.5, 48), sf::Vector2f(34, 2), "relative", 100, 7000);
	Slider Countr(sf::Vector2f(56.5, 48), sf::Vector2f(34, 2), "relative", 2, 7000);
	Slider FinalCountr(sf::Vector2f(56.5, 59), sf::Vector2f(34, 2), "relative", 1, 400);
	Slider Rebels(sf::Vector2f(9.5, 59), sf::Vector2f(34, 2), "relative", 0, 50);
	Provs.value = nbProvinces; Countr.value = nbCountries; FinalCountr.value = nbFinalCountries; Rebels.value = rebelsFrequency;

	Res.value = 1;

	sf::IntRect posTxt;

	while (window.isOpen()) {
		window.clear();
		backgroundImage.setPosition(sf::Vector2f(windowWidth/2 - 1920/2, windowHeight/2 - 1080/2));
		window.draw(backgroundImage);

		menuEvents();
		int menuSize = std::min(windowWidth, windowHeight) / 1.05;

		sf::RectangleShape backgroundMenu;
		backgroundMenu.setSize(sf::Vector2f(menuSize, menuSize));
		backgroundMenu.setFillColor(sf::Color(0, 0, 0, 230));
		backgroundMenu.setPosition(sf::Vector2f(windowWidth / 2 - menuSize / 2, windowHeight / 2 - menuSize / 2));
		window.draw(backgroundMenu);





		sf::Vector2u screenSize = sf::Vector2u(windowWidth, windowHeight);
		Start.update(screenSize, menuSize);
		Mode.update(screenSize, menuSize);
		ProvAndCountPannel.update(screenSize, menuSize);
		Provs.update(screenSize, menuSize);
		Countr.update(screenSize, menuSize);
		FinalCountr.update(screenSize, menuSize);
		Rebels.update(screenSize, menuSize);



		if (UserMouse.startPressing) {
			if (Mode.clicked(UserMouse)) {
				generationMode = (generationMode + 1) % 2;
				if (generationMode == 0) {
					Mode.text = "Generate new map";
				}
				else {
					Mode.text = "Use existing map";
				}
			}
			else if (Start.clicked(UserMouse)) {
				startGeneration();
			}
		}
		else if (UserMouse.isLeftPressed) {
			if (Provs.clicked(UserMouse)) {
				nbProvinces = Provs.value;
				
				Countr.value = std::min(nbCountries, nbProvinces);
				nbCountries = Countr.value;

				FinalCountr.value = std::min(nbCountries, FinalCountr.value);
				nbFinalCountries = FinalCountr.value;
			}
			else if (Countr.clicked(UserMouse)) {
				nbCountries = std::min(Countr.value, Provs.value);
				Countr.value = nbCountries;

				FinalCountr.value = std::min(nbCountries, FinalCountr.value);
				nbFinalCountries = FinalCountr.value;
			}
			else if (FinalCountr.clicked(UserMouse)) {
				nbFinalCountries = std::min(FinalCountr.value, Countr.value);
				FinalCountr.value = nbFinalCountries;
			}
			else if (Rebels.clicked(UserMouse)) {
				rebelsFrequency = Rebels.value;
				rebelsWhenTooBig = !(rebelsFrequency == 0);
			}
		}


		Start.draw(window);
		Mode.draw(window);
		ProvAndCountPannel.draw(window);
		Provs.draw(window);
		Countr.draw(window);
		FinalCountr.draw(window);
		Rebels.draw(window);

		posTxt = ProvAndCountPannel.rectScreen;
		posTxt.height /= 3.5; posTxt.top += posTxt.height / 8; posTxt.width /= 2.25;
		write(window, "nb of provinces: " + std::to_string(nbProvinces), posTxt);

		posTxt.left += ProvAndCountPannel.rectScreen.width / 1.81;
		write(window, "nb of countries: " + std::to_string(nbCountries), posTxt);

		posTxt.top += ProvAndCountPannel.rectScreen.height / 2.1;
		write(window, "nb of final countries: " + std::to_string(nbFinalCountries), posTxt);

		posTxt.left -= ProvAndCountPannel.rectScreen.width / 1.81;
		write(window, "rebels frequency: " + std::to_string(rebelsFrequency), posTxt);

		if (generationMode == 0) {
			SizePannel.update(screenSize, menuSize);
			XSize.update(screenSize, menuSize);
			YSize.update(screenSize, menuSize);
			ResPannel.update(screenSize, menuSize);
			Res.update(screenSize, menuSize);

			if (UserMouse.isLeftPressed) {
				if (XSize.clicked(UserMouse)) { SIZE.x = XSize.value; }
				if (YSize.clicked(UserMouse)) { SIZE.y = YSize.value; }
				if (Res.clicked(UserMouse)) { mapRes = (int)Res.value; }
			}

			SizePannel.draw(window);
			XSize.draw(window);
			YSize.draw(window);
			ResPannel.draw(window);
			Res.draw(window);


			posTxt = SizePannel.rectScreen;
			posTxt.height /= 2.5; posTxt.top += posTxt.height / 8;
			write(window, "Size (" + std::to_string((int)SIZE.x) + ", " + std::to_string((int)SIZE.y) + ")", posTxt);


			posTxt = ResPannel.rectScreen;
			posTxt.height /= 1.7; posTxt.top += posTxt.height / 6;
			write(window, "Map resolution: " + std::to_string(mapRes), posTxt);

		}

		window.display();
	}
}





int main() {
	menu();
};