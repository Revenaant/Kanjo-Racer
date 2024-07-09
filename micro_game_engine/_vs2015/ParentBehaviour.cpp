#include "ParentBehaviour.h"

ParentBehaviour::ParentBehaviour(GameObject * pParent):AbstractBehaviour(), _parent(pParent)
{
}

ParentBehaviour::~ParentBehaviour()
{
}

void ParentBehaviour::update(float pStep)
{
	_parentPos = _parent->getTransform();
	_owner->setTransform(_parentPos);
}
