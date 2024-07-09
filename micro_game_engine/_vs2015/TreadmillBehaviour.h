#ifndef TREADMILL_BEHAVIOUR_H
#define TREADMILL_BEHAVIOUR_H

#include <vector>
#include "mge/behaviours/AbstractBehaviour.hpp"
#include "mge/core/GameObject.hpp"
#include "Resource.h"
#include "bullet.h"
#include "btBulletDynamicsCommon.h"

class TreadmillBehaviour : public AbstractBehaviour 
{
public:
	TreadmillBehaviour(float pDistanceThreshold = 50, ResourceSet pSet = ResourceSet::Normal);
	virtual ~TreadmillBehaviour();
	virtual void update(float pStep);

	// Adds a new element to the list of new possible segments to spawn
	void addElement(GameObject* pObject);

	// Spawns obstacles in the area
	void spawnObstacles(int nObstacles, GameObject* spawnArea, float meshSize);

	void initRigidbody(glm::vec3 pBounds, float bodyMass = 1);
	void createObstacleBody(GameObject* pObstacle, glm::vec3 pBounds, float bodyMass = 1);
	btRigidBody* createRigidbody(GameObject* pObject, glm::vec3 pBounds, float bodyMass = 1);

	static void setSpeed(float value);


private:

	const float _distanceThreshold = 50;
	ResourceSet _resourceSet = ResourceSet::Normal;

	btRigidBody* _rigidbody;
	bool _addedToCurrent = false;
	std::vector<GameObject*> _elements;
};

#endif

