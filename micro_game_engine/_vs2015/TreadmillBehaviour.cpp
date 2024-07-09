#include "TreadmillBehaviour.h"
#include "mge/core/Mesh.hpp"
#include "mge/core/World.hpp"
#include "ResourceManager.h"
#include "ObstacleCarBehaviour.h"
#include "bullet.h"
#include <algorithm>

static std::vector<GameObject*> _currentTread;
static float _speed;

TreadmillBehaviour::TreadmillBehaviour(float pDistanceThreshold, ResourceSet pSet) : AbstractBehaviour(), _distanceThreshold(pDistanceThreshold), _resourceSet(pSet)
{

}

TreadmillBehaviour::~TreadmillBehaviour()
{
}

void TreadmillBehaviour::setSpeed(float value) {
	_speed = value;
}

void TreadmillBehaviour::update(float pStep)
{
	// Pause behaviour
	if (_owner->getWorld()->gameDone) return;

	// Adds the new objects to the tread
	if (_addedToCurrent == false) {
		_currentTread.push_back(_owner);
		std::cout << "Tread size: " << _currentTread.size() << std::endl;
		_addedToCurrent = true;

		_owner->getMesh();

		glm::vec3 meshSize = _owner->getMesh()->size(_owner->getMesh());
		initRigidbody(glm::vec3(meshSize.x * 0.45f, 0.25f, meshSize.z*10), 0);

		// Spawn some obstacles 
		if(_resourceSet == ResourceSet::Beach) spawnObstacles(glm::linearRand(1, 3), _owner, meshSize.z);
		else if (_resourceSet == ResourceSet::Mountain) spawnObstacles(glm::linearRand(1, 5), _owner, meshSize.z);
		else if (_resourceSet == ResourceSet::Kanjo) spawnObstacles(glm::linearRand(1, 7), _owner, meshSize.z);

	}

	// Moves the tread
	_owner->translate(glm::vec3(0.0f, 0.0f, _speed * pStep));
	glm::vec3 position = _owner->getLocalPosition();

	// And it's rigidbody
	//btTransform transform;
	//_rigidbody->getMotionState()->getWorldTransform(transform);
	//btVector3 newPos = btVector3(position.x, position.y, position.z);
	//transform.setOrigin(newPos);
	//_rigidbody->getMotionState()->setWorldTransform(transform);
	//_rigidbody->applyCentralForce(btVector3(0, 1200000, 1000000));
	//_rigidbody->setWorldTransform(btTransform(btQuaternion(0, 0, 0, 1), btVector3(position.x, 0, position.z)));

	// Instantiate the new tread
	if (position.z > _distanceThreshold) {
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
		std::cout << "newG's Posish: " << newg->getLocalPosition() << std::endl;

		// Place in correct position
		float newgLength = newg->getMesh()->size(newg->getMesh()).z;
		newg->setLocalPosition(glm::vec3(0, 0, newg->getLocalPosition().z - (newgLength * 0.5f) + 1));

		// Add this behaviour to the new object
		newg->addBehaviour(new TreadmillBehaviour(_distanceThreshold, _resourceSet));
		_owner->getWorldObject()->add(newg);

		// Destroy the old object
		_currentTread.erase(std::remove(_currentTread.begin(), _currentTread.end(), _owner), _currentTread.end());
		_owner->getWorld()->registerDeathMarked(_owner);

		if (_rigidbody && _rigidbody->getMotionState())
		{
			while (_rigidbody->getNumConstraintRefs())
			{
				btTypedConstraint* constraint = _rigidbody->getConstraintRef(0);
				_owner->getWorld()->dynamicsWorld()->removeConstraint(constraint);
				delete constraint;
			}
			delete _rigidbody->getMotionState();
			_owner->getWorld()->dynamicsWorld()->removeRigidBody(_rigidbody);
		}

		delete _rigidbody->getCollisionShape();
	}
}


void TreadmillBehaviour::spawnObstacles(int nObstacles, GameObject* spawnArea, float meshSize) 
{
	float laneWidth = 4.0f;
	float carLength = 15.0f;
	int lane[] = { -1, 0, 1 };
	float maxZPos = (meshSize * 0.5f) / carLength;

	glm::vec3 parentPos = spawnArea->getLocalPosition();

	for (int i = 0; i < nObstacles; i++)
	{
		GameObject* obstacle = new GameObject("car", parentPos);
		Mesh* oMesh = ResourceManager::instance().pullCarMesh();
		obstacle->setMesh(oMesh);
		obstacle->setMaterial(ResourceManager::instance().pullCarMat(oMesh));
		_owner->getWorldObject()->add(obstacle);
		obstacle->addBehaviour(new ObstacleCarBehaviour(_speed, _distanceThreshold, 45));

		obstacle->setLocalPosition(glm::vec3
		((parentPos.x + (laneWidth * lane[glm::linearRand(0, 2)]) - 2.0f),
			1,
			parentPos.z + carLength * glm::linearRand(-maxZPos, maxZPos)));

		glm::vec3 obstacleSize = obstacle->getMesh()->size(obstacle->getMesh());
		createObstacleBody(obstacle, glm::vec3 (obstacleSize.x * 0.5f,
												obstacleSize.y * 0.5f,
												obstacleSize.z * 0.65f), 12000);
	}
}

void TreadmillBehaviour::addElement(GameObject * pObject)
{
	_elements.push_back(pObject);
}

void TreadmillBehaviour::createObstacleBody(GameObject* pObstacle, glm::vec3 pBounds, float bodyMass) 
{
	pObstacle->getBehaviour<ObstacleCarBehaviour>()->setRigidBody(createRigidbody(pObstacle, pBounds, bodyMass));

	// Create AABB Collider
	glm::vec3 colliderSize = glm::vec3(pBounds);
	pObstacle->getWorld()->colliders.push_back(pObstacle->createCollider(ColliderTypee::CarCol, colliderSize));
}

btRigidBody* TreadmillBehaviour::createRigidbody(GameObject* pObject, glm::vec3 pBounds, float bodyMass) 
{

	glm::vec3 obstaclePos = pObject->getLocalPosition();
	btDefaultMotionState* motionState =
		new btDefaultMotionState(btTransform(btQuaternion(0, 0, 0, 1), btVector3(obstaclePos.x, obstaclePos.y, obstaclePos.z)));

	btVector3 halfExtends(pBounds.x, 1.0f, pBounds.z);
	btCollisionShape* shape = new btBoxShape(halfExtends);

	btVector3 localInertia(0, 0, 0);
	shape->calculateLocalInertia(bodyMass, localInertia);

	btRigidBody::btRigidBodyConstructionInfo
		rigidBodyCI(bodyMass, motionState, shape, btVector3(0, 0, 0));

	btRigidBody* rigidbody = new btRigidBody(rigidBodyCI);
	_owner->getWorld()->dynamicsWorld()->addRigidBody(rigidbody);

	return rigidbody;
}

void TreadmillBehaviour::initRigidbody(glm::vec3 pBounds, float bodyMass)
{
	_rigidbody = createRigidbody(_owner, pBounds, bodyMass);
	//_rigidbody->setCollisionFlags(_rigidbody->getCollisionFlags() | btCollisionObject::CF_KINEMATIC_OBJECT);
}