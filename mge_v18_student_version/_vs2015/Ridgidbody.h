#ifndef RIDGIDBODY_H
#define RIDGIDBODY_H

#include <vector>
#include <typeindex>
#include <unordered_map>
#include <memory>
#include "glm.hpp"
/*#include <chrono/physics/ChSystemNSC.h>
#include <chrono/physics/ChBodyEasy.h>
#include <chrono/assets/ChObjShapeFile.h>
#include <chrono/assets/ChAsset.h>
#include <chrono/assets/ChTexture.h>
#include <chrono/core/ChVector.h>
#include <chrono>*/
#include "mge/core/GameObject.hpp"
#include "../src/mge/BulletCommonInterfaces/CommonRigidBodyBase.h"

class Ridgidbody //: public CommonRigidBodyBase
{
public:
	//Ridgidbody(struct GUIHelperInterface* help);
	Ridgidbody();
	virtual ~Ridgidbody();

	//void initializePhysics();

	//std::shared_ptr<chrono::ChBodyEasyBox> ridgidbody;
private:

};


#endif