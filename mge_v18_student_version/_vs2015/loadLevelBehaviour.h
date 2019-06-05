#ifndef LOAD_LEVEL_BEHAVIOUR_H
#define LOAD_LEVEL_BEHAVIOUR_H

#include "mge\behaviours\AbstractBehaviour.hpp"
#include "mge\core\World.hpp"
#include <functional>
#include "Resource.h"

class loadLevelBehaviour : public AbstractBehaviour {
public:
	loadLevelBehaviour(World* world);
	virtual ~loadLevelBehaviour();
	virtual void update(float pStep);
	void loadLevel(std::function<void(ResourceSet)> pAction, ResourceSet pSet);

	std::function<void(ResourceSet)> levelToCall;

private:
	World* _world;

};

#endif // !LOAD_LEVEL_BEHAVIOUR_H

