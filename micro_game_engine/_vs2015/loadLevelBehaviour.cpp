#include <iostream>
#include "loadLevelBehaviour.h"
#include "SFML\Window\Keyboard.hpp"
#include "SFML\Window\Joystick.hpp"
#include "Script.h"

loadLevelBehaviour::loadLevelBehaviour(World* world) : AbstractBehaviour(), _world(world) {

}

loadLevelBehaviour::~loadLevelBehaviour() {

}

void loadLevelBehaviour::loadLevel(std::function<void(ResourceSet)> pAction, ResourceSet pSet) 
{
	// Call the action lol
	pAction(pSet);
}

void loadLevelBehaviour::update(float pStep) 
{
	if (Script::instance().getStartGame()) {

		for (int i = _world->getChildCount() - 1; i >= 0; --i) {
			_world->registerDeathMarked(_world->getChildAt(i));
		}

		//std::cout << "NIGGA I AM READY TO GO, LEMME GO" << std::endl;
		loadLevel(levelToCall, Script::instance().getTileSet());

		Script::instance().setStartGame(false);
	}
}