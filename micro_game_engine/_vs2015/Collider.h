#ifndef COLLIDER_H
#define COLLIDER_H

#include <glm.hpp>
#include <unordered_map>

enum ColliderTypee { Player, CarCol, Pickup, Wall };
class GameObject;
class Collider {

public:
	Collider();
	Collider(GameObject* pObject, ColliderTypee pType, glm::vec3& pCenter, glm::vec3& pSize);
	virtual ~Collider();

	GameObject* getObject() const;

	ColliderTypee getType() const;
	glm::vec3 getCenter() const;
	glm::vec3 getSize() const;
	glm::vec3 getMin() const;
	glm::vec3 getMax() const;
	std::unordered_map<Collider*, bool> isCollidingWith;

	bool collides(Collider* other);
	void update();

protected:
	GameObject* _gameObject;
	ColliderTypee _type;

	glm::vec3 _center;
	glm::vec3 _size;
	glm::vec3 _halfSize;
	glm::vec3 _min;
	glm::vec3 _max;
};

#endif // !COLLIDER_H

