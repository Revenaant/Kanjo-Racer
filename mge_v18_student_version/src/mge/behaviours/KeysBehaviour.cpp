#include "mge/behaviours/KeysBehaviour.hpp"
#include "mge/core/GameObject.hpp"
#include <SFML/Window/Keyboard.hpp>

KeysBehaviour::KeysBehaviour(float pMoveSpeed, float pTurnSpeed/* btRaycastVehicle* pVehicle*/): AbstractBehaviour(), _moveSpeed(pMoveSpeed), _turnSpeed(pTurnSpeed) /*_vehicle(pVehicle)*/
{
}

KeysBehaviour::~KeysBehaviour()
{
}

void KeysBehaviour::update( float pStep )
{
	//float xMoveSpeed = 0.0f; //default if no keys
	//float yMoveSpeed = 0.0f; //default if no keys
	//float zMoveSpeed = 0.0f; //default if no keys
	//float turnSpeed = 0.0f;

	//if ( sf::Keyboard::isKeyPressed( sf::Keyboard::Up ) || sf::Keyboard::isKeyPressed( sf::Keyboard::W)) {
	//	zMoveSpeed = _moveSpeed;
	//}
	//if ( sf::Keyboard::isKeyPressed( sf::Keyboard::Down ) || sf::Keyboard::isKeyPressed(sf::Keyboard::S)) {
	//	zMoveSpeed = -_moveSpeed;
	//}
	//if ( sf::Keyboard::isKeyPressed( sf::Keyboard::Left ) || sf::Keyboard::isKeyPressed(sf::Keyboard::A)) {
	//	xMoveSpeed = _moveSpeed;
	//}
	//if ( sf::Keyboard::isKeyPressed( sf::Keyboard::Right ) || sf::Keyboard::isKeyPressed(sf::Keyboard::D)) {
	//	xMoveSpeed = -_moveSpeed;
	//}
	//if ( sf::Keyboard::isKeyPressed( sf::Keyboard::Space )) {
	//	yMoveSpeed = _moveSpeed;
	//}
	//if ( sf::Keyboard::isKeyPressed( sf::Keyboard::LControl )) {
	//	yMoveSpeed = -_moveSpeed;
	//}
	////translate the object in its own local space
	//_owner->translate( glm::vec3(xMoveSpeed*pStep, yMoveSpeed*pStep, zMoveSpeed*pStep ) );

	//we can also translate directly, basically we take the z axis from the matrix
	//which is normalized and multiply it by moveSpeed*step, then we add it to the
	//translation component
	//glm::mat4 transform = _owner->getTransform();
	//transform[3] += transform[2] * moveSpeed*pStep;
	//_owner->setTransform(transform);





	//rotate the object in its own local space
	//_owner->rotate( glm::radians(turnSpeed*pStep), glm::vec3(0.0f, 1.0f, 0.0f ) );

	//NOTE:
	//The reason the above happens in the local space of the object and not in the world space
	//is that we use the _owner->translate / rotate shortcuts which in turn call glm::rotate / glm::translate
	//The methods multiply the current transform with the translation/rotation matrix from left to right
	//meaning everything happens in local space.


	//keyboardCallback(_vehicle);
}


//bool KeysBehaviour::keyboardCallback(btRaycastVehicle* vehicle)
//{
//	bool handled = false;
//
//	//Key pressed events
//	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right) || sf::Keyboard::isKeyPressed(sf::Keyboard::D))
//	{
//		vehicle->setSteeringValue(btScalar(0.6), 0);
//		vehicle->setSteeringValue(btScalar(0.6), 1);
//		handled = true;
//	}
//	else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left) || sf::Keyboard::isKeyPressed(sf::Keyboard::A))
//	{
//		vehicle->setSteeringValue(btScalar(-0.6), 0);
//		vehicle->setSteeringValue(btScalar(-0.6), 1);
//		handled = true;
//	}
//	//else
//	//{
//	//	vehicle->setSteeringValue(0, 0);
//	//	vehicle->setSteeringValue(0, 1);
//	//	handled = true;
//	//}
//
//
//	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down) || sf::Keyboard::isKeyPressed(sf::Keyboard::S))
//	{
//		vehicle->applyEngineForce(10000, 2);
//		vehicle->applyEngineForce(10000, 3);
//		handled = true;
//	}
//	else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up) || sf::Keyboard::isKeyPressed(sf::Keyboard::W))
//	{
//		vehicle->applyEngineForce(-5000, 2);
//		vehicle->applyEngineForce(-5000, 3);
//		handled = true;
//	}
//	//else
//	//{
//	//	vehicle->applyEngineForce(0, 2);
//	//	vehicle->applyEngineForce(0, 3);
//
//	//	//Default braking force, always added otherwise there is no friction on the wheels
//	//	vehicle->setBrake(10, 2);
//	//	vehicle->setBrake(10, 3);
//	//	handled = true;
//	//}
//
//	//Handbrake
//	if (sf::Keyboard::isKeyPressed(sf::Keyboard::LControl))
//	{
//		vehicle->setBrake(500, 2);
//		vehicle->setBrake(500, 3);
//		handled = true;
//	}
//	//else
//	//{
//	//	vehicle->setBrake(0, 2);
//	//	vehicle->setBrake(0, 3);
//	//	handled = true;
//	//}
//
//	return handled;
//}
