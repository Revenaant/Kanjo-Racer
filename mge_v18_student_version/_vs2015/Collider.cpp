#include "Collider.h"
#include "mge/core/Mesh.hpp"
#include "mge/core/GameObject.hpp"

Collider::Collider() {

}

Collider::Collider(GameObject* pObject, ColliderTypee pType, glm::vec3& pCenter, glm::vec3& pSize) {
	_gameObject = pObject;
	_type = pType;

	_center = pCenter;
	_size = pSize;
	_halfSize = pSize * 0.5f;

	_min = _center - _halfSize;
	_max = _center + _halfSize;
}

Collider::~Collider() {
	isCollidingWith.clear();
}

void Collider::update() {
	_center = _gameObject->getLocalPosition();
	_size = glm::vec3(0,0,0);
	if (_gameObject->getMesh() != nullptr) {
		_size = _gameObject->getMesh()->size(_gameObject->getMesh());
	}
	_halfSize = _size * 0.5f;

	_min = _center - _halfSize;
	_max = _center + _halfSize;
}

bool Collider::collides(Collider* other) {
	return this->getMax().x > other->getMin().x && this->getMin().x < other->getMax().x
		&& this->getMax().y > other->getMin().y && this->getMin().y < other->getMax().y
		&& this->getMax().z > other->getMin().z && this->getMin().z < other->getMax().z;
}

GameObject* Collider::getObject() const {
	return _gameObject;
}

ColliderTypee Collider::getType() const {
	return _type;
}

glm::vec3 Collider::getCenter() const {
	return _center;
}

glm::vec3 Collider::getSize() const {
	return _size;
}

glm::vec3 Collider::getMin() const {
	return _min;
}

glm::vec3 Collider::getMax() const {
	return _max;
}