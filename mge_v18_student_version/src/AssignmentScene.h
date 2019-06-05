#ifndef ASSIGNMENTSCENE_H
#define ASSIGNMENTSCENE_H

#include "mge/core/AbstractGame.hpp"

class DebugHud;

class AssignmentScene : public AbstractGame
{
public:
	AssignmentScene();
	virtual ~AssignmentScene();

	virtual void initialize();

protected:
	virtual void _initializeScene();

	virtual void _render();

private:
	DebugHud* _hud;

	void _updateHud();

};

#endif //"ASSIGNMENTSCENE_H"

