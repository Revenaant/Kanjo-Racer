#ifndef POSITION_TRACKER_H
#define POSITION_TRACKER_H

#include "mge/behaviours/AbstractBehaviour.hpp"
#include "glm.hpp"
#include "Hud.h"
#include "Script.h"

class PositionTrackerBehaviour : public AbstractBehaviour 
{
public:
	PositionTrackerBehaviour(float pLoseDistance, glm::vec3 pStartPos, sf::RenderWindow* pWindow);
	~PositionTrackerBehaviour();
	virtual void update(float pStep);
	float getTraveledDistance() const;
	void setPlayerHasLost(bool pHasLost);
	bool getPlayerHasLost();

private:
	const float _loseDistance;
	float _traveledDistance = 0.0f;
	bool _isMoving;
	glm::vec3 _startPos;
	sf::RenderWindow* _window;

	bool _playerHasLost = false;
};

#endif	// POSITION_TRACKER_H
