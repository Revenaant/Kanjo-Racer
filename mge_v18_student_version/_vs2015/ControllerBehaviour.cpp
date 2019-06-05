#include "ControllerBehaviour.h"
#include "mge/core/GameObject.hpp"
#include <SFML/Window/Joystick.hpp>

#include "mge\core\Camera.hpp"
#include "mge\core\World.hpp"


ControllerBehaviour::ControllerBehaviour(int pIndex, float pMoveSpeedX, float pMoveSpeedY) : AbstractBehaviour(), _index(pIndex), _moveSpeedX(pMoveSpeedX), _moveSpeedY(pMoveSpeedY)
{
}

ControllerBehaviour::~ControllerBehaviour()
{
}

void ControllerBehaviour::callDaPolice() {

}

const float DEADZONE_X = 20;
const float DEADZONE_Y = 20;

void ControllerBehaviour::update(float pStep)
{
	if (_owner->getWorld()->gameDone) return;
	move(_index, pStep);
}

btVector3 ControllerBehaviour::move(int index, float pStep)
{
	float moveSpeedX = 0.0f; //default if no movement
	float moveSpeedY = 0.0f;

	float joystickX = sf::Joystick::getAxisPosition(index, sf::Joystick::X);
	float joystickY = sf::Joystick::getAxisPosition(index, sf::Joystick::Y);
	float joystickZ = sf::Joystick::getAxisPosition(index, sf::Joystick::Z);

	if (joystickX > DEADZONE_X || joystickX < -DEADZONE_X) {
		moveSpeedX = joystickX * (_moveSpeedX);

		//std::cout << moveSpeedX << std::endl;

	}

	//float joystickY = sf::Joystick::getAxisPosition(index, sf::Joystick::Y);
	int input = 0;

	//if (joystickY > DEADZONE_Y || joystickY < -DEADZONE_Y) {
	//	input = joystickY;
	//	std::cout << "the input: " << input << std::endl;
	//}

	/*if (joystickY > DEADZONE_Y || joystickY < -DEADZONE_Y) {
		moveSpeedY = joystickY * (_moveSpeedX);

	}
	if(sf::Joystick::isButtonPressed(index, 0))
	{
		//moveSpeedY = joystickY * (_moveSpeedY * 0.1f);
		moveSpeedY = -1 * (_moveSpeedY * 0.9f);
		std::cout << moveSpeedY << std::endl;

	}
	else if (sf::Joystick::isButtonPressed(index, 1)) { // Backwards
		moveSpeedY = 1 * (_moveSpeedY * 1.5f);
	}*/


	if (joystickZ > DEADZONE_X || joystickZ < -DEADZONE_X) {
		moveSpeedY = joystickZ * (_moveSpeedY);
	}
	//Camera* cam = new Camera()

	//_owner->translate(glm::vec3(moveSpeedX*pStep, 0.0f, moveSpeedY*pStep));

	return btVector3(moveSpeedX*pStep, 0.0f, moveSpeedY*pStep);
}
