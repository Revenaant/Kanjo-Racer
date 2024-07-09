#ifndef GAME_SCENE_H
#define GAME_SCENE_H

#include "mge/core/AbstractGame.hpp"
//#include <chrono>
#include "../_vs2015/Script.h"
/*#include <chrono/physics/ChSystemNSC.h>
#include <chrono/physics/ChSystem.h>
#include <chrono/physics/ChSystemSMC.h>*/
#include "btBulletDynamicsCommon.h"
#include "BulletCollision/CollisionShapes/btHeightfieldTerrainShape.h"
#include "BulletDynamics/MLCPSolvers/btDantzigSolver.h"
#include "BulletDynamics/MLCPSolvers/btSolveProjectedGaussSeidel.h"
#include "BulletDynamics/MLCPSolvers/btMLCPSolver.h"
#include "BulletDynamics/Vehicle/btRaycastVehicle.h"
#include "BulletDynamics/ConstraintSolver/btHingeConstraint.h"
#include "BulletDynamics/ConstraintSolver/btSliderConstraint.h"
#include "LinearMath/btVector3.h"
#include "LinearMath/btAlignedObjectArray.h"
#include "btBulletCollisionCommon.h"
#include <Bullet3Collision\NarrowPhaseCollision\b3RigidBodyCL.h>
#include <Bullet3Dynamics\b3CpuRigidBodyPipeline.h>
#include <BulletCollision\CollisionShapes\btBoxShape.h>
#include <stdio.h>
#include <BulletCollision\Gimpact\btGImpactCollisionAlgorithm.h>
#include <string>

#include "Collider.h"

class DebugHud;
class Hud;

class GameScene : public AbstractGame
{
public:
	GameScene(bool debuging);
	virtual ~GameScene();

	virtual void initialize();
	void initializeMenu();
	void initializeGame(ResourceSet pSet);

	//Script script;

	void GameScene::loadMountain();
	void GameScene::loadForest();
	void GameScene::loadKanjo();
	void GameScene::loadVillage();
	void GameScene::loadBeach();
	void GameScene::loadCity();

	void restartLevel(ResourceSet pSet);

//---------------------------------------------------------------------

	void exitPhysics();

	virtual bool mouseMoveCallback(float x, float y)
	{
		return false;
	}

	virtual bool mouseButtonCallback(int button, int state, float x, float y)
	{
		return false;
	}

protected:
	virtual void _initializeScene();

	virtual void _render();
	
private:
	DebugHud* _hud;
	Hud* _menuHud;
	Hud* _gameHud;
	Hud* _resolutionScreen;
	Hud* _countDownHud;
	bool _loadData = true;

	//chrono::ChSystemNSC my_system;
	void _updateHud();

	void handleCollisions();
	void resolveCollisions(Collider* c1, Collider* c2, int i);

	bool _gameIsInitialized;
	bool _resolutionScreenIsActiveP1;
	bool _resolutionScreenIsActiveP2;
	bool _startingSounds = true;
	float _startTimer = 0;

	//----------------------------------
	//btRaycastVehicle* vehicle;

	btAlignedObjectArray<btCollisionShape*> collisionShapes;

	btRigidBody* createGroundRigidBodyFromShape(btCollisionShape* groundShape);

	btRigidBody* createChassisRigidBodyFromShape(btCollisionShape* chassisShape);

	/*void addWheels(
		btVector3* halfExtents,
		btRaycastVehicle* vehicle,
		btRaycastVehicle::btVehicleTuning tuning);*/
};



#endif //GAME_SCENE_H

