#ifndef KEYSBEHAVIOUR_HPP
#define KEYSBEHAVIOUR_HPP

#include "mge/behaviours/AbstractBehaviour.hpp"
#include "btBulletDynamicsCommon.h"

/**
 * KeysBehaviour allows you to move an object using the keyboard in its own local space.
 * Left right turns, forward back moves.
 */
class KeysBehaviour : public AbstractBehaviour
{
	public:
	    //move speed is in units per second, turnspeed in degrees per second
		KeysBehaviour(float pMoveSpeed = 5, float pTurnSpeed = 45 /*btRaycastVehicle* pVehicle = nullptr*/);
		virtual ~KeysBehaviour();
		virtual void update( float pStep );

    private:
        float _moveSpeed;
        float _turnSpeed;
		btRaycastVehicle* _vehicle;

		//virtual bool keyboardCallback(btRaycastVehicle* vehicle);
};

#endif // KEYSBEHAVIOUR_HPP
