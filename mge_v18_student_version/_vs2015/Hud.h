#ifndef HUD_H
#define HUD_H

#include <SFML/Graphics.hpp>
#include <iostream>
#include <vector>

class Hud 
{
public:
	Hud(sf::RenderWindow * aWindow);
	virtual ~Hud();
	void draw();

	void addSprite(const std::string& pFilename);
	void clearHud();
	void setText(std::string pInfo, sf::Vector2f position);
	void setSpritePosition(int spriteIndex, sf::Vector2f position);
	int getImageCount();

private:

	sf::RenderWindow * _window;

	sf::Font _font;
	sf::Text _text;
	std::vector<sf::Texture*> _images;
	std::vector<sf::Sprite> _sprites;

	void _createHud();

	Hud(const Hud&);
	Hud& operator=(const Hud&);

};

#endif	// HUD_H
