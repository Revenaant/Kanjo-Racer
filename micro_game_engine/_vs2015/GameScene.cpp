#pragma region Includes
#include <iostream>
#include <string>

#include "glm.hpp"
#include <memory>
#include <functional>

#include "mge/core/Renderer.hpp"
#include "mge/core/Mesh.hpp"
#include "mge/core/World.hpp"
#include "mge/core/Texture.hpp"
#include "mge/core/Light.hpp"
#include "mge/core/Camera.hpp"
#include "mge/core/GameObject.hpp"

#include "mge/materials/AbstractMaterial.hpp"
#include "mge/materials/ColorMaterial.hpp"
#include "mge/materials/TextureMaterial.hpp"

#include "mge/behaviours/RotatingBehaviour.hpp"
#include "mge/behaviours/KeysBehaviour.hpp"

#include "mge/util/DebugHud.hpp"
#include "mge/config.hpp"
#include "Hud.h"

#include "CameraOrbitBehaviour.h"
#include "ControllerBehaviour.h"
#include "TreadmillBehaviour.h"
#include "EnvTreadmillBehaviour.h"
#include "PositionTrackerBehaviour.h"
#include "loadLevelBehaviour.h"
#include "PickupBehaviour.h"
#include "ResourceManager.h"
#include "GameScene.h"
#include "Ridgidbody.h"
#include "MusicManager.h"

#include <windows.h>
#include <Xinput.h>

//#include "chrono\physics\ChSystemNSC.h"
//#include "chrono\physics\ChBodyEasy.h"

#include "../_vs2015/Script.h"

#pragma endregion

GameScene::GameScene(bool debug) :AbstractGame(), _hud(0)
{
	Script::instance().setDebugging(debug);
	_gameIsInitialized = false;
}

GameScene::~GameScene()
{
	exitPhysics();
}

void GameScene::initialize()
{
	//setup the core part
	AbstractGame::initialize();

	Script::instance().initialize();
	Script::instance().init();
}

PositionTrackerBehaviour* distanceTracker = nullptr;

void GameScene::_initializeScene()
{
	initializeMenu();
	ResourceManager::instance().loadCarMeshes();
}

void GameScene::initializeMenu() 
{
	_menuHud = new Hud(_window);
	Script::instance().setHud(_menuHud);

	_countDownHud = new Hud(_window);

	Script::instance().canvas = new GameObject("plane", glm::vec3(0, 0, 0));
	_world->add(Script::instance().canvas);
	Script::instance().canvas->addBehaviour(new loadLevelBehaviour(_world));
	Script::instance().canvas->getBehaviour<loadLevelBehaviour>()->levelToCall = [this](ResourceSet pSet) { initializeGame(pSet); };
	/*std::function<void(ResourceSet)> thing = &GameScene::initializeGame;
	Script::instance().canvas->getBehaviour<loadLevelBehaviour>()->levelToCall = thing;*/
	
	Mesh* planeMeshDefault = Mesh::load(config::MGE_MODEL_PATH + "plane.obj");
	AbstractMaterial* brickMaterial = new TextureMaterial(Texture::load(config::MGE_TEXTURE_PATH + "bricks.jpg"));

	Camera* camera = new Camera("camera", glm::vec3(0, 0, 50));
	_world->add(camera);
	_world->setMainCamera(camera);

	//add the floor
	GameObject* canvas = new GameObject("plane", glm::vec3(0, 0, 0));
	canvas->scale(glm::vec3(50, 50, 50));
	canvas->rotate(1.57f, glm::vec3(1, 0, 0));
	canvas->setMesh(planeMeshDefault);
	canvas->setMaterial(brickMaterial);
	//_world->add(canvas);

	std::cout << "IM HERE YAYAAAYAYA" << std::endl;
	Light* light = new Light("light", glm::vec3(0, 4, 0));
	light->scale(glm::vec3(0.1f, 0.1f, 0.1f));
	_world->add(light);

	//canvas->addBehaviour(new loadLevelBehaviour(_world));
	//canvas->getBehaviour<loadLevelBehaviour>()->levelToCall = [this]() { initializeGame(); };

	//camera->addBehaviour(new CameraOrbitBehaviour(3.0f, 1.0f, 50.0f, 2.5f, plane, _window, this));

	MusicManager::instance().playMusicSet(ResourceSet::Menu);
	MusicManager::instance().playSound("P2Startup.ogg");
}

GameObject* Model1;
GameObject* Model3;

btRigidBody* carRigidBody;
btRigidBody* carRigidBody2;

void GameScene::initializeGame(ResourceSet pSet) 
{
	//setup the custom part so we can display some text
	std::cout << "Initializing HUD" << std::endl;
	_hud = new DebugHud(_window);
	std::cout << "HUD initialized." << std::endl << std::endl;

	_gameHud = new Hud(_window);
	Script::instance().setHud(_gameHud);
	_gameHud->addSprite("IN_GAME/INGAME_HUD.png");

	_resolutionScreen = new Hud(_window);
	_resolutionScreenIsActiveP1 = false;
	_resolutionScreenIsActiveP2 = false;

	//if (_loadData == true) 
	{
		switch (pSet) {
		case ResourceSet::Beach:
			ResourceManager::instance().loadBeach();
			break;
		case ResourceSet::Mountain:
			ResourceManager::instance().loadMountain();
			break;
		case ResourceSet::Village:
			ResourceManager::instance().loadVillage();
			break;
		case ResourceSet::Forest:
			ResourceManager::instance().loadForest();
			break;
		case ResourceSet::Kanjo:
			ResourceManager::instance().loadKanjo();
			break;
		case ResourceSet::City:
			ResourceManager::instance().loadCity();
			break;
		}
	}


	//MESHES AND MATERIALS
#pragma region Meshes and Materials
	//MESHES

	//load a bunch of meshes we will be using throughout this demo
	//each mesh only has to be loaded once, but can be used multiple times:
	//F is flat shaded, S is smooth shaded (normals aligned or not), check the models folder!
	//Mesh* planeMeshDefault = Mesh::load(config::MGE_MODEL_PATH + "plane.obj");
	//Mesh* cubeMeshF = Mesh::load(config::MGE_MODEL_PATH + "cube_flat.obj");
	//Mesh* cylinderMesh = Mesh::load(config::MGE_MODEL_PATH + "cylinder_smooth.obj");

	//Mesh* Mesh1 = Mesh::load(config::MGE_MODEL_PATH + config::MESH_ONE);
	//Mesh* Mesh2 = Mesh::load(config::MGE_MODEL_PATH + config::MESH_TWO);
	//Mesh* Mesh3 = Mesh::load(config::MGE_MODEL_PATH + config::MESH_THREE);

	//Mesh* High2 = Mesh::load(config::MGE_MODEL_PATH + "Highway_bridge.obj");

	//ResourceManager::instance().addMesh(Mesh1, "car1", ResourceSet::Cars);
	//ResourceManager::instance().addMesh(Mesh3, "car2", ResourceSet::Cars);

	//// Road
	//Mesh* road1 = Mesh::load(config::MGE_MODEL_PATH + "Road_set_1.obj");
	//Mesh* road2 = Mesh::load(config::MGE_MODEL_PATH + "Road_set_2.obj");
	//Mesh* road3 = Mesh::load(config::MGE_MODEL_PATH + "Road_set_3.obj");
	//Mesh* road4 = Mesh::load(config::MGE_MODEL_PATH + "Road_set_4.obj");

	//ResourceManager::instance().addMesh(road1, "road1", ResourceSet::Normal);
	//ResourceManager::instance().addMesh(road2, "road2", ResourceSet::Normal);
	//ResourceManager::instance().addMesh(road3, "road3", ResourceSet::Normal);
	//ResourceManager::instance().addMesh(road4, "road4", ResourceSet::Normal);

	//// Environment
	//Mesh* Environment1 = Mesh::load(config::MGE_MODEL_PATH + "Environment_set_1.obj");
	//Mesh* Environment2 = Mesh::load(config::MGE_MODEL_PATH + "Environment_set_2.obj");
	//Mesh* Environment3 = Mesh::load(config::MGE_MODEL_PATH + "Environment_set_3.obj");

	//ResourceManager::instance().addMesh(Environment1, "environment1", ResourceSet::KanjoEnvironment);
	//ResourceManager::instance().addMesh(Environment2, "environment2", ResourceSet::KanjoEnvironment);
	//ResourceManager::instance().addMesh(Environment3, "environment3", ResourceSet::KanjoEnvironment);

	//// Extra
	//ResourceManager::instance().addMesh(Mesh2, "Highway", ResourceSet::Mountain);
	//ResourceManager::instance().addMesh(High2, "Bridge", ResourceSet::Mountain);

	////MATERIALS

	////create some materials to display the cube, the plane and the light
	//AbstractMaterial* lightMaterial = new ColorMaterial(glm::vec3(0.5f, 0.5f, 0.5f));
	//AbstractMaterial* brickMaterial = new TextureMaterial(Texture::load(config::MGE_TEXTURE_PATH + "bricks.jpg"));
	AbstractMaterial* groundMaterial = new TextureMaterial(Texture::load(config::MGE_TEXTURE_PATH + "Gradient test.png"));

	//AbstractMaterial* Texture1 = new TextureMaterial(Texture::load(config::MGE_TEXTURE_PATH + config::TEXTURE_ONE));
	//AbstractMaterial* Texture2 = new TextureMaterial(Texture::load(config::MGE_TEXTURE_PATH + config::TEXTURE_TWO));
	//AbstractMaterial* Texture3 = new TextureMaterial(Texture::load(config::MGE_TEXTURE_PATH + config::TEXTURE_THREE));

	//ResourceManager::instance().addMaterial(Texture2, "Highway");
#pragma endregion

	//SCENE SETUP
#pragma region Object loading
	//add camera first (it will be updated last)
	Camera* camera = new Camera("camera", glm::vec3(30, 45, 30));
	camera->rotate(glm::radians(60.0f), glm::vec3(0, 1, 0));
	camera->rotate(glm::radians(-60.0f), glm::vec3(1, 0, 0));
	_world->add(camera);
	_world->setMainCamera(camera);

	//add the floor
	//GameObject* plane = new GameObject("plane", glm::vec3(0, 0, 0));
	//plane->scale(glm::vec3(5, 5, 5));
	//plane->setMesh(planeMeshDefault);
	//plane->setMaterial(groundMaterial);
	//_world->add(plane);

	//Ridgidbody* rb = new Ridgidbody(glm::vec3(1, 1, 1), 200);

	/*GameObject*/ Model1 = new GameObject("Model_one", glm::vec3(config::POSITION_ONE[0], config::POSITION_ONE[1], config::POSITION_ONE[2]));
	Model1->scale(glm::vec3(config::SCALE_ONE[0], config::SCALE_ONE[1], config::SCALE_ONE[2]));
	Model1->rotate(config::ROTATION_ONE * glm::pi<float>() / 180, glm::vec3(0, 1, 0));
	Mesh* p1Mesh = ResourceManager::instance().pullMesh("player1");
	Model1->setMesh(p1Mesh);
	Model1->setMaterial(ResourceManager::instance().pullCarMat(p1Mesh));

	//rb->setLocalPosition(glm::vec3(0, 5, 0));
	//Model1->setLocalPosition(rb->getLocalPosition());
	std::cout << "POS" << Model1->getWorldPosition() << std::endl;
	/*rb->ridgidbody->SetMass(-500);
	rb->ridgidbody->SetInertiaXX(chrono::ChVector<>(20, 20, 20));
	rb->ridgidbody->SetBodyFixed(true);
	rb->ridgidbody->RemoveAllForces();*/


	// Set no gravity
	//my_system.Set_G_acc(chrono::ChVector<>(0, 20000000, 0));

	//Model1->AddRidgidbody(glm::vec3(1, 1, 1), 500);
	//my_system.AddBody(Model1->ridgidbody);
	if (config::ADD_ONE == true) _world->add(Model1);
	//_world->add(rb);
	//Model1->setParent(camera);


#pragma region SHIT




	GameObject* Model2 = new GameObject("Model_two", glm::vec3(0, 0, config::POSITION_TWO[2]));
	Model2->scale(glm::vec3(config::SCALE_TWO[0], config::SCALE_TWO[1], config::SCALE_TWO[2]));
	Model2->rotate(config::ROTATION_TWO * glm::pi<float>() / 180, glm::vec3(0, 1, 0));
	Mesh* m2Mesh = ResourceManager::instance().pullMesh("straightMesh");
	Model2->setMesh(m2Mesh);
	Model2->setMaterial(ResourceManager::instance().pullMaterial(m2Mesh));

	GameObject* Highway2 = new GameObject("Model_two", glm::vec3(0, 0, Model2->getLocalPosition().z - Model2->getMesh()->size(m2Mesh).z));
	Highway2->scale(glm::vec3(config::SCALE_TWO[0], config::SCALE_TWO[1], config::SCALE_TWO[2]));
	Highway2->rotate(config::ROTATION_TWO * glm::pi<float>() / 180, glm::vec3(0, 1, 0));
	Mesh* h2Mesh = ResourceManager::instance().pullMesh("straightMesh");
	Highway2->setMesh(h2Mesh);
	Highway2->setMaterial(ResourceManager::instance().pullMaterial(h2Mesh));

	GameObject* Highway3 = new GameObject("Model_two", glm::vec3(0, 0, Highway2->getLocalPosition().z - Highway2->getMesh()->size(h2Mesh).z));
	Highway3->scale(glm::vec3(config::SCALE_TWO[0], config::SCALE_TWO[1], config::SCALE_TWO[2]));
	Highway3->rotate(config::ROTATION_TWO * glm::pi<float>() / 180, glm::vec3(0, 1, 0));
	Mesh* h3Mesh = ResourceManager::instance().pullMesh("bridgeMesh");
	Highway3->setMesh(h3Mesh);
	Highway3->setMaterial(ResourceManager::instance().pullMaterial(h3Mesh));

	GameObject* Highway4 = new GameObject("Model_two", glm::vec3(0, 0, Highway3->getLocalPosition().z - Highway3->getMesh()->size(h3Mesh).z));
	Highway4->scale(glm::vec3(config::SCALE_TWO[0], config::SCALE_TWO[1], config::SCALE_TWO[2]));
	Highway4->rotate(config::ROTATION_TWO * glm::pi<float>() / 180, glm::vec3(0, 1, 0));
	Mesh* h4Mesh = ResourceManager::instance().pullMesh("straightMesh");
	Highway4->setMesh(h4Mesh);
	Highway4->setMaterial(ResourceManager::instance().pullMaterial(h4Mesh));

	// ADD STARTING HIGHWAY FROM FARTHEST TO CLOSEST, OR THE ORDER IN THE LIST BREAKS
	if (config::ADD_TWO == true) _world->add(Highway4);
	if (config::ADD_TWO == true) _world->add(Highway3);
	if (config::ADD_TWO == true) _world->add(Highway2);
	if (config::ADD_TWO == true) _world->add(Model2);

	/*GameObject*/ Model3 = new GameObject("Model_three", glm::vec3(config::POSITION_THREE[0], config::POSITION_THREE[1], config::POSITION_THREE[2]));
	Model3->scale(glm::vec3(config::SCALE_THREE[0], config::SCALE_THREE[1], config::SCALE_THREE[2]));
	Model3->rotate(config::ROTATION_THREE * glm::pi<float>() / 180, glm::vec3(0, 1, 0));
	Mesh* p3Mesh = ResourceManager::instance().pullMesh("player2");
	Model3->setMesh(p3Mesh);
	Model3->setMaterial(ResourceManager::instance().pullCarMat(p3Mesh));
	if (config::ADD_THREE == true) _world->add(Model3);

	//GameObject* Pickup = new GameObject("Pickup", glm::vec3(0, 3, -10));
	//Pickup->scale(glm::vec3(5,5,5));
	//Pickup->rotate(45 * glm::pi<float>() / 180, glm::vec3(0, 1, 0));
	//Mesh* PickupMesh = Mesh::load(config::MGE_MODEL_PATH + "Fuel.obj");
	//Pickup->setMesh(PickupMesh);
	//Pickup->setMaterial(new TextureMaterial(Texture::load(config::MGE_TEXTURE_PATH + "Fuel.png")));
	//_world->add(Pickup);

	//_world->colliders.push_back(Pickup->createCollider((ColliderTypee::Pickup), glm::vec3(10, 10, 5)));
	//Pickup->addBehaviour(new PickupBehaviour());

	// ENVIRONMENT
	//GameObject* Env1 = new GameObject("Env1", glm::vec3(0, 0, 0));
	//Env1->setMesh(Environment1);
	//Env1->setMaterial(Texture2);
	//_world->add(Env1);

	//GameObject* Env2 = new GameObject("Env2", glm::vec3(0, 0, Env1->getLocalPosition().z - Env1->getMesh()->size(Environment1).z * 0.5f));
	//Env2->setMesh(Environment2);
	//Env2->setMaterial(Texture2);
	//_world->add(Env2);

	//Env2->setLocalPosition(glm::vec3(0, 0, Env2->getLocalPosition().z - (Env2->getMesh()->size(Env2->getMesh()).z * 0.5f) + 1));

	//GameObject* Env3 = new GameObject("Env3", glm::vec3(0, 0, Env2->getLocalPosition().z - Env2->getMesh()->size(Environment2).z * 0.5f));
	//Env3->setMesh(Environment3);
	//Env3->setMaterial(Texture2);
	//_world->add(Env3);

	//Env3->setLocalPosition(glm::vec3(0, 0, Env3->getLocalPosition().z - (Env3->getMesh()->size(Env3->getMesh()).z * 0.5f) + 1));

	////add a light. Note that the light does ABSOLUTELY ZIP! NADA ! NOTHING !
	////It's here as a place holder to get you started.
	////Note how the texture material is able to detect the number of lights in the scene
	////even though it doesn't implement any lighting yet!
	//Light* light = new Light("light", glm::vec3(0, 4, 0));
	//light->scale(glm::vec3(0.1f, 0.1f, 0.1f));
	//light->setMesh(cubeMeshF);
	//light->setMaterial(lightMaterial);
	//_world->add(light);
#pragma endregion

	//BEHAVIOUR SETUP
	//camera->addBehaviour(new CameraOrbitBehaviour(3.0f, 1.0f, 50.0f, 2.5f, Model3, _window, this));
	//camera->getBehaviour<CameraOrbitBehaviour>()->_player1 = Model1;
	//camera->getBehaviour<CameraOrbitBehaviour>()->_player2 = Model3;

	//Model1->addBehaviour < std::make_unique<PositionTrackerBehaviour>(10, Model1->getLocalPosition());
	//Model1->addBehaviour(new ControllerBehaviour(0, 5, 5));
	//Model3->addBehaviour(new ControllerBehaviour(1, 5, 5));

	// Track
	float trackSpeed = 80;

	Model2->addBehaviour(new TreadmillBehaviour(200, pSet));
	Highway2->addBehaviour(new TreadmillBehaviour(200, pSet));
	Highway3->addBehaviour(new TreadmillBehaviour(200, pSet));
	Highway4->addBehaviour(new TreadmillBehaviour(200, pSet));
	TreadmillBehaviour::setSpeed(80);

	// Environment
	//Env1->addBehaviour(new EnvTreadmillBehaviour(trackSpeed, 100, ResourceSet::KanjoEnvironment));
	//Env2->addBehaviour(new EnvTreadmillBehaviour(trackSpeed, 100, ResourceSet::KanjoEnvironment));
	//Env3->addBehaviour(new EnvTreadmillBehaviour(trackSpeed, 100, ResourceSet::KanjoEnvironment));

	// Other
	//Model1->addBehaviour(new KeysBehaviour(5.0f, 45.0f, vehicle));
	Model1->addBehaviour(new ControllerBehaviour(0, 50000, 50000));
	Model1->addBehaviour(new PositionTrackerBehaviour(60, Model1->getLocalPosition(),_window));

	Model3->addBehaviour(new ControllerBehaviour(1, 50000, 50000));
	Model3->addBehaviour(new PositionTrackerBehaviour(60, Model3->getLocalPosition(),_window));
	distanceTracker = Model3->getBehaviour<PositionTrackerBehaviour>();
	//sphere4->setBehaviour(new KeysBehaviour(5.0f, 45.0f));
	_camera = camera;
	//camera->setBehaviour(new KeysBehaviour(-5, 50));

#pragma endregion

#pragma region Physics



	btDefaultMotionState* carMotionState =
		new btDefaultMotionState(btTransform(btQuaternion(0, 0, 0, 1), btVector3(3, 1, 0)));

	btDefaultMotionState* carMotionState2 =
		new btDefaultMotionState(btTransform(btQuaternion(0, 0, 0, 1), btVector3(0, 1, 0)));

	btVector3 halfExtends(1, btScalar(0.25f), btScalar(2.75f));
	btCollisionShape* carShape = new btBoxShape(halfExtends);

	// Make AABB Colliders
	glm::vec3 colliderSize = glm::vec3(halfExtends.x(), halfExtends.y(), halfExtends.z());
	_world->colliders.push_back(Model1->createCollider(ColliderTypee::Player, colliderSize));
	_world->colliders.push_back(Model3->createCollider(ColliderTypee::Player, colliderSize));

	btVector3 localInertia(0, 0, 0);
	carShape->calculateLocalInertia(1200, localInertia);


	btTransform localTransform;
	localTransform.setIdentity();
	localTransform.setOrigin(btVector3(0, -1, 0));

	btRigidBody::btRigidBodyConstructionInfo
		carRigidBodyCI(1200, carMotionState, carShape, btVector3(0, 0, 0));
	/*btRigidBody*/ carRigidBody = new btRigidBody(carRigidBodyCI);
	//carRigidBody = chassisRigidBody;
		/////GAYYYYYYYYYY
	btRigidBody::btRigidBodyConstructionInfo
		carRigidBodyCI2(1200, carMotionState2, carShape, btVector3(0, 0, 0));
	/*btRigidBody*/ carRigidBody2 = new btRigidBody(carRigidBodyCI2);

	carRigidBody->setDamping(0.9f,0.9f);
	carRigidBody->setFriction(3.2f);

	carRigidBody2->setDamping(0.9f, 0.9f);
	carRigidBody2->setFriction(3.2f);

	_world->dynamicsWorld()->addRigidBody(carRigidBody);
	_world->dynamicsWorld()->addRigidBody(carRigidBody2);

	#pragma region Vehicle Test 

	//btCompoundShape* compound = new btCompoundShape();
	//compound->addChildShape(localTransform, carShape);
	//btRigidBody* chassisRigidBody = this->createChassisRigidBodyFromShape(compound);

	//btVehicleRaycaster* vehicleRayCaster = new btDefaultVehicleRaycaster(_world->dynamicsWorld());

	//btRaycastVehicle::btVehicleTuning tuning;

	////Creates a new instance of the raycast vehicle
	//this->vehicle = new btRaycastVehicle(tuning, chassisRigidBody, vehicleRayCaster);

	////Never deactivate the vehicle
	//chassisRigidBody->setActivationState(DISABLE_DEACTIVATION);

	////Adds the vehicle to the world
	//_world->dynamicsWorld()->addVehicle(this->vehicle);

	////Adds the wheels to the vehicle
	//this->addWheels(&halfExtends, this->vehicle, tuning);
#pragma endregion


	// Ground
	
	btDefaultMotionState* groundMotionState =
		new btDefaultMotionState(btTransform(btQuaternion(0, 0, 0, 1), btVector3(0, 0, 0)));

	btDefaultMotionState* leftWallMotionState =
		new btDefaultMotionState(btTransform(btQuaternion(0, 0, 0, 1), btVector3(-10, 0, 0)));

	btDefaultMotionState* rightWallMotionState =
		new btDefaultMotionState(btTransform(btQuaternion(0, 0, 0, 1), btVector3(10, 0, 0)));


	//Creates the ground shape
	btCollisionShape* groundShape = new btBoxShape(btVector3(100, 0.5f, 100));

	btCollisionShape* leftShape = new btBoxShape(btVector3(1, 10, 100));

	btCollisionShape* rightShape = new btBoxShape(btVector3(1, 10, 100));

	GameObject* leftWall = new GameObject("leftWall", glm::vec3(0,0,0));
	GameObject* rightWall = new GameObject("rightWall", glm::vec3(0, 0, 0));

	glm::vec3 wallSize = glm::vec3(1, 10, 100);
	//_world->colliders.push_back(leftWall->createCollider(ColliderTypee::Wall, wallSize));
	//_world->colliders.push_back(rightWall->createCollider(ColliderTypee::Wall, wallSize));


	//Stores on an array for reusing
	collisionShapes.push_back(groundShape);


	btRigidBody::btRigidBodyConstructionInfo
		groundRigidBodyCI(0, groundMotionState, groundShape, btVector3(0, 0, 0));
	btRigidBody* groundRigidBody = new btRigidBody(groundRigidBodyCI);

	btRigidBody::btRigidBodyConstructionInfo
		leftRigidBodyCI(0, leftWallMotionState, leftShape, btVector3(0, 0, 0));
	btRigidBody* leftRigidBody = new btRigidBody(leftRigidBodyCI);

	btRigidBody::btRigidBodyConstructionInfo
		rightRigidBodyCI(0, rightWallMotionState, rightShape, btVector3(0, 0, 0));
	btRigidBody* rightRigidBody = new btRigidBody(rightRigidBodyCI);

	//Creates the ground rigidbody
	//btRigidBody* groundRigidBody = this->createGroundRigidBodyFromShape(groundShape);

	//Adds it to the world
	_world->dynamicsWorld()->addRigidBody(groundRigidBody);
	_world->dynamicsWorld()->addRigidBody(leftRigidBody);
	_world->dynamicsWorld()->addRigidBody(rightRigidBody);


#pragma endregion
	//auto rb = std::make_shared<chrono::ChBody>();
	//auto marker = std::make_shared<chrono::ChMarker>();
	//auto force = std::make_shared<chrono::ChForce>();

	//rb->AddMarker(marker);
	//rb->AddForce(force);
	/*my_system.SetMaxPenetrationRecoverySpeed(1.6);  // used by Anitescu stepper only
	my_system.SetMaxItersSolverSpeed(40);
	my_system.SetMaxItersSolverStab(20);  // unuseful for Anitescu, only Tasora uses this
	my_system.SetSolverWarmStarting(true);
	my_system.SetParallelThreadNumber(4);

	my_system.AddBody(rb->ridgidbody);

	my_system.SetupInitial();

	my_system.DoEntireDynamics();*/
	//chrono::ChSystemNSC physicsSystem;
	//auto rb = std::make_shared<chrono::ChBody>();
	//auto marker = std::make_shared<chrono::ChMarker>();
	//auto force = std::make_shared<chrono::ChForce>();

	//rb->AddMarker(marker);
	//rb->AddForce(force);
	//physicsSystem.AddBody(rb);
	//

	//chrono::ChBodyEasyBox* boxy = NULL;

	MusicManager::instance().playMusicSet(pSet);

	_gameIsInitialized = true;
}

void GameScene::_render()
{
	AbstractGame::_render();
	_updateHud();
}

void GameScene::_updateHud()
{
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::U)) {
		_world->gameDone = !_world->gameDone;
		MusicManager::instance().playSound("P1Startup.ogg");
	}

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::I)) {
		MusicManager::instance().playSound("P2Startup.ogg");

	}

	float dt = 1.0f / 60.0f;
	Script::instance().update(dt);
	/*#pragma region ControllerAddforceTesting
	float joystickX = sf::Joystick::getAxisPosition(0, sf::Joystick::X);
	float joystickY = sf::Joystick::getAxisPosition(0, sf::Joystick::Y);
	if (joystickX > 20) {
		carRigidBody->applyForce(btVector3(50000, 0, 0), btVector3(0, 0, 1));
		std::cout << "I happen" << std::endl;
	}
	else if (joystickX < -20) carRigidBody->applyForce(btVector3(-50000, 0, 0), btVector3(0, 0, 1));
	#pragma endregion*/
	if (!_gameIsInitialized)  _menuHud->draw(); 
	else if (_startingSounds)
	{
		if(_countDownHud->getImageCount() > 0)_countDownHud->clearHud();

		_startTimer += dt;
		carRigidBody->setLinearVelocity(btVector3(0, 0, 0));
		carRigidBody2->setLinearVelocity(btVector3(0, 0, 0));
		_world->gameDone = true;

		if (_startTimer < 0.2f)
		{
			MusicManager::instance().playSound("StartSound.wav");
			_countDownHud->addSprite("IN_GAME/3.png");
		}
		else if (_startTimer > 0.4f && _startTimer < 0.6f) 
		{
			_countDownHud->addSprite("IN_GAME/2.png");
		}
		else if (_startTimer > 0.6f && _startTimer < 0.8f)
		{
			_countDownHud->addSprite("IN_GAME/1.png");
		}
		if (_startTimer > 0.8f)
		{
			MusicManager::instance().playSound("P1Startup.ogg");
			MusicManager::instance().playSound("P2Startup.ogg");
			_countDownHud->addSprite("IN_GAME/GO!.png");
			_world->gameDone = false;
		}

		if(_countDownHud != nullptr) _countDownHud->draw();

		if (_startTimer > 0.81f)
		{
			_countDownHud->clearHud();
			delete _countDownHud;
			_startingSounds = false;
			_world->gameDone = false;

			MusicManager::instance().playSound("P1ConstantSpeed.ogg").setLoop(true);
			MusicManager::instance().playSound("P2ConstantSpeed.ogg").setLoop(true);
		}
	}
	else
	{
		// Pause behaviour
		//if (_world->gameDone) return;

		if (Model3->getLocalPosition().z < -50 || Model1->getLocalPosition().z < -50) {
			carRigidBody->applyCentralForce(btVector3(0, 0, 85000));
			carRigidBody2->applyCentralForce(btVector3(0, 0, 85000));
			TreadmillBehaviour::setSpeed(90);
		}
		else {
			carRigidBody->applyCentralForce(btVector3(0, 0, 27000));
			carRigidBody2->applyCentralForce(btVector3(0, 0, 27000));
			TreadmillBehaviour::setSpeed(80);
		}

		//if(carRigidBody->getLinearVelocity().z() < -40000)
			carRigidBody->applyCentralForce(Model3->getBehaviour<ControllerBehaviour>()->move(0, dt));
			carRigidBody2->applyCentralForce(Model1->getBehaviour<ControllerBehaviour>()->move(1, dt));

		btTransform trans;
		btTransform trans2;
		carRigidBody->getMotionState()->getWorldTransform(trans);

		glm::vec3 testPos(trans.getOrigin().getX(), trans.getOrigin().getY(), trans.getOrigin().getZ());
		Model3->setLocalPosition(testPos);
		Model3->rotate(glm::radians(trans.getRotation().getY()) * glm::pi<float>() / 180, glm::vec3(0, 1, 0));

		carRigidBody2->getMotionState()->getWorldTransform(trans2);
		glm::vec3 testPos2(trans2.getOrigin().getX(), trans2.getOrigin().getY(), trans2.getOrigin().getZ());
		Model1->setLocalPosition(testPos2);
		Model1->rotate(glm::radians(trans2.getRotation().getY()) * glm::pi<float>() / 180, glm::vec3(0, 1, 0));
		//std::cout << "sphere height: " << trans.getOrigin().getY() << std::endl;
		_world->dynamicsWorld()->stepSimulation(dt);
		handleCollisions();

		if (trans.getOrigin().getY() < 1.5f) carRigidBody->applyCentralForce(btVector3(0, 18000, 0));

		if (trans2.getOrigin().getY() < 1.5f) carRigidBody2->applyCentralForce(btVector3(0, 18000, 0));
		

		std::string debugInfo = "";
		debugInfo += std::string("FPS:") + std::to_string((int)_fps) + "\n";
		glm::vec3 _cameraPos = _camera->getLocalPosition();
		debugInfo += std::string("Camera Pos:") + std::to_string(_cameraPos.x) + " " +
			std::to_string(_cameraPos.y) + " " + std::to_string(_cameraPos.z) + "\n";

		debugInfo += std::string("Distance Traveled: ") + std::to_string(distanceTracker->getTraveledDistance()) + "\n";
		//debugInfo += std::string("Camera Angle") + std::to_string(_camera->getTransform())


		if (sf::Keyboard::isKeyPressed(sf::Keyboard::O)) {
			initializeGame(ResourceSet::Kanjo);
		}

		if (Model1->getBehaviour<PositionTrackerBehaviour>()->getPlayerHasLost() && !_resolutionScreenIsActiveP1)
		{
			Script::instance().setHud(_resolutionScreen);
			//_resolutionScreen->addSprite("RESOLUTION_SCREEN/P2_WON.png");
			_resolutionScreenIsActiveP1 = true;
			Script::instance().setIsResScreen(true);
			Script::instance().setWinningPlayer(2);
			_resolutionScreen->draw();
		}
		else if (Model3->getBehaviour<PositionTrackerBehaviour>()->getPlayerHasLost() && !_resolutionScreenIsActiveP2)
		{
			Script::instance().setHud(_resolutionScreen);
			//_resolutionScreen->addSprite("RESOLUTION_SCREEN/P1_WON.png");
			_resolutionScreenIsActiveP2 = true;
			Script::instance().setIsResScreen(true);
			Script::instance().setWinningPlayer(1);
			_resolutionScreen->draw();
			//carRigidBody2->applyCentralForce(btVector3(0,0,1000000));
		}

		if(_resolutionScreenIsActiveP1) carRigidBody->applyCentralForce(btVector3(0, 0, -800000));
		else if (_resolutionScreenIsActiveP2)carRigidBody2->applyCentralForce(btVector3(0, 0, -800000));
	
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::R)) {
			restartLevel(ResourceSet::Beach);
		}

		_hud->setDebugInfo(debugInfo);
		//_hud->draw();
		//_gameHud->setText(std::string("Distance Traveled: ") + std::to_string(distanceTracker->getTraveledDistance()), sf::Vector2f(_window->getSize().x / 2, 150));
		_gameHud->draw();
		_resolutionScreen->draw();
	}
}

void GameScene::loadMountain() {
	ResourceManager::instance().loadMountain();
	initializeGame(ResourceSet::Mountain);
}

void GameScene::loadForest() {
	ResourceManager::instance().loadForest();
	initializeGame(ResourceSet::Forest);
}

void GameScene::loadKanjo() {
	ResourceManager::instance().loadKanjo();
	initializeGame(ResourceSet::Kanjo);
}

void GameScene::loadVillage() {
	ResourceManager::instance().loadVillage();
	initializeGame(ResourceSet::Village);
}

void GameScene::loadBeach() {
	ResourceManager::instance().loadBeach();
	initializeGame(ResourceSet::Beach);
}

void GameScene::loadCity() {
	ResourceManager::instance().loadCity();
	initializeGame(ResourceSet::City);
}

void GameScene::restartLevel(ResourceSet pSet) {

	_world->gameDone = true;

	for (int i = _world->getChildCount() - 1; i >= 0; --i) {
		_world->registerDeathMarked(_world->getChildAt(i));
	}
	_world->colliders.clear();
	for (int i = _world->dynamicsWorld()->getNumCollisionObjects() - 1; i >= 0; i--)
	{
		btCollisionObject* obj = _world->dynamicsWorld()->getCollisionObjectArray()[i];
		btRigidBody* body = btRigidBody::upcast(obj);
		if (body && body->getMotionState())
		{
			while (body->getNumConstraintRefs())
			{
				btTypedConstraint* constraint = body->getConstraintRef(0);
				_world->dynamicsWorld()->removeConstraint(constraint);
				delete constraint;
			}
			delete body->getMotionState();
			_world->dynamicsWorld()->removeRigidBody(body);
		}
		else
		{
			_world->dynamicsWorld()->removeCollisionObject(obj);
		}
		delete obj;
	}

	//delete collision shapes
	for (int j = 0; j< collisionShapes.size(); j++)
	{
		btCollisionShape* shape = collisionShapes[j];
		delete shape;
	}
	collisionShapes.clear();


	_loadData = false;
	GameObject* loader = new GameObject("loader", glm::vec3(0, 0, 0));
	loader->addBehaviour(new loadLevelBehaviour(_world));
	_world->add(loader);

	loader->getBehaviour<loadLevelBehaviour>()->loadLevel([this](ResourceSet pSet) { initializeGame(pSet); }, pSet);

	_loadData = true;
	_world->gameDone = false;
}


void GameScene::handleCollisions() 
{
	for (int i = 0; i < _world->colliders.size(); i++) 
	{
		Collider* c1 = _world->colliders[i];
		c1->update();

		if (c1->getObject() == nullptr) continue;
		GameObject* obj1 = c1->getObject();
		Mesh* mesh1 = obj1->getMesh();
		if (mesh1 == nullptr) continue;

		glm::vec3 size1 = mesh1->size(mesh1);

		for (int j = i + 1; j < _world->colliders.size();j++)
		{
			Collider* c2 = _world->colliders[j];
			c2->update();

			if (c2->getObject() == nullptr) continue;
			GameObject* obj2 = c2->getObject();
			Mesh* mesh2 = obj2->getMesh();
			if (mesh2 == nullptr) continue;

			glm::vec3 size2 = mesh2->size(mesh2);

			if (c1->collides(c2)) 
			{
				if (c1->isCollidingWith.count(c2) == 0)	// First time objects collide
				{
					c1->isCollidingWith.emplace(c2, false);
				}

				if (c1->isCollidingWith[c2] == false) // OnCollisionEnter
				{
					// Player against player
					if (c1->getType() == ColliderTypee::Player && c2->getType() == ColliderTypee::Player) {

						MusicManager::instance().playSoundRandom();
					}

					// Player against Pickup
					if (c1->getType() == ColliderTypee::Player && c2->getType() == ColliderTypee::Pickup) {
						std::cout << "gay shit1" << std::endl;
						c2->getObject()->getBehaviour<PickupBehaviour>()->Activate(c1->getObject());
					}
					else if (c2->getType() == ColliderTypee::Player && c1->getType() == ColliderTypee::Pickup) {
						std::cout << "gay shit2" << std::endl;
						c1->getObject()->getBehaviour<PickupBehaviour>()->Activate(c2->getObject());
					}

					// Player against NPC
					if ((c1->getType() == ColliderTypee::Player && c2->getType() == ColliderTypee::CarCol)
						|| (c1->getType() == ColliderTypee::CarCol && c2->getType() == ColliderTypee::Player))
					{
						std::cout << "BOOOOM GO DA CAR AND DA ODA CAR AND CAR CAR" << std::endl;
				
						MusicManager::instance().playSoundRandom();
					}

					// Player against wall
					if ((c1->getType() == ColliderTypee::Player && c2->getType() == ColliderTypee::Wall)
						|| (c1->getType() == ColliderTypee::Wall && c2->getType() == ColliderTypee::Player))
					{
						std::cout << "walllllllllllllllllllllllllllllllllllllllllll." << std::endl;
						MusicManager::instance().playSoundRandom();
					}


					if (i == 0 || i == 1)
					{
						XINPUT_VIBRATION vibration;
						vibration.wLeftMotorSpeed = 16383;
						vibration.wRightMotorSpeed = 16383;
						XInputSetState(i - 1, &vibration);
					}

					//std::cout << "ME, " << c1->getObject()->getName() << " COLLIDE: " << c2->getObject()->getName() << std::endl;
					c1->isCollidingWith[c2] = true;
				}

				// OnCollisionStay
				// stuff
			}
			else if (c1->isCollidingWith[c2] == true) // OnCollisionExit
			{
				c1->isCollidingWith[c2] = false;
			
				if (i == 0 || i == 1)
				{
					XINPUT_VIBRATION vibration;
					vibration.wLeftMotorSpeed = 0;
					vibration.wRightMotorSpeed = 0;
					XInputSetState(i - 1, &vibration);
				}
			}
		}
	}
}



/*

{

	int numManifolds = _world->dynamicsWorld()->getDispatcher()->getNumManifolds();
	for (int i = 0; i < numManifolds; i++)
	{
		btPersistentManifold* contactManifold = _world->dynamicsWorld()->getDispatcher()->getManifoldByIndexInternal(i);
		const btCollisionObject* obA = contactManifold->getBody0();
		const btCollisionObject* obB = contactManifold->getBody1();

		int numContacts = contactManifold->getNumContacts();
		for (int j = 0; j < numContacts; j++)
		{
			btManifoldPoint& pt = contactManifold->getContactPoint(j);
			//if (pt.getDistance() < 0.f)
			//{
			//	const btVector3& ptA = pt.getPositionWorldOnA();
			//	const btVector3& ptB = pt.getPositionWorldOnB();
			//	const btVector3& normalOnB = pt.m_normalWorldOnB;
			//}

			if (pt.getDistance() < 0.1f) {
				const btVector3& ptA = pt.getPositionWorldOnA();
				const btVector3& ptB = pt.getPositionWorldOnB();
				const btVector3& normalOnB = pt.m_normalWorldOnB;

				if (obA->getUserPointer() == nullptr) std::cout << "Dis is bad" << std::endl;
				if (obB->getUserPointer() == nullptr) std::cout << "Dis is bad2" << std::endl;

				GameObject* thisObject = (GameObject*)(obA->getUserPointer());
				GameObject* thatObject = (GameObject*)(obB->getUserPointer());
				if (thisObject == nullptr) {
					//std::cout << "thisObject is null" << std::endl;
					return;
				}

				//std::cout << "Thingy one: " << thisObject->getName() << " other thingy: " << thatObject->getName() << std::endl;	

				if (thisObject->getName() == "Model_one") {
					std::cout << "1---HOMMOIN SOOOOOOO HARRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRD" << std::endl;
				}
				//if (thatObject->getName() == "Model_three") {
				//	std::cout << "2---HOMMOIN SOOOOOOO HARRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRD" << std::endl;
				//}
				//if ((thisObject->getName() == "Model_one") && (thatObject->getName() == "Model_three")) {
				//	std::cout << "3---HOMMOIN SOOOOOOO HARRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRD" << std::endl;
				//}

				//if (thisObject->getCollisionListener() != null) {
				//	thisObject->getCollisionListener().collided(
				//		ptA, ptB, normalOnB, thatObject);
				//}
				//if (thatObject->getCollisionListener() != null) {
				//	thatObject->getCollisionListener().collided(
				//		ptB, ptA, normalOnB, thisObject);
				//}
			}
		}
	}
}

*/


btRigidBody* GameScene::createChassisRigidBodyFromShape(btCollisionShape* chassisShape)
{
	btTransform chassisTransform;
	chassisTransform.setIdentity();
	chassisTransform.setOrigin(btVector3(0, 50, 0));

	{
		//chassis mass 
		btScalar mass(1200);

		//since it is dynamic, we calculate its local inertia
		btVector3 localInertia(0, 0, 0);
		chassisShape->calculateLocalInertia(mass, localInertia);

		//using motionstate is optional, it provides interpolation capabilities, and only synchronizes 'active' objects
		btDefaultMotionState* groundMotionState = new btDefaultMotionState(chassisTransform);
		btRigidBody::btRigidBodyConstructionInfo rbInfo(mass, groundMotionState, chassisShape, localInertia);

		return new btRigidBody(rbInfo);
	}
}

btRigidBody* GameScene::createGroundRigidBodyFromShape(btCollisionShape* groundShape)
{
	btTransform groundTransform;
	groundTransform.setIdentity();
	groundTransform.setOrigin(btVector3(0, -1, 0));

	{
		//The ground is not dynamic, we set its mass to 0
		btScalar mass(0);

		//No need for calculating the inertia on a static object
		btVector3 localInertia(0, 0, 0);

		//using motionstate is optional, it provides interpolation capabilities, and only synchronizes 'active' objects
		btDefaultMotionState* groundMotionState = new btDefaultMotionState(groundTransform);
		btRigidBody::btRigidBodyConstructionInfo rbInfo(mass, groundMotionState, groundShape, localInertia);

		return new btRigidBody(rbInfo);
	}
}

//void GameScene::addWheels(
//	btVector3* halfExtents,
//	btRaycastVehicle* vehicle,
//	btRaycastVehicle::btVehicleTuning tuning)
//{
//	//The direction of the raycast, the btRaycastVehicle uses raycasts instead of simiulating the wheels with rigid bodies
//	btVector3 wheelDirectionCS0(0, -1, 0);
//
//	//The axis which the wheel rotates arround
//	btVector3 wheelAxleCS(-1, 0, 0);
//
//	btScalar suspensionRestLength(0.7);
//
//	btScalar wheelWidth(0.4);
//
//	btScalar wheelRadius(0.5);
//
//	//The height where the wheels are connected to the chassis
//	btScalar connectionHeight(-1.2);
//
//	//All the wheel configuration assumes the vehicle is centered at the origin and a right handed coordinate system is used
//	btVector3 wheelConnectionPoint(halfExtents->x() - wheelRadius, connectionHeight, halfExtents->z() - wheelWidth);
//
//	//Adds the front wheels
//	vehicle->addWheel(wheelConnectionPoint, wheelDirectionCS0, wheelAxleCS, suspensionRestLength, wheelRadius, tuning, true);
//
//	vehicle->addWheel(wheelConnectionPoint * btVector3(-1, 1, 1), wheelDirectionCS0, wheelAxleCS, suspensionRestLength, wheelRadius, tuning, true);
//
//	//Adds the rear wheels
//	vehicle->addWheel(wheelConnectionPoint* btVector3(1, 1, -1), wheelDirectionCS0, wheelAxleCS, suspensionRestLength, wheelRadius, tuning, false);
//
//	vehicle->addWheel(wheelConnectionPoint * btVector3(-1, 1, -1), wheelDirectionCS0, wheelAxleCS, suspensionRestLength, wheelRadius, tuning, false);
//
//	//Configures each wheel of our vehicle, setting its friction, damping compression, etc.
//	//For more details on what each parameter does, refer to the docs
//	for (int i = 0; i < vehicle->getNumWheels(); i++)
//	{
//		btWheelInfo& wheel = vehicle->getWheelInfo(i);
//		wheel.m_suspensionStiffness = 50;
//		wheel.m_wheelsDampingCompression = btScalar(0.3) * 2 * btSqrt(wheel.m_suspensionStiffness);//btScalar(0.8);
//		wheel.m_wheelsDampingRelaxation = btScalar(0.5) * 2 * btSqrt(wheel.m_suspensionStiffness);//1;
//																								  //Larger friction slips will result in better handling
//		wheel.m_frictionSlip = btScalar(1.2);
//		wheel.m_rollInfluence = 1;
//	}
//}

void GameScene::exitPhysics()
{
	for (int i = _world->dynamicsWorld()->getNumCollisionObjects() - 1; i >= 0; i--)
	{
		btCollisionObject* obj = _world->dynamicsWorld()->getCollisionObjectArray()[i];
		btRigidBody* body = btRigidBody::upcast(obj);
		if (body && body->getMotionState())
		{
			while (body->getNumConstraintRefs())
			{
				btTypedConstraint* constraint = body->getConstraintRef(0);
				_world->dynamicsWorld()->removeConstraint(constraint);
				delete constraint;
			}
			delete body->getMotionState();
			_world->dynamicsWorld()->removeRigidBody(body);
		}
		else
		{
			_world->dynamicsWorld()->removeCollisionObject(obj);
		}
		delete obj;
	}

	//delete collision shapes
	for (int j = 0; j< collisionShapes.size(); j++)
	{
		btCollisionShape* shape = collisionShapes[j];
		delete shape;
	}
	collisionShapes.clear();

	btConstraintSolver* constraintSolver = _world->dynamicsWorld()->getConstraintSolver();
	btBroadphaseInterface* broadphaseInterface = _world->dynamicsWorld()->getBroadphase();
	btDispatcher* collisionDispatcher = _world->dynamicsWorld()->getDispatcher();

	delete _world->dynamicsWorld();

	delete constraintSolver;
	delete broadphaseInterface;
	delete collisionDispatcher;
}