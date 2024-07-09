#ifndef PICKUP_BEHAVIOUR
#define PICKUP_BEHAVIOUR

#include "mge/behaviours/AbstractBehaviour.hpp"

class GameObject;

class PickupBehaviour : public AbstractBehaviour {
public:
	PickupBehaviour();
	virtual ~PickupBehaviour();
	virtual void update(float pStep);
	void Activate(GameObject* pActivator);
protected:
private:
};

#endif // !PICKUP_BEHAVIOUR

