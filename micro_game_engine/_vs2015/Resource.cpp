#include "Resource.h"

Resource::Resource() 
{
}

Resource::~Resource() 
{
}

void Resource::loadResource() {

}

void Resource::unloadResource() {

}

void Resource::setResourceId(unsigned id) {

}

unsigned Resource::getResourceId() const {
	return _ResourceId;
}

void Resource::setResourceSet(ResourceSet set) {
	_resourceSet = set;
}

ResourceSet Resource::getResourceSet() const {
	return _resourceSet;
}