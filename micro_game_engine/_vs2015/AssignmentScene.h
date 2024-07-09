#ifndef ASSIGNMENTSCENE_H
#define ASSIGNMENTSCENE_H

#include "mge/core/AbstractGame.hpp"
#include "../_vs2015/Script.h"

class DebugHud;

class AssignmentScene : public AbstractGame
{
public:
	AssignmentScene(bool debuging);
	virtual ~AssignmentScene();

	virtual void initialize();

protected:
	virtual void _initializeScene();

	virtual void _render();

private:
	DebugHud* _hud;

	void _updateHud();

};

AssignmentScene::~AssignmentScene()
{
}

#endif //"ASSIGNMENTSCENE_H"

