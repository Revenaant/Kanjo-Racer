#include <cassert>
#include <iostream>

#include <GL/glew.h>
#include <SFML/Graphics/Text.hpp>
#include "Hud.h"
#include "mge/config.hpp"

Hud::Hud(sf::RenderWindow* aWindow) : _window(aWindow), _font(), _text()
{
	assert(_window != NULL);

	if (!_font.loadFromFile(config::MGE_FONT_PATH + config::FONT)) {
		std::cout << "Could not load font, exiting..." << std::endl;
		return;
	}

	_createHud();
}

Hud::~Hud()
{
}

void Hud::_createHud() {
	_text.setString("");
	_text.setFont(_font);
	_text.setCharacterSize(22);
	_text.setFillColor(sf::Color::White);
}

void Hud::addSprite(const std::string& pFilename) {
	sf::Texture* image = new sf::Texture();
	image->loadFromFile(config::MGE_MENU_HUD + pFilename);
	_images.push_back(image);

	sf::Sprite sprite;
	sprite.setTexture(*image);
	_sprites.push_back(sprite);
}

void Hud::clearHud()
{
	_sprites.clear();
}

void Hud::setSpritePosition(int spriteIndex, sf::Vector2f position) {
	_sprites[spriteIndex].setPosition(position);
}

int Hud::getImageCount()
{
	return _sprites.size();
}

void Hud::setText(std::string pInfo, sf::Vector2f position = {0, 0}) {
	_text.setString(pInfo);
	_text.setPosition(position);
}

void Hud::draw()
{
	//glDisable( GL_CULL_FACE );
	glActiveTexture(GL_TEXTURE0);
	_window->pushGLStates();
	
	for (int i = 0; i < _sprites.size(); i++) {
		_window->draw(_sprites[i]);
	}

	_window->draw(_text);
	_window->popGLStates();
}