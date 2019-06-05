#include "ObstacleCarBehaviour.h"
#include "mge\core\GameObject.hpp"
#include "mge\core\World.hpp"

#include "SFML\Window.hpp"
#include <algorithm>
#include <unordered_map>
#include <iterator>
#include <cstdio>

ObstacleCarBehaviour::ObstacleCarBehaviour(float pSpeed, float pDistanceThreshold, float pLifetime) : AbstractBehaviour(), _speed(pSpeed), _distanceThreshold(pDistanceThreshold), _lifetime(pLifetime)
{
}

ObstacleCarBehaviour::~ObstacleCarBehaviour() 
{
}

void ObstacleCarBehaviour::update(float pStep) 
{
	if (_lifetime <= 0) 
	{
		deleteRigidbody();
		return;
	}

	// Pause behaviour
	if (_owner->getWorld()->gameDone) return;

	_lifetime -= pStep;

	if(_rigidbody->getLinearVelocity().z() < 5000)
		_rigidbody->applyCentralForce(btVector3(0, 0, -10000));

	btTransform trans;
	_rigidbody->getMotionState()->getWorldTransform(trans);

	glm::vec3 testPos(trans.getOrigin().getX(), trans.getOrigin().getY(), trans.getOrigin().getZ());
	_owner->setLocalPosition(testPos);
	_owner->rotate(glm::radians(trans.getRotation().getY()) * glm::pi<float>() / 180, glm::vec3(0, 1, 0));

	//_owner->getCollider()->update();
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::L)) {
		std::cout << "RB: " << testPos << " Pos: " << _owner->getLocalPosition() << " collider center: " << _owner->getCollider()->getCenter() << std::endl;
	}

	glm::vec3 position = _owner->getLocalPosition();

	if (position.z > _distanceThreshold) {
		deleteRigidbody();
	}

	return;

	if (!_moving) 
	{
		if (glm::linearRand(0, 1500) < 5) 
		{
			_moving = true;
			_startX = position.x;

			int dir = _direction[glm::linearRand(0, 1)];
			if (_startX <= -6) dir = 1;
			else if (_startX >= 2) dir = -1;

			_destX = _startX + (4.0f * dir);
		}
	}
	else 
	{
		if (glm::abs(position.x - _destX) >= 0.1f) 
		{
			if (_startX < _destX) {
				_owner->translate(glm::vec3(2 * pStep, 0, 0));
				//_owner->rotate(1 * glm::pi<float>() / 180, glm::vec3(0, 1, 0));
			}
			else {
				_owner->translate(glm::vec3(-2 * pStep, 0, 0));
				//_owner->rotate(-1 * glm::pi<float>() / 180, glm::vec3(0, 1, 0));
			}
		}
		else 
		{
			_owner->setLocalPosition(glm::vec3(_destX, position.y, position.z));
			_moving = false;
		}
	}

	btTransform transform;
	_rigidbody->getMotionState()->getWorldTransform(transform);
	btVector3 newPos = btVector3(position.x, position.y, position.z);
	transform.setOrigin(newPos);
	_rigidbody->getMotionState()->setWorldTransform(transform);


	_owner->translate(glm::vec3(0.0f, 0.0f, _speed * pStep));
}

btRigidBody* ObstacleCarBehaviour::getRigidbody() 
{
	if (!_rigidbody) return nullptr;
	return _rigidbody;
}

void ObstacleCarBehaviour::setRigidBody(btRigidBody* pBody) 
{
	_rigidbody = pBody;
	_rigidbody->setFriction(0);
	_rigidbody->setLinearVelocity(btVector3(0, 0, 50));
}

void ObstacleCarBehaviour::deleteRigidbody()
{
	_owner->getWorld()->registerDeathMarked(_owner);

	if (_rigidbody && _rigidbody->getMotionState())
	{
		while (_rigidbody->getNumConstraintRefs())
		{
			btTypedConstraint* constraint = _rigidbody->getConstraintRef(0);
			_owner->getWorld()->dynamicsWorld()->removeConstraint(constraint);
			delete constraint;
		}
		delete _rigidbody->getMotionState();
		_owner->getWorld()->dynamicsWorld()->removeRigidBody(_rigidbody);
	}

	if (_owner->getCollider()) {
		_owner->getWorld()->colliders.erase(std::remove(_owner->getWorld()->colliders.begin(),
			_owner->getWorld()->colliders.end(),
			_owner->getCollider()),

			_owner->getWorld()->colliders.end());
		delete _owner->getCollider();
	}

	
	//erase(std::remove(_deathMarkedObjects.begin(), _deathMarkedObjects.end(), pObject), _deathMarkedObjects.end());
	delete _rigidbody->getCollisionShape();
}
