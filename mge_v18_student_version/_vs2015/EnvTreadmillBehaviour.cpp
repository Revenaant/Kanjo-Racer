#include "EnvTreadmillBehaviour.h"
#include "mge/core/Mesh.hpp"
#include "mge/core/World.hpp"
#include "ResourceManager.h"
#include <algorithm>

static std::vector<GameObject*> _currentTread;

EnvTreadmillBehaviour::EnvTreadmillBehaviour(float pSpeed, float pDistanceThreshold, ResourceSet pSet) : AbstractBehaviour(), _speed(pSpeed), _distanceThreshold(pDistanceThreshold), _resourceSet(pSet)
{
}

EnvTreadmillBehaviour::~EnvTreadmillBehaviour()
{
}


void EnvTreadmillBehaviour::update(float pStep)
{
	// Adds the new objects to the tread
	if (_addedToCurrent == false) {
		_currentTread.push_back(_owner);
		std::cout << _currentTread.size() << std::endl;
		_addedToCurrent = true;
	}

	// Moves the tread
	_owner->translate(glm::vec3(0.0f, 0.0f, _speed * pStep));

	// Instantiate the new tread
	if (_owner->getLocalPosition().z > _distanceThreshold) {
		std::cout << "I has reached" << std::endl;

		// Get the current mesh for the position of the new object
		//Mesh* mesh = _owner->getMesh();
		Mesh* mesh = _currentTread.back()->getMesh();
		if (mesh == nullptr) std::cout << "This mesh is null:" << std::endl;
		glm::vec3 size = mesh->size(mesh);

		// Create and place new object
		GameObject* newg = new GameObject("new Treadmill", glm::vec3(0, 0, _currentTread.back()->getLocalPosition().z - (size.z * 0.5f)));
		Mesh* nMesh = ResourceManager::instance().pullMeshSet(_resourceSet);
		newg->setMesh(nMesh);
		newg->setMaterial(ResourceManager::instance().pullMaterial(nMesh));
		std::cout << newg->getLocalPosition() << std::endl;

		// Place in correct position
		newg->setLocalPosition(glm::vec3(0, 0, newg->getLocalPosition().z - (newg->getMesh()->size(newg->getMesh()).z * 0.5f) + 10));

		// Add this behaviour to the new object
		newg->addBehaviour(new EnvTreadmillBehaviour(_speed, _distanceThreshold, _resourceSet));
		_owner->getWorldObject()->add(newg);

		// Destroy the old object
		_currentTread.erase(std::remove(_currentTread.begin(), _currentTread.end(), _owner), _currentTread.end());
		_owner->getWorld()->registerDeathMarked(_owner);
	}
}

void EnvTreadmillBehaviour::addElement(GameObject * pObject)
{
	_elements.push_back(pObject);
}
