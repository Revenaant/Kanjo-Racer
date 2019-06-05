#ifndef RESOURCE_MANAGER_H
#define RESOURCE_MANAGER_H

#include <vector>
#include <unordered_map>
#include "mge/core/Mesh.hpp"
#include "mge/materials/AbstractMaterial.hpp"
#include "Resource.h"

class ResourceManager {
public:
	~ResourceManager();

	void addMesh(Mesh* pMesh, const std::string& pName, ResourceSet pResourceSet = ResourceSet::Normal);
	Mesh* pullMesh(const std::string& pName);
	Mesh* pullMeshSet(ResourceSet pSet);

	//void addMaterial(AbstractMaterial* pMaterial, const std::string& pName, ResourceSet pResourceSet = ResourceSet::Normal);
	//AbstractMaterial* pullMaterial(const std::string& pName);
	void addMaterial(AbstractMaterial* pMaterial, Mesh* pMatchingMesh, ResourceSet pResourceSet);
	AbstractMaterial* pullMaterial(Mesh* pMatchingMesh);

	void addCarMesh(Mesh* pMesh, const std::string& pName);
	void addCarMat(AbstractMaterial* pMaterial, Mesh* pMatchingMesh, ResourceSet pResourceSet);
	AbstractMaterial* pullCarMat(Mesh* pMatchingMesh);
	Mesh* pullCarMesh();
	
	//AbstractMaterial* pullMaterialSet(ResourceSet pSet);

	void releaseResource(const std::string& pName);
	void releaseAll();

	void loadCarMeshes();
	void loadCarMats(ResourceSet pSet);

	void loadMountain();
	void loadVillage();
	void loadForest();
	void loadBeach();
	void loadKanjo();
	void loadCity();

	// Singleton pattern
	static ResourceManager& instance();

private:
	ResourceManager();
	//std::vector<Mesh*> _meshes;
	//std::vector<Texture*> _textures;

	std::unordered_map<std::string, Mesh*> _meshes;
	//std::unordered_map<std::string, AbstractMaterial*> _materials;
	std::unordered_map<Mesh*, AbstractMaterial*> _materials;

	std::unordered_map<std::string, Mesh*> _carMeshes;
	std::unordered_map<Mesh*, AbstractMaterial*> _carMats;
	//std::unordered_map<std::string, GameObject> _gameObjects;


	Mesh* player1;
	Mesh* player2;
	Mesh* bus;	
	Mesh* firetruck;
	Mesh* foodvan;
	Mesh* jeep;
	Mesh* kCar;
	Mesh* miniTruck;
	Mesh* taxi;
};

#endif	// RESOURCE_MANAGER_H
