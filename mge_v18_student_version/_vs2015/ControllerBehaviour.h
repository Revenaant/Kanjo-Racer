#ifndef CONTROLLERBEHAVIOUR_H
#define CONTROLLERBEHAVIOUR_H

#include "mge/behaviours/AbstractBehaviour.hpp"
#include "LinearMath/btVector3.h"

/**
* KeysBehaviour allows you to move an object using the keyboard in its own local space.
* Left right turns, forward back moves.
*/
class ControllerBehaviour : public AbstractBehaviour
{
public:
	//move speed is in units per second, turnspeed in degrees per second
	ControllerBehaviour(int pIndex = 0, float pMoveSpeedX = 5, float pMoveSpeedY = 5);
	virtual ~ControllerBehaviour();
	virtual void update(float pStep);

	// Inner move function to be used inside update
	btVector3 move(int index, float pStep);
	void callDaPolice();

private:
	int _index;
	float _moveSpeedX;
	float _moveSpeedY;

};

#endif 
