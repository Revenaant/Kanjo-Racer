#include "PickupBehaviour.h"
#include "mge\core\GameObject.hpp"
#include "mge\core\World.hpp"
#include "MusicManager.h"

#include "SFML\Window.hpp"

PickupBehaviour::PickupBehaviour() {

}

PickupBehaviour::~PickupBehaviour() {

}

void PickupBehaviour::update(float pStep) {
	if (_owner->getWorld()->gameDone == true) return;

	_owner->rotate(0.02f, glm::vec3(0, 1, 0));

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::K)) {
		std::cout << " Pos: " << _owner->getLocalPosition() << " collider center: " << _owner->getCollider()->getCenter() << std::endl;
		Activate(_owner);
	}
}

void PickupBehaviour::Activate(GameObject* pActivator) {
	_owner->getWorld()->colliders.erase(std::remove(_owner->getWorld()->colliders.begin(),
		_owner->getWorld()->colliders.end(),
		_owner->getCollider()),

		_owner->getWorld()->colliders.end());

	// Do stuff
	_owner->getWorld()->registerDeathMarked(_owner);
	MusicManager::instance().playSound("P1Startup.ogg");
	std::cout << "FUEL BOIII" << std::endl;
}