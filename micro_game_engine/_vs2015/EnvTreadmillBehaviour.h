#ifndef ENV_TREADMILL_BEHAVIOUR_H
#define ENV_TREADMILL_BEHAVIOUR_H

#include <vector>
#include "mge/behaviours/AbstractBehaviour.hpp"
#include "mge/core/GameObject.hpp"
#include "Resource.h"

class EnvTreadmillBehaviour : public AbstractBehaviour
{
public:
	EnvTreadmillBehaviour(float pSpeed = 5, float pDistanceThreshold = 50, ResourceSet pSet = ResourceSet::Normal);
	virtual ~EnvTreadmillBehaviour();
	virtual void update(float pStep);

	// Adds a new element to the list of new possible segments to spawn
	void addElement(GameObject* pObject);

private:
	float _speed;
	const float _distanceThreshold = 50;
	ResourceSet _resourceSet = ResourceSet::Normal;

	bool _addedToCurrent = false;
	std::vector<GameObject*> _elements;
};

#endif

