#ifndef OBSTACLE_CAR_H
#define OBSTACLE_CAR_H

#include "mge\behaviours\AbstractBehaviour.hpp"
#include "Resource.h"
#include "bullet.h"
#include "btBulletDynamicsCommon.h"

class ObstacleCarBehaviour : public AbstractBehaviour {
public:
	ObstacleCarBehaviour(float pSpeed = 5, float pDistanceThreshold = 50, float pLifetime = 60);
	virtual ~ObstacleCarBehaviour();

	virtual void update(float pStep);

	btRigidBody* getRigidbody();
	void setRigidBody(btRigidBody* pBody);
	void deleteRigidbody();

private:
	float _speed;
	const float _distanceThreshold = 50;
	ResourceSet _resourceSet = ResourceSet::Normal;

	btRigidBody* _rigidbody;

	bool _moving = false;
	float _startX = 0;
	float _destX = 0;
	int _direction[2] = { -1, 1 };
	float _lifetime = 60000;

};

#endif // !OBSTACLE_CAR_H

