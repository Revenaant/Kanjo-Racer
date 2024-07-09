#include "ResourceManager.h"
#include "glm.hpp"
#include "glm\gtc\random.hpp"
#include <vector>
#include <algorithm>

#include "mge/materials/AbstractMaterial.hpp"
#include "mge/materials/TextureMaterial.hpp"
#include "mge/core/Texture.hpp"
#include "mge/config.hpp"

ResourceManager::ResourceManager() 
{
	std::cout << "Created Resource Manager" << std::endl;
	srand(time(nullptr));
}

ResourceManager::~ResourceManager() 
{
	_meshes.clear();
	_materials.clear();
	_carMats.clear();
	_carMeshes.clear();
}

void ResourceManager::addMesh(Mesh *pMesh, const std::string& pName, ResourceSet pResourceSet)
{
	/*Mesh* mesh = dynamic_cast<Mesh*>(pResource);
	if (mesh != nullptr) {
		_meshes.push_back(mesh);
	}*/

	if (pMesh != nullptr) {
		_meshes.emplace(pName, pMesh);
		pMesh->setResourceSet(pResourceSet);
	}
}

void ResourceManager::addCarMesh(Mesh* pMesh, const std::string& pName) 
{
	if (pMesh != nullptr) {
		_carMeshes.emplace(pName, pMesh);
		pMesh->setResourceSet(ResourceSet::Car);
	}
}

Mesh* ResourceManager::pullMesh(const std::string& pName)
{
	return _meshes[pName];
}

Mesh* ResourceManager::pullCarMesh() 
{
	auto random_it = std::next(std::begin(_carMeshes), glm::linearRand<int>(0, (int)_carMeshes.size() - 1));
	return random_it->second;
}

Mesh* ResourceManager::pullMeshSet(ResourceSet pSet) 
{
	// Store the values passed here
	std::vector<std::unordered_map<std::string, Mesh*>::iterator> numbersPassed;

	// Iterate only as much as the size of the collection
	for (int i = 0; i < _meshes.size(); i++) 
	{
		// Get a random object
		auto random_it = std::next(std::begin(_meshes), glm::linearRand<int>(0, (int)_meshes.size() -1));
		//std::cout << "Thing" << std::endl;

		// Skip if already used
		while ( std::find(numbersPassed.begin(), numbersPassed.end(), random_it) != numbersPassed.end())
		{
			random_it = std::next(std::begin(_meshes), glm::linearRand<int>(0, (int)_meshes.size() -1));
			//std::cout << "The other thing" << std::endl;
		}

		// Return the object
		if (random_it->second->getResourceSet() == pSet)
		{
			numbersPassed.clear();
			return random_it->second;
		}
		else {
			numbersPassed.push_back(random_it);
		}
	}
	return nullptr;
}

//void ResourceManager::addMaterial(AbstractMaterial* pMaterial, const std::string& pName, ResourceSet pResourceSet) 
//{
//	if (pMaterial != nullptr) 
//	{
//		_materials.emplace(pName, pMaterial);
//	}
//}

//AbstractMaterial* ResourceManager::pullMaterial(const std::string& pName) 
//{
//	return _materials[pName];
//}

void ResourceManager::addMaterial(AbstractMaterial* pMaterial, Mesh* pMatchingMesh, ResourceSet pResourceSet)
{
	if (pMaterial != nullptr)
	{
		_materials.emplace(pMatchingMesh, pMaterial);
	}
}

void ResourceManager::addCarMat(AbstractMaterial* pMaterial, Mesh* pMatchingMesh, ResourceSet pResourceSet) 
{
	if (pMaterial != nullptr) 
	{
		_carMats.emplace(pMatchingMesh, pMaterial);
	}
}

AbstractMaterial* ResourceManager::pullMaterial(Mesh* pMatchingMesh)
{
	return _materials[pMatchingMesh];
}

AbstractMaterial* ResourceManager::pullCarMat(Mesh* pMatchingMesh) 
{
	return _carMats[pMatchingMesh];
}

//AbstractMaterial* ResourceManager::pullMaterialSet(ResourceSet pSet)
//{
//	// Store the values passed here
//	std::vector<std::unordered_map<std::string, AbstractMaterial*>::iterator> numbersPassed;
//
//	// Iterate only as much as the size of the collection
//	for (int i = 0; i < _materials.size(); i++)
//	{
//		// Get a random object
//		auto random_it = std::next(std::begin(_materials), glm::linearRand(0, (int)_materials.size()) - 1);
//
//		// Skip if already used
//		while (std::find(_materials.begin(), _materials.end(), random_it) != _materials.end())
//		{
//			random_it = std::next(std::begin(_materials), glm::linearRand(0, (int)_materials.size()) - 1);
//		}
//
//		// Return the object
//		if (random_it->second->getResourceSet() == pSet)
//		{
//			return random_it->second;
//		}
//		else {
//			numbersPassed.push_back(random_it);
//		}
//	}
//}


void ResourceManager::releaseResource(const std::string& pName)
{

}

void ResourceManager::releaseAll()
{
	std::unordered_map< std::string, Mesh*>::iterator it = _meshes.begin();

	while (it != _meshes.end())
	{
		delete (*it).second;

		it = _meshes.erase(it);
	}

	std::unordered_map< Mesh*, AbstractMaterial*>::iterator itt = _materials.begin();

	while (itt != _materials.end())
	{
		delete (*itt).second;

		itt = _materials.erase(itt);
	}
}

ResourceManager& ResourceManager::instance() {
	static ResourceManager* instance = new ResourceManager();
	return *instance;
}




// Level sets 
#pragma region levelSets

void ResourceManager::loadCarMeshes() 
{
	player1 =		Mesh::load(config::MGE_MODEL_PATH + "Player_car_1.obj");
	player2 =		Mesh::load(config::MGE_MODEL_PATH + "Player_car_2.obj");

	bus =			Mesh::load(config::MGE_MODEL_PATH + "Car_Bus.obj");
	firetruck =		Mesh::load(config::MGE_MODEL_PATH + "Car_FireTruck.obj");
	foodvan =		Mesh::load(config::MGE_MODEL_PATH + "Car_Food_Van.obj");
	jeep =			Mesh::load(config::MGE_MODEL_PATH + "Car_Jeep.obj");
	kCar =			Mesh::load(config::MGE_MODEL_PATH + "Car_KCar.obj");
	miniTruck =		Mesh::load(config::MGE_MODEL_PATH + "Car_MiniTruck.obj");
	taxi =			Mesh::load(config::MGE_MODEL_PATH + "Car_Taxi.obj");

	addMesh(player1, "player1");
	addMesh(player2, "player2");

	addCarMesh(bus, "bus");
	addCarMesh(firetruck, "fireTruck");
	addCarMesh(foodvan, "foodVan");
	addCarMesh(jeep, "jeep");
	addCarMesh(kCar, "kCar");
	addCarMesh(miniTruck, "miniTruck");
	addCarMesh(taxi, "taxi");
}

void ResourceManager::loadCarMats(ResourceSet pSet) 
{
	_carMats.clear();

	std::string setName = "";
	std::string p1Name = "";
	std::string p2Name = "";
	
	std::string count1 = "IN_GAME/1.png";
	std::string count2 = "IN_GAME/2.png";
	std::string count3 = "IN_GAME/3.png";
	std::string countGO = "IN_GAME/GO!.png";

	switch (pSet) {
	case ResourceSet::Beach:
		p1Name = "Beach_02Player1.png";
		p2Name = "Beach_02Player2.png";

		setName = "Beach_Cars_Other.png";
		break;
	case ResourceSet::Mountain:
		p1Name = "Mountains_02Player1.png";
		p2Name = "Mountains_02Player2.png";

		setName = "Mountains_Cars_Other.png";
		break;
	case ResourceSet::Village:
		p1Name = "Village_02Player1.png";
		p2Name = "Village_02Player2.png";

		setName = "Village_Cars_Other.png";
		break;
	case ResourceSet::Forest:
		p1Name = "Forest_02Player1.png";
		p2Name = "Forest_02Player2.png";

		setName = "Forest_Cars_Other.png";
		break;
	case ResourceSet::Kanjo:
		p1Name = "Kanjo_02Player1.png";
		p2Name = "Kanjo_02Player2.png";

		setName = "Kanjo_Cars_Other.png";
		break;
	case ResourceSet::City:
		p1Name = "City_02Player1.png";
		p2Name = "City_02Player2.png";

		setName = "City_Cars_Other.png";
		break;
	}

	addCarMat(new TextureMaterial(Texture::load(config::MGE_TEXTURE_PATH + p1Name)), player1, pSet);
	addCarMat(new TextureMaterial(Texture::load(config::MGE_TEXTURE_PATH + p2Name)), player2, pSet);

	addCarMat(new TextureMaterial(Texture::load(config::MGE_TEXTURE_PATH + setName)), bus, pSet);
	addCarMat(new TextureMaterial(Texture::load(config::MGE_TEXTURE_PATH + setName)), firetruck, pSet);
	addCarMat(new TextureMaterial(Texture::load(config::MGE_TEXTURE_PATH + setName)), foodvan, pSet);
	addCarMat(new TextureMaterial(Texture::load(config::MGE_TEXTURE_PATH + setName)), jeep, pSet);
	addCarMat(new TextureMaterial(Texture::load(config::MGE_TEXTURE_PATH + setName)), kCar, pSet);
	addCarMat(new TextureMaterial(Texture::load(config::MGE_TEXTURE_PATH + setName)), miniTruck, pSet);
	addCarMat(new TextureMaterial(Texture::load(config::MGE_TEXTURE_PATH + setName)), taxi, pSet);
}

void ResourceManager::loadMountain() {
	loadCarMats(ResourceSet::Mountain);

	Mesh* straightMesh = Mesh::load(config::MGE_MODEL_PATH + "Environment_MountainStraight.obj");
	Mesh* bridgeMesh = Mesh::load(config::MGE_MODEL_PATH + "Environment_MountainBridge.obj");
	Mesh* doubleMesh = Mesh::load(config::MGE_MODEL_PATH + "Environment_MountainDoubleBridge.obj");

	addMesh(straightMesh, "straightMesh", ResourceSet::Mountain);
	addMesh(bridgeMesh, "bridgeMesh", ResourceSet::Mountain);
	addMesh(doubleMesh, "doubleMesh", ResourceSet::Mountain);

	AbstractMaterial* straightMat = new TextureMaterial(Texture::load(config::MGE_TEXTURE_PATH + "Mountains_Straight.jpg"));
	AbstractMaterial* bridgeMat = new TextureMaterial(Texture::load(config::MGE_TEXTURE_PATH + "Mountains_Bridge.jpg"));
	AbstractMaterial* doubleMat = new TextureMaterial(Texture::load(config::MGE_TEXTURE_PATH + "Mountains_BridgeDouble.jpg"));

	addMaterial(straightMat, straightMesh, ResourceSet::Mountain);
	addMaterial(bridgeMat, bridgeMesh, ResourceSet::Mountain);
	addMaterial(doubleMat, doubleMesh, ResourceSet::Mountain);
}

void ResourceManager::loadBeach() {
	loadCarMats(ResourceSet::Beach);

	Mesh* straightMesh = Mesh::load(config::MGE_MODEL_PATH + "Environment_BeachStraight.obj");
	Mesh* bridgeMesh = Mesh::load(config::MGE_MODEL_PATH + "Environment_BeachBridge.obj");
	Mesh* doubleMesh = Mesh::load(config::MGE_MODEL_PATH + "Environment_BeachDoubleBridge.obj");

	addMesh(straightMesh, "straightMesh", ResourceSet::Beach);
	addMesh(bridgeMesh, "bridgeMesh", ResourceSet::Beach);
	addMesh(doubleMesh, "doubleMesh", ResourceSet::Beach);

	AbstractMaterial* straightMat = new TextureMaterial(Texture::load(config::MGE_TEXTURE_PATH + "Beach_Straight_Baked.png"));
	AbstractMaterial* bridgeMat = new TextureMaterial(Texture::load(config::MGE_TEXTURE_PATH + "Beach_Bridge_Baked.png"));
	AbstractMaterial* doubleMat = new TextureMaterial(Texture::load(config::MGE_TEXTURE_PATH + "Beach_Bridgedouble_Baked.png"));

	addMaterial(straightMat, straightMesh, ResourceSet::Beach);
	addMaterial(bridgeMat, bridgeMesh, ResourceSet::Beach);
	addMaterial(doubleMat, doubleMesh, ResourceSet::Beach);
}

void ResourceManager::loadVillage() {
	loadCarMats(ResourceSet::Village);

	Mesh* straightMesh = Mesh::load(config::MGE_MODEL_PATH + "Village_Middle_v2.obj");
	Mesh* bridgeMesh = Mesh::load(config::MGE_MODEL_PATH + "Village_Doubke_Bridge.obj");

	addMesh(straightMesh, "straightMesh", ResourceSet::Village);
	addMesh(bridgeMesh, "bridgeMesh", ResourceSet::Village);

	AbstractMaterial* straightMat = new TextureMaterial(Texture::load(config::MGE_TEXTURE_PATH + "Village_Straight_02.png"));
	AbstractMaterial* bridgeMat = new TextureMaterial(Texture::load(config::MGE_TEXTURE_PATH + "Village_DOubleBridge_01.png"));

	addMaterial(straightMat, straightMesh, ResourceSet::Village);
	addMaterial(bridgeMat, bridgeMesh, ResourceSet::Village);
}

void ResourceManager::loadForest() {
	loadCarMats(ResourceSet::Forest);

	Mesh* straightMesh = Mesh::load(config::MGE_MODEL_PATH + "Environment_ForestStraight.obj");
	Mesh* bridgeMesh = Mesh::load(config::MGE_MODEL_PATH + "Environment_ForestBridge.obj");
	Mesh* doubleMesh = Mesh::load(config::MGE_MODEL_PATH + "Environment_ForestDoubleBridge.obj");

	addMesh(straightMesh, "straightMesh", ResourceSet::Forest);
	addMesh(bridgeMesh, "bridgeMesh", ResourceSet::Forest);
	addMesh(doubleMesh, "doubleMesh", ResourceSet::Forest);

	AbstractMaterial* straightMat = new TextureMaterial(Texture::load(config::MGE_TEXTURE_PATH + "Forest_Straight.png"));
	AbstractMaterial* bridgeMat = new TextureMaterial(Texture::load(config::MGE_TEXTURE_PATH + "Forest_Bridge.png"));
	AbstractMaterial* doubleMat = new TextureMaterial(Texture::load(config::MGE_TEXTURE_PATH + "Forest_DoubleBridge.png"));

	addMaterial(straightMat, straightMesh, ResourceSet::Forest);
	addMaterial(bridgeMat, bridgeMesh, ResourceSet::Forest);
	addMaterial(doubleMat, doubleMesh, ResourceSet::Forest);

}
void ResourceManager::loadKanjo() {
	loadCarMats(ResourceSet::Kanjo);

	Mesh* straightMesh = Mesh::load(config::MGE_MODEL_PATH + "ENV_KANJO_NORMAL.obj");
	Mesh* bridgeMesh = Mesh::load(config::MGE_MODEL_PATH + "ENV_KANJO_BRIDGE.obj");
	Mesh* doubleMesh = Mesh::load(config::MGE_MODEL_PATH + "ENV_KANJO_SPLIT.obj");
	Mesh* splitMesh = Mesh::load(config::MGE_MODEL_PATH + "ENV_KANJO_SPLIT2.obj");

	addMesh(straightMesh, "straightMesh", ResourceSet::Kanjo);
	addMesh(bridgeMesh, "bridgeMesh", ResourceSet::Kanjo);
	addMesh(doubleMesh, "doubleMesh", ResourceSet::Kanjo);
	addMesh(splitMesh, "splitMesh", ResourceSet::Kanjo);

	AbstractMaterial* straightMat = new TextureMaterial(Texture::load(config::MGE_TEXTURE_PATH + "ENV_KANJO_NORMAL.png"));
	AbstractMaterial* bridgeMat = new TextureMaterial(Texture::load(config::MGE_TEXTURE_PATH + "ENV_KANJO_BRIDGE.png"));
	AbstractMaterial* doubleMat = new TextureMaterial(Texture::load(config::MGE_TEXTURE_PATH + "ENV_KANJO_SPLIT.png"));
	AbstractMaterial* splitMat = new TextureMaterial(Texture::load(config::MGE_TEXTURE_PATH + "ENV_KANJO_SPLIT2.png"));

	addMaterial(straightMat, straightMesh, ResourceSet::Kanjo);
	addMaterial(bridgeMat, bridgeMesh, ResourceSet::Kanjo);
	addMaterial(doubleMat, doubleMesh, ResourceSet::Kanjo);
	addMaterial(splitMat, splitMesh, ResourceSet::Kanjo);
}
void ResourceManager::loadCity() {

}

#pragma endregion