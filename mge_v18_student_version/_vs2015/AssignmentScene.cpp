#pragma region Includes
#include <iostream>
#include <string>

#include "glm.hpp"

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

#include "CameraOrbitBehaviour.h"
#include "ControllerBehaviour.h"
#include "AssignmentScene.h"

#pragma endregion

AssignmentScene::AssignmentScene(bool debug) :AbstractGame(), _hud(0) {}

void AssignmentScene::initialize() 
{
	//setup the core part
	AbstractGame::initialize();
	
	//setup the custom part so we can display some text
	std::cout << "Initializing HUD" << std::endl;
	_hud = new DebugHud(_window);
	std::cout << "HUD initialized." << std::endl << std::endl;
}

void AssignmentScene::_initializeScene()
{
	//MESHES AND MATERIALS
	#pragma region Meshes and Materials
	//MESHES

	//load a bunch of meshes we will be using throughout this demo
	//each mesh only has to be loaded once, but can be used multiple times:
	//F is flat shaded, S is smooth shaded (normals aligned or not), check the models folder!
	Mesh* planeMeshDefault = Mesh::load(config::MGE_MODEL_PATH + "plane.obj");
	Mesh* cubeMeshF = Mesh::load(config::MGE_MODEL_PATH + "cube_flat.obj");
	Mesh* cylinderMesh = Mesh::load(config::MGE_MODEL_PATH + "cylinder_smooth.obj");

	Mesh* Mesh1 = Mesh::load(config::MGE_MODEL_PATH + config::MESH_ONE);
	Mesh* Mesh2 = Mesh::load(config::MGE_MODEL_PATH + config::MESH_TWO);
	Mesh* Mesh3 = Mesh::load(config::MGE_MODEL_PATH + config::MESH_THREE);

	//MATERIALS

	//create some materials to display the cube, the plane and the light
	AbstractMaterial* lightMaterial = new ColorMaterial(glm::vec3(0.5f, 0.5f, 0.5f));
	AbstractMaterial* brickMaterial = new TextureMaterial(Texture::load(config::MGE_TEXTURE_PATH + "bricks.jpg"));
	AbstractMaterial* groundMaterial = new TextureMaterial(Texture::load(config::MGE_TEXTURE_PATH + "land.jpg"));

	AbstractMaterial* Texture1 = new TextureMaterial(Texture::load(config::MGE_TEXTURE_PATH + config::TEXTURE_ONE));
	AbstractMaterial* Texture2 = new TextureMaterial(Texture::load(config::MGE_TEXTURE_PATH + config::TEXTURE_TWO));
	AbstractMaterial* Texture3 = new TextureMaterial(Texture::load(config::MGE_TEXTURE_PATH + config::TEXTURE_THREE));
	#pragma endregion

	//SCENE SETUP
	#pragma region Object loading
	//add camera first (it will be updated last)
	Camera* camera = new Camera("camera", glm::vec3(25, 30, 25));
	camera->rotate(glm::radians(45.0f), glm::vec3(0, 1, 0));
	camera->rotate(glm::radians(-40.0f), glm::vec3(1, 0, 0));
	_world->add(camera);
	_world->setMainCamera(camera);

	//add the floor
	//GameObject* plane = new GameObject("plane", glm::vec3(0, 0, 0));
	//plane->scale(glm::vec3(5, 5, 5));
	//plane->setMesh(planeMeshDefault);
	//plane->setMaterial(groundMaterial);
	//_world->add(plane);

	GameObject* Model1 = new GameObject("Model_one", glm::vec3(config::POSITION_ONE[0], config::POSITION_ONE[1], config::POSITION_ONE[2]));
	Model1->scale(glm::vec3(config::SCALE_ONE[0], config::SCALE_ONE[1], config::SCALE_ONE[2]));
	Model1->rotate(config::ROTATION_ONE * glm::pi<float>() / 180, glm::vec3(0, 1, 0));
	Model1->setMesh(Mesh1);
	Model1->setMaterial(Texture1);
	if (config::ADD_ONE == true) _world->add(Model1);

	GameObject* Model2 = new GameObject("Model_two", glm::vec3(config::POSITION_TWO[0], config::POSITION_TWO[1], config::POSITION_TWO[2]));
	Model2->scale(glm::vec3(config::SCALE_TWO[0], config::SCALE_TWO[1], config::SCALE_TWO[2]));
	Model2->rotate(config::ROTATION_TWO * glm::pi<float>() / 180, glm::vec3(0, 1, 0));
	Model2->setMesh(Mesh2);
	Model2->setMaterial(Texture2);
	if (config::ADD_TWO == true) _world->add(Model2);

	GameObject* Model3 = new GameObject("Model_three", glm::vec3(config::POSITION_THREE[0], config::POSITION_THREE[1], config::POSITION_THREE[2]));
	Model3->scale(glm::vec3(config::SCALE_THREE[0], config::SCALE_THREE[1], config::SCALE_THREE[2]));
	Model3->rotate(config::ROTATION_THREE * glm::pi<float>() / 180, glm::vec3(0, 1, 0));
	Model3->setMesh(Mesh3);
	Model3->setMaterial(Texture3);
	if (config::ADD_THREE == true) _world->add(Model3);

	//add a light. Note that the light does ABSOLUTELY ZIP! NADA ! NOTHING !
	//It's here as a place holder to get you started.
	//Note how the texture material is able to detect the number of lights in the scene
	//even though it doesn't implement any lighting yet!
	Light* light = new Light("light", glm::vec3(0, 4, 0));
	light->scale(glm::vec3(0.1f, 0.1f, 0.1f));
	light->setMesh(cubeMeshF);
	light->setMaterial(lightMaterial);
	_world->add(light);
	
	#pragma endregion


	//BEHAVIOUR SETUP
//	camera->addBehaviour(new CameraOrbitBehaviour(3.0f, 1.0f, 50.0f, 2.5f, Model1, _window, this));
	
	//sphere4->setBehaviour(new KeysBehaviour(5.0f, 45.0f));
	_camera = camera;
	//camera->setBehaviour(new KeysBehaviour(-5, 50));
}

void AssignmentScene::_render() 
{
	AbstractGame::_render();
	_updateHud();
}

void AssignmentScene::_updateHud() 
{
	float dt = 1.0f / 60.0f;

	std::string debugInfo = "";
	debugInfo += std::string("FPS:") + std::to_string((int)_fps) + "\n";
	glm::vec3 _cameraPos = _camera->getLocalPosition();
	debugInfo += std::string("Camera Pos:") + std::to_string(_cameraPos.x) + " " + 
		std::to_string(_cameraPos.y) + " " + std::to_string(_cameraPos.z) + "\n";
	//debugInfo += std::string("Camera Angle") + std::to_string(_camera->getTransform())

	_hud->setDebugInfo(debugInfo);
	_hud->draw();
}