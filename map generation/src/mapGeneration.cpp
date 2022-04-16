#include "mapGeneration.h"






void generateCellsMap(int numberOfProvinces, sf::Image& mapImg, std::vector<Province*>& provinces, float& advancement) {
	advancement = 0;
	float increment = 10.0 / numberOfProvinces;
	for (int i = 0; i < numberOfProvinces; i++) {
		Province* newProv = new Province;
		findPlaceToSpawnProvince(mapImg, newProv);
		provinces.push_back(newProv);
		advancement += increment;
	}

	//avdancement of 10 at the end
	advancement = 10.0;
	increment = 65.0 / numberOfProvinces;
	std::vector<Province*> provincesToGrow = provinces;
	while (provincesToGrow.size() != 0) {
		int n = rand() % provincesToGrow.size();
		int bef = provincesToGrow[n]->allPixels.size();

		provincesToGrow[n]->grow(mapImg);

		if (bef == provincesToGrow[n]->allPixels.size()) {
			provincesToGrow[n]->timeWithoutGrowth++;
		}
		else {
			provincesToGrow[n]->timeWithoutGrowth = 0;
		}

		if (provincesToGrow[n]->timeWithoutGrowth == 10) {
		
			provincesToGrow.erase(provincesToGrow.begin() + n);
			advancement += increment;
			continue;
		}
	}

	//avdancement of 75
	advancement = 75.0;


	mapImg.saveToFile("../img/cellsMap.png");
	//avdancement of 80
	advancement = 80.0;
}















sf::Texture generateNoiseHeightmap(sf::Vector2f MAP_SIZE, int resolution, std::vector<Province*> provinces, sf::Image& mapImg) {
	sf::Image noiseIm;
	noiseIm.create(MAP_SIZE.x / resolution, MAP_SIZE.y / resolution);
	resizeImage(mapImg, noiseIm);
	//noiseIm.saveToFile("../img/test.png");
	//noiseTxt.clear(sf::Color(0, 0, 0));

	sf::Vector2i sizePerlin = sf::Vector2i(MAP_SIZE.x / resolution, MAP_SIZE.y / resolution);
	const float POWISLAND = 9; // +/- squared fadding at borders
	srand(std::time(nullptr));
	int seeds[4] = { rand(), rand(), rand(), rand() }; //seeds for different octaves
	float p = 0.5, p1 = 0.5, p2 = 0.5, p3 = 0.5, p4 = 0.5, p5 = 0.5, c = 0.5; //init value of octaves and color
	float s = std::min(MAP_SIZE.x, MAP_SIZE.y); //size of largest square that can fit in (used for fadding at borders)

	
	for (int provIndex = 0; provIndex < provinces.size(); provIndex++) {
		Province* P = provinces[provIndex];

		float x = P->posCity.x; float y = P->posCity.y;

		//https://www.desmos.com/calculator/fqvcoq9vcz
		p = powf(abs(sizePerlin.x / 2 - x), POWISLAND) / powf((sizePerlin.x / 2), POWISLAND);
		p += powf(abs(sizePerlin.y / 2 - y), POWISLAND) / powf((sizePerlin.y / 2), POWISLAND);
		p = 1 - fmax(fmin(1, p), 0);
		//1 at the center, 0 at the corners


		//from 0 to 1			
		p1 = 0.5 * perlin(x * resolution / s * 3, y * resolution / s * 3, seeds[0]) + 0.5;
		p2 = 0.5 * perlin((x + 0.1) * resolution / s * 7, (y + 0.11) * resolution / s * 7, seeds[1]) + 0.5;
		p3 = 0.5 * perlin(x * resolution / s * 35, y * resolution / s * 35, seeds[2]) + 0.5;
		//p4 = 0.5 * perlin((x + 0.31) * resolution / s * 30, (y + 0.31) * resolution / s * 30, seeds[3]) + 0.5;
		//p5 = 0.5 * perlin((x + 0.73) * resolution / s * 20, (y + 0.73) * resolution / s * 20, seeds[0]) + 0.5;

		c = ((p/1 + p1/1 + p2/1.8 + p3/4 + p4/6 + p5/7) / (1/1 + 1/1 + 1/1.8 + 1/4 + 1/6 + 1/7)) * 205;
		c = fmin(fmax(0, c), 255);

		P->changeColor(sf::Color(c, c, c), noiseIm);
	}

	/* //generate a map without using provinces
	for (float y = 0; y < sizePerlin.y; y++) {
		for (float x = 0; x < sizePerlin.x; x++) {
			//re.setPosition(sf::Vector2f(x * resolution, y * resolution));

			//https://www.desmos.com/calculator/fqvcoq9vcz
			p = powf(abs(sizePerlin.x / 2 - x), POWISLAND) / powf((sizePerlin.x / 2), POWISLAND);
			p += powf(abs(sizePerlin.y / 2 - y), POWISLAND) / powf((sizePerlin.y / 2), POWISLAND);
			p = 1 - fmax(fmin(1, p), 0);
			//1 at the center, 0 at the corners

			//from 0 to 1			
			p1 = 0.5 * perlin(x * resolution / s * 3, y * resolution / s * 3, seeds[0]) + 0.5;
			p2 = 0.5 * perlin((x + 0.1) * resolution / s * 7, (y + 0.11) * resolution / s * 7, seeds[1]) + 0.5;
			p3 = 0.5 * perlin(x * resolution / s * 35, y * resolution / s * 35, seeds[2]) + 0.5;
			p4 = 0.5 * perlin((x + 0.31) * resolution / s * 30, (y + 0.31) * resolution / s * 30, seeds[3]) + 0.5;
			p5 = 0.5 * perlin((x + 0.73) * resolution / s * 20, (y + 0.73) * resolution / s * 20, seeds[0]) + 0.5;

			c = ((p / 1 + p1 / 1 + p2 / 1.8 + p3 / 4 + p4 / 6 + p5 / 7) / (1 / 1 + 1 / 1 + 1 / 1.8 + 1 / 4 + 1 / 6 + 1 / 7)) * 205;
			c = fmin(fmax(0, c), 255);


			noiseIm.setPixel(x, y, sf::Color(c, c, c));
		}
		if ((int)y % 25 == 0) std::cout << "calculated heightmap of line " << y << '\n';
	}*/

	noiseIm.saveToFile("../img/test.png");
	sf::Image rsim; rsim.create(MAP_SIZE.x, MAP_SIZE.y); resizeImage(noiseIm, rsim);
	sf::Texture t; t.loadFromImage(rsim);
	return t;
}




sf::Texture heightToColors(sf::Image heightmap) {
	sf::Color c;
	sf::Vector2u size = heightmap.getSize();
	//sf::Image o; o.loadFromFile("../img/colors.png");

	//find average color of the heightmap
	float avg = 0;
	for (float i = 0; i < (size.y + size.x) / 2; i++) {
		avg += heightmap.getPixel(rand() % size.x, rand() % size.y).r;
	}
	avg /= (size.y + size.x) / 2;


	int v;
	for (float y = 0; y < heightmap.getSize().y; y++) {
		for (float x = 0; x < heightmap.getSize().x; x++) {
			v = heightmap.getPixel(x, y).r;

			if (v > avg * 1.1) c = sf::Color::White;
			else if (v > avg * 1) c = sf::Color(0, 70, 150);
			else c = sf::Color::Black;

			//c = o.getPixel(v/1.3, 0);
			heightmap.setPixel(x, y, c);
		}
	}

	//just to don't create a new texture
	sf::Texture t;
	t.loadFromImage(heightmap);
	return t;
}


sf::Texture newMap(sf::Vector2f MAP_SIZE, int resolution, int fractalLike, float& advancement) {
	//		std::cout << "calculated heightmap of prov" << provIndex << '\n';

	MAP_SIZE.x = ceil(MAP_SIZE.x / resolution) * resolution;
	MAP_SIZE.y = ceil(MAP_SIZE.y / resolution) * resolution;

	sf::Image mapImg; mapImg.create(MAP_SIZE.x/resolution, MAP_SIZE.y/resolution, sf::Color::White);

	//generate provinces used as "cells" on a voronoï diagram, helps making more fractal world but with not so much islands
	int numberOfProvinces = sqrt(MAP_SIZE.x/resolution*MAP_SIZE.y/resolution) * (fractalLike+1);
	std::vector<Province*> provinces;
	generateCellsMap(numberOfProvinces, mapImg, provinces, advancement);
	//advancement at 80% after generating the cells

	//generate the rest of the heightmap
	sf::Texture noiseMap;
	noiseMap.create(MAP_SIZE.x, MAP_SIZE.y);
	advancement = 85.0;
	noiseMap = generateNoiseHeightmap(MAP_SIZE, resolution, provinces, mapImg);
	advancement = 100.0;
	//noiseMap.copyToImage().saveToFile("../img/map15.png");
	std::cout << "End of map generation\n";

	return noiseMap;//mapTxt.getTexture();
}