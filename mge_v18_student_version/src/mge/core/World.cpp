#include <iostream>
#include <algorithm>

#include "mge/core/World.hpp"
#include "mge/core/Light.hpp"
#include "BulletCollision\Gimpact\btGImpactCollisionAlgorithm.h"

World::World():GameObject("root"), _mainCamera(0), _deathMarkedObjects(), _dynamicsWorld(nullptr)
{
	_world = this;
}

void World::setMainCamera (Camera* pCamera) {
    if (pCamera != NULL) _mainCamera = pCamera;
}

Camera* World::getMainCamera () {
    return _mainCamera;
}


void World::registerLight (Light* pLight) {
    std::cout << "Registering light " << pLight->getName() << std::endl;
    _lights.push_back(pLight);
}

void World::unregisterLight (Light* pLight) {
    std::cout << "Unregistering light " << pLight->getName() << std::endl;
	if (_lights.size() == 0) return;
    _lights.erase(std::remove(_lights.begin(), _lights.end(), pLight), _lights.end());
}

void World::registerDeathMarked(GameObject* pObject) {
	_deathMarkedObjects.push_back(pObject);
}

void World::unregisterDeathMarked(GameObject* pObject) {
	if (_deathMarkedObjects.size() == 0) return;
	_deathMarkedObjects.erase(std::remove(_deathMarkedObjects.begin(), _deathMarkedObjects.end(), pObject), _deathMarkedObjects.end());
}

std::vector<GameObject*> World::getDeathMarks() const {
	return _deathMarkedObjects;
}

btDiscreteDynamicsWorld* World::dynamicsWorld() {
	if (!_dynamicsWorld) {
		btBroadphaseInterface* broadphase = new btDbvtBroadphase();
		btDefaultCollisionConfiguration* collisionConfiguration = new btDefaultCollisionConfiguration();
		btCollisionDispatcher* dispatcher = new btCollisionDispatcher(collisionConfiguration);
		btGImpactCollisionAlgorithm::registerAlgorithm(dispatcher);
		btSequentialImpulseConstraintSolver* solver = new btSequentialImpulseConstraintSolver;

		_dynamicsWorld = new btDiscreteDynamicsWorld(dispatcher, broadphase, solver, collisionConfiguration);
		_dynamicsWorld->setGravity(btVector3(0, -10, 0));
		return _dynamicsWorld;
	}
	else {
		return _dynamicsWorld;
	}

}

Light* World::getLightAt (int pIndex) {
    return _lights[pIndex];
}

int World::getLightCount() {
    return _lights.size();
}
