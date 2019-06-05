#ifndef RESOURCE_H
#define RESOURCE_H

enum ResourceSet { Normal, Any, Mountain, Beach, Kanjo, Village, Forest, City, Car, Menu };

class Resource {
public:
	Resource();
	virtual ~Resource();

	virtual void loadResource();
	virtual void unloadResource();

	void setResourceId(unsigned id);
	unsigned getResourceId() const;
	ResourceSet getResourceSet() const;
	void setResourceSet(ResourceSet set);

protected:
	unsigned _ResourceId;
	ResourceSet _resourceSet = Normal;
};


#endif // RESOURCE_H

