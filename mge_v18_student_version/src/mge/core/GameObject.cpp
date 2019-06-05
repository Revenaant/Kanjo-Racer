#include <iostream>
#include "GameObject.hpp"
#include "mge/config.hpp"
#include "Mesh.hpp"
#include "Texture.hpp"
#include "mge/materials/TextureMaterial.hpp"
#include "mge/behaviours/AbstractBehaviour.hpp"

GameObject::GameObject(const std::string& pName, const glm::vec3& pPosition )
:	_name( pName ), _transform( glm::translate( pPosition ) ), _parent(nullptr), _children(),
    _mesh( nullptr ), _material(nullptr), _world(nullptr)

{
}

GameObject::~GameObject()
{
    //detach all children
    std::cout << "GC running on:" << _name << std::endl;

    while (_children.size() > 0) {
        GameObject* child = _children[0];
        remove (child);
        delete child;
    }

	for (auto &mapValue : _behaviours) {
		delete mapValue.second;
	}

	setParent(nullptr);

    //do not forget to delete behaviour, material, mesh, collider manually if required!
}

void GameObject::setName (const std::string& pName)
{
    _name = pName;
}

std::string GameObject::getName() const
{
	return _name;
}

//-----------------------------------------------------Transform

void GameObject::setTransform (const glm::mat4& pTransform)
{
    _transform = pTransform;
}

const glm::mat4& GameObject::getTransform() const
{
    return _transform;
}

void GameObject::setLocalPosition (glm::vec3 pPosition)
{
    _transform[3] = glm::vec4 (pPosition,1);
}

glm::vec3 GameObject::getLocalPosition() const
{
	return glm::vec3(_transform[3]);
}

//-----------------------------------------------------Mesh and Material

void GameObject::setMaterial(AbstractMaterial* pMaterial)
{
    _material = pMaterial;
}

AbstractMaterial * GameObject::getMaterial() const
{
    return _material;
}

void GameObject::setMesh(Mesh* pMesh)
{
	_mesh = pMesh;
}

Mesh * GameObject::getMesh() const
{
    return _mesh;
}

void GameObject::loadnSetMesh(std::string pFileName) 
{
	Mesh* newMesh = Mesh::load(config::MGE_MODEL_PATH + pFileName);
	setMesh(newMesh);
}

void GameObject::loadnSetMaterial(std::string pFileName) 
{
	AbstractMaterial* newMat = new TextureMaterial(Texture::load(config::MGE_TEXTURE_PATH + pFileName));
	setMaterial(newMat);
}

//-----------------------------------------------------Behaviour

void GameObject::addBehaviour(AbstractBehaviour* pBehaviour) 
{
	const std::type_index* typeIndex = &std::type_index(typeid(*pBehaviour));
	_behaviours.insert(std::pair<std::string, AbstractBehaviour*>(typeIndex->name(), pBehaviour));
	pBehaviour->setOwner(this);
}

int GameObject::getBehaviourCount() 
{
	return _behaviours.size();
}

//--------------------------------------------------------Collider

Collider* GameObject::createCollider(ColliderTypee pType, glm::vec3 pSize) {
	if (pSize == glm::vec3(0, 0, 0)) pSize = getMesh()->size(getMesh());

	_collider = new Collider(this, pType, getLocalPosition(), pSize);
	return _collider;
}

Collider* GameObject::getCollider() {
	return _collider;
}

//--------------------------------------------------------Parenting

void GameObject::setParent (GameObject* pParent) {
    //remove from previous parent
    if (_parent != nullptr) {
        _parent->_innerRemove(this);
    }

    //set new parent
    if (pParent != nullptr) {
        pParent->_innerAdd(this);
    }

    //if we have been detached from our parent, make sure
    //the world reference for us and all our children is set to null
    //if we have been attached to a parent, make sure
    //the world reference for us and all our children is set to our parent world reference
    //(this could still be null if the parent or parent's parent is not attached to the world)
    if (_parent == nullptr) {
        _setWorldRecursively(nullptr);
    } else {
        //might still not be available if our parent is not part of the world
        _setWorldRecursively(_parent->_world);
    }
}

void GameObject::_innerRemove (GameObject* pChild) {
    for (auto i = _children.begin(); i != _children.end(); ++i) {
        if (*i == pChild) {
            _children.erase(i);
            pChild->_parent = nullptr;
            return;
        }
    }
}

void GameObject::_innerAdd(GameObject* pChild)
{
	_children.push_back(pChild);
   pChild->_parent = this;
}

void GameObject::add (GameObject* pChild) {
    pChild->setParent(this);
}

void GameObject::remove (GameObject* pChild) {
    pChild->setParent(nullptr);
}

GameObject* GameObject::getParent() const {
    return _parent;
}

GameObject* GameObject::getWorldObject() const {
	return (GameObject*)_world;
}

World* GameObject::getWorld() const {
	return _world;
}

////////////

//costly operation, use with care
glm::vec3 GameObject::getWorldPosition() const
{
	return glm::vec3(getWorldTransform()[3]);
}

//costly operation, use with care
glm::mat4 GameObject::getWorldTransform() const
{
	if (_parent == nullptr) return _transform;
	else return _parent->getWorldTransform() * _transform;
}

////////////

//-----------------------------------------------------Object manipulation

void GameObject::translate(glm::vec3 pTranslation)
{
	setTransform(glm::translate(_transform, pTranslation));
}

void GameObject::scale(glm::vec3 pScale)
{
	setTransform(glm::scale(_transform, pScale));
}

void GameObject::rotate(float pAngle, glm::vec3 pAxis)
{
	setTransform(glm::rotate(_transform, pAngle, pAxis));
}

//--------------------------------------------------------Update

void GameObject::update(float pStep)
{

	//make sure behaviours are updated after worldtransform is set
	for (auto &mapValue : _behaviours) {
		if (mapValue.second) {
			mapValue.second->update(pStep);
		}
	}

	for (int i = _children.size() - 1; i >= 0; --i) {
		_children[i]->update(pStep);
	}
	if (this->getParent() != NULL)
	{
		this->setLocalPosition(_parent->getLocalPosition() + this->getLocalPosition());
		//std::cout << "BITCONEEEEEEEEEEEEEEEEECT" << _parent->getWorldPosition() << std::endl;
	}
}

void GameObject::_setWorldRecursively (World* pWorld) {
    _world = pWorld;

    for (int i = _children.size()-1; i >= 0; --i ) {
        _children[i]->_setWorldRecursively (pWorld);
    }
}

int GameObject::getChildCount() const {
    return _children.size();
}

GameObject* GameObject::getChildAt(int pIndex) const {
    return _children[pIndex];
}

GameObject::GameObject(const GameObject &)
{
}

const std::string & GameObject::name()
{
	return _name;
}

const std::string & GameObject::type()
{
	return _type;
}

//Physics stuff
/*void GameObject::AddRidgidbody(glm::vec3 size, float density)
{
	ridgidbody = std::make_shared<chrono::ChBodyEasyBox>(size.x, size.y, size.z,        // x,y,z size
		density,         // density
		true,            // collide enable?
		true);           // visualization?>;
	
	ridgidbody->SetPos(chrono::ChVector<>(this->getWorldPosition().x, this->getWorldPosition().y, this->getWorldPosition().z));
	ridgidbody->SetPos_dt(chrono::ChVector<>(0, 5, 0));
	//auto mtexture = std::make_shared<chrono::ChTexture>();
	//mtexture->SetTextureFilename(texture);  // use shared surface properties
	//rb.AddAsset(mtexture);


}*/

