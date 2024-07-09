#ifndef PARENTBEHAVIOUR_H
#define PARENTBEHAVIOUR_H

#include "mge/behaviours/AbstractBehaviour.hpp"
#include "mge/core/AbstractGame.hpp"
#include "mge/core/GameObject.hpp"
#include <SFML/Graphics.hpp>

class ParentBehaviour : public AbstractBehaviour
{
public:
	ParentBehaviour(GameObject* pParent);

	virtual ~ParentBehaviour();
	virtual void update(float pStep);

private:
	GameObject* _parent;
	glm::mat4 _parentPos;
};

#endif