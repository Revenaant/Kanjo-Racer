#include "PositionTrackerBehaviour.h"
#include "mge/core/GameObject.hpp"
#include <SFML/Window/Joystick.hpp>
#include "mge/core/World.hpp"

glm::vec3 lastPos;

PositionTrackerBehaviour::PositionTrackerBehaviour(float pLoseDistance, glm::vec3 pStartPos, sf::RenderWindow* pWindow) : AbstractBehaviour(), _loseDistance(pLoseDistance), _startPos(pStartPos), _window(pWindow)
{
	lastPos = _startPos;
}

PositionTrackerBehaviour::~PositionTrackerBehaviour()
{
}
const float DEADZONE_X = 20;
const float DEADZONE_Y = 20;


void PositionTrackerBehaviour::update(float pStep)
{


	glm::vec3 currentPos = _owner->getLocalPosition();
	if (lastPos != currentPos) {
	//std::cout << "This is the traveled distance: " << _traveledDistance << std::endl;
		_traveledDistance += -(currentPos - lastPos).z * 0.1f;
		_traveledDistance += 0.8f * pStep;
		lastPos = currentPos;
	}

	if (_owner->getWorld()->gameDone == false && currentPos.z > _loseDistance) {
		std::cout << "Player has lost!" << std::endl;
		_owner->getBehaviour<PositionTrackerBehaviour>()->setPlayerHasLost(true);
		_owner->getWorld()->gameDone = true;
	}
}

float PositionTrackerBehaviour::getTraveledDistance() const
{
	return _traveledDistance;
}

void PositionTrackerBehaviour::setPlayerHasLost(bool pHasLost)
{
	_playerHasLost = pHasLost;
}

bool PositionTrackerBehaviour::getPlayerHasLost()
{
	return _playerHasLost;
}
