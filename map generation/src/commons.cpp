#include "commons.h"


sf::Color HSVtoRGB(float H, float S, float V) {   // h:0-360.0, s:0.0-1.0, v:0.0-1.0
	S *= 100; V *= 100;
	if (H > 360 || H < 0 || S>100 || S < 0 || V>100 || V < 0) {
		std::cout << "The given HSV values are not in valid range" << "\n";
		return sf::Color(0, 0, 0);
	}
	float s = S / 100;
	float v = V / 100;
	float C = s * v;
	float X = C * (1 - abs(fmod(H / 60.0, 2) - 1));
	float m = v - C;
	float r, g, b;
	if (H >= 0 && H < 60) {
		r = C, g = X, b = 0;
	}
	else if (H >= 60 && H < 120) {
		r = X, g = C, b = 0;
	}
	else if (H >= 120 && H < 180) {
		r = 0, g = C, b = X;
	}
	else if (H >= 180 && H < 240) {
		r = 0, g = X, b = C;
	}
	else if (H >= 240 && H < 300) {
		r = X, g = 0, b = C;
	}
	else {
		r = C, g = 0, b = X;
	}
	int R = (r + m) * 255;
	int G = (g + m) * 255;
	int B = (b + m) * 255;

	return sf::Color(R, G, B);
}



sf::Vector3f RGBtoHSV(sf::Color RGB) {
	float fR = RGB.r/255; float fG = RGB.g / 255; float fB = RGB.b / 255;
	float fCMax = fmax(fmax(fR, fG), fB);
	float fCMin = fmin(fmin(fR, fG), fB);
	float fDelta = fCMax - fCMin;

	float fH; float fS; float fV;

	if (fDelta > 0) {
		if (fCMax == fR) {
			fH = 60 * (fmod(((fG - fB) / fDelta), 6));
		}
		else if (fCMax == fG) {
			fH = 60 * (((fB - fR) / fDelta) + 2);
		}
		else if (fCMax == fB) {
			fH = 60 * (((fR - fG) / fDelta) + 4);
		}

		if (fCMax > 0) {
			fS = fDelta / fCMax;
		}
		else {
			fS = 0;
		}

		fV = fCMax;
	}
	else {
		fH = 0;
		fS = 0;
		fV = fCMax;
	}

	if (fH < 0) {
		fH = 360 + fH;
	}

	return sf::Vector3f(fH, fS, fV);
}



void resizeImage(const sf::Image& originalImage, sf::Image& resizedImage)
{
	const sf::Vector2u originalImageSize{ originalImage.getSize() };
	const sf::Vector2u resizedImageSize{ resizedImage.getSize() };
	for (unsigned int y{ 0u }; y < resizedImageSize.y; ++y)
	{
		for (unsigned int x{ 0u }; x < resizedImageSize.x; ++x)
		{
			unsigned int origX{ static_cast<unsigned int>(static_cast<double>(x) / resizedImageSize.x * originalImageSize.x) };
			unsigned int origY{ static_cast<unsigned int>(static_cast<double>(y) / resizedImageSize.y * originalImageSize.y) };
			resizedImage.setPixel(x, y, originalImage.getPixel(origX, origY));
		}
	}
}


bool equalToVectu(sf::Vector2u& a, sf::Vector2u& b) {
	return ((a.x == b.x) & (a.y == b.y));
}



float dist(sf::Vector2f a, sf::Vector2f b) {
	return sqrt((b.x - a.x) * (b.x - a.x) + (b.y - a.y) * (b.y - a.y));
}
