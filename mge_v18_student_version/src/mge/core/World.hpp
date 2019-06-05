#ifndef WORLD_HPP
#define WORLD_HPP

#include "mge/core/GameObject.hpp"
#include "btBulletDynamicsCommon.h"
#include "../_vs2015/Collider.h"

class Camera;
class Light;

class World : public GameObject
{
	public:
        World();

		void setMainCamera (Camera* pCamera);
		Camera* getMainCamera();

        //only used internally, do not use from outside
        void registerLight (Light* pLight);
        void unregisterLight (Light* pLight);

		void registerDeathMarked(GameObject* pObject);
		void unregisterDeathMarked(GameObject* pObject);
		std::vector<GameObject*> getDeathMarks() const;

        Light* getLightAt (int pIndex);
        int getLightCount();

		btDiscreteDynamicsWorld* dynamicsWorld();
		std::vector<Collider*> colliders;

		bool gameDone = false;

	private:
	    Camera* _mainCamera;
	    std::vector<Light*> _lights;
		std::vector<GameObject*> _deathMarkedObjects;
		btDiscreteDynamicsWorld* _dynamicsWorld;

        World(const World&);
        World& operator=(const World&);
};


#endif // WORLD_HPP
